#include "Instance.h"

void Instance_init(Instance * instance) {
    instance->solution = NULL;
    instance->nbJobs = 0;
    instance->nbMachine = 0;
    instance->times = NULL;
    distances = NULL;
    instance->deliveryDates = NULL;
    instance->config = NULL;

}

void Instance_finalize(Instance * instance) {
    unsigned int i;

    if(instance->solution != NULL) {
        Solution_finalize(instance->solution);
        free(instance->solution);
        instance->solution = NULL;
    }

    if(instance->times != NULL) {
        for(i = 0; i < instance->nbJobs; i++)
            free(instance->times[i]);
        free(instance->times);
        instance->times = NULL;
    }

    if(distances != NULL) {
        for(i = 0; i < instance->nbJobs + 1; i++)
            free(distances[i]);
        free(distances);
        distances = NULL;
    }

    if(instance->deliveryDates != NULL) {
        free(instance->deliveryDates);
        instance->deliveryDates = NULL;
    }
}

Instance * Instance_duplicate(Instance * instance) {
    unsigned int i;
    Instance * dup;

    if(instance == NULL)
        return NULL;

    MALLOC(dup, Instance, 1);

    dup->nbJobs = instance->nbJobs;
    dup->nbMachine = instance->nbMachine;

    dup->deliveryDates = duplicateArray(instance->deliveryDates, instance->nbJobs);

    MALLOC(dup->times, unsigned int *, dup->nbJobs);
    for(i = 0; i < dup->nbJobs; i++)
        dup->times[i] = duplicateArray(instance->times[i], instance->nbMachine);

    dup->solution = Solution_duplicate(instance->solution);

    return dup;
}

int Instance_equals(Instance * i1, Instance * i2) {
    if(i1 == NULL && i2 == NULL)
        return 1;
    else if(i1 == NULL || i2 == NULL)
        return 0;

    if(Solution_equals(i1->solution, i2->solution) == 0)
        return 0;

    return 1;
}

void Instance_setSolution(Instance * instance, Solution * solution){
    if(instance != NULL) {
        if(instance->solution != NULL) {
            Solution_finalize(instance->solution);
            free(instance->solution);
            instance->solution = NULL;
        }

        if(solution != NULL)
            instance->solution = Solution_duplicate(solution);
    }
}

void Instance_parseInstance(Instance * instance, char * inputFileName) {
    unsigned int i, j;
    FILE * inputFile;

    if((inputFile = fopen(inputFileName, "r")) == NULL)
       fatalError("error open input file");

    if(fscanf(inputFile, "%u", &instance->nbMachine) != 1)
        fatalError("error read file");

    if(fscanf(inputFile, "%u", &instance->nbJobs) != 1)
        fatalError("error read file");

    MALLOC(instance->times, unsigned int *, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++)
        MALLOC(instance->times[i], unsigned int, instance->nbMachine);  //error

    for(i = 0; i < instance->nbMachine; i++) {
        for(j = 0; j < instance->nbJobs; j++)
            if(fscanf(inputFile, "%u", &instance->times[j][i]) != 1)
                fatalError("error read file");
    }

    MALLOC(instance->deliveryDates, unsigned int, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++)
        if(fscanf(inputFile, "%u", &instance->deliveryDates[i]) != 1)
            fatalError("error read file");

    MALLOC(distances, unsigned int *, instance->nbJobs + 1);

    for(i = 0; i < instance->nbJobs + 1; i++) {
        MALLOC(distances[i], unsigned int, instance->nbJobs + 1);

        for(j = 0; j < instance->nbJobs + 1; j++) {
            if(fscanf(inputFile, "%u", &distances[i][j]) != 1)
                fatalError("error read file");
        }
    }

    if(fclose(inputFile) != 0)
       fatalError("error close input file");
}

void Instance_writeInstance(Instance * instance, FILE * file) {
    if(instance != NULL)
        Solution_writeSolution(instance->solution, file);
    else
        fprintf(file, "null");
}

unsigned int Instance_eval(Instance * instance, unsigned int diversification) {
    unsigned int i, j,
                obj = 0,
                nbTurn,
                turnNb = 0,
                stop,
                startLocation,
                lagTotal = 0,
                startTime,
                endTime = 0,
                arrivedTime = 0;
    unsigned int * costMachine,
                   * actualDelay;
    unsigned int ** costJobMachine;
    int * soonerStart;

    Batch * currentBatch;
    int jobLag;

    if(instance == NULL)
        return 0;

    if(instance->solution == NULL)
        return 0;

    CALLOC(costMachine, unsigned int, instance->nbJobs);
    MALLOC(costJobMachine, unsigned int *, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++)
        CALLOC(costJobMachine[i], unsigned int, instance->nbMachine);

    for(i = 0; i < instance->nbJobs; i++) {
        obj = instance->solution->sequence->sequence[i];
        costMachine[0] += instance->times[obj][0];
        costJobMachine[obj][0] = costMachine[0];

        for(j = 1; j < instance->nbMachine; j++) {
            costMachine[j] = MAX(costMachine[j - 1], costMachine[j]) + instance->times[obj][j];
            costJobMachine[obj][j] = costMachine[j];
        }
    }

    nbTurn = instance->solution->batchList->size;
    MALLOC(soonerStart, int, nbTurn);

    for(i = 0; i < nbTurn; i++) {
        soonerStart[i] = -1;

        for(j = 0; j < instance->solution->batchList->batches[i]->size; j++)
            soonerStart[i] = MAX((int)costJobMachine[instance->solution->batchList->batches[i]->batch[j]][instance->nbMachine - 1], (int)soonerStart[i]);
    }

    stop = instance->nbJobs;
    startTime = soonerStart[turnNb];
    CALLOC(actualDelay, unsigned int, instance->nbJobs);

    for(i = 0; i < instance->solution->batchList->size; i++) {
        currentBatch = instance->solution->batchList->batches[i];
        startLocation = stop;
        startTime = MAX((int)endTime, (int)soonerStart[turnNb]);

        for(j = 0; j < currentBatch->size; j++) {
            arrivedTime = startTime + distances[startLocation][currentBatch->batch[j]];
            actualDelay[currentBatch->batch[j]] = arrivedTime;

            if(!diversification){
				jobLag = MAX((int) (actualDelay[currentBatch->batch[j]] - instance->deliveryDates[currentBatch->batch[j]]), 0);
            }
            else
                jobLag = (int) actualDelay[currentBatch->batch[j]];

            lagTotal += jobLag;
            startTime = arrivedTime;
            startLocation = currentBatch->batch[j];
        }

        endTime = arrivedTime + distances[currentBatch->batch[currentBatch->size - 1]][stop];
        turnNb++;
    }

    free(costMachine);
    for(i = 0; i < instance->nbJobs; i++)
        free(costJobMachine[i]);
    free(costJobMachine);

    free(soonerStart);

    free(actualDelay);

    return lagTotal;
}

void Instance_firstSolution(Instance * instance, Config * cfg){
	Batch batch;
	BatchList batchList;
	Solution solution;
	Instance currentSolution;
	Sequence sequence;
    unsigned int * deliveryDatesCopy;
	unsigned int infinity = -1;
    unsigned int i,
				j,
				indexMin = 0,
				min,
				nbJobBatch = 1,
				evalCurrentSolution,
				evalBestSolution = infinity,
				counter;

//    Instance_init(&currentSolution);
    currentSolution = *instance;
    currentSolution.solution = NULL;

    Sequence_init(&sequence);
    Sequence_allocate(&sequence, instance->nbJobs);

    deliveryDatesCopy = duplicateArray(instance->deliveryDates, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++) {
        min = infinity;

        for(j = 0; j < instance->nbJobs; j++){
            if(deliveryDatesCopy[j] < min) {
                min = deliveryDatesCopy[j];
                indexMin = j;
            }
        }

        Sequence_add(&sequence, indexMin);
        deliveryDatesCopy[indexMin] = infinity;
    }

    free(deliveryDatesCopy);

    while(nbJobBatch <= instance->nbJobs/2){
        Solution_init(&solution);
        Solution_setSequence(&solution, &sequence);

        i = 0;
        BatchList_init(&batchList);
        while(i < instance->nbJobs) {
            counter = 0;
            Batch_init(&batch);

            while(counter < nbJobBatch && i < instance->nbJobs) {
                Batch_addJob(&batch, solution.sequence->sequence[i]);
                i++;
                counter++;
            }

            BatchList_addBatch(&batchList, &batch);
            Batch_finalize(&batch);
        }

        Solution_setBatchList(&solution, &batchList);
        BatchList_finalize(&batchList);

        Instance_setSolution(&currentSolution, &solution);
        Solution_finalize(&solution);

        evalCurrentSolution = Instance_eval(&currentSolution, 0);

        if(evalCurrentSolution < evalBestSolution) {
            evalBestSolution = evalCurrentSolution;
            Instance_setSolution(instance, currentSolution.solution);
        }

        nbJobBatch++;
    }

    if(cfg && cfg->TWO_OPT) {
        evalBestSolution = Instance_eval(instance, 0);
        Solution_init(&solution);

        for(i = 0; i < instance->nbJobs - 1; i++) {
            Solution_setSequence(&solution, instance->solution->sequence);
            Solution_setBatchList(&solution, instance->solution->batchList);
            Solution_swap_both(&solution, i, i+1);
            Instance_setSolution(&currentSolution, &solution);
            evalCurrentSolution = Instance_eval(&currentSolution, 0);

            if(evalCurrentSolution < evalBestSolution) {
                evalBestSolution = evalCurrentSolution;
                Instance_setSolution(instance, currentSolution.solution);
            }

            Solution_finalize(&solution);
        }
    }

    Sequence_finalize(&sequence);

    Solution_finalize(currentSolution.solution);
    free(currentSolution.solution);
}

void Instance_randomizeSolution(Instance * instance)
{
    int i;
    int nbOperations = RAND(instance->nbJobs, instance->nbJobs * 3);

    // I. Shuffle

    for(i = 0; i < nbOperations; i++){
        int type = RAND(0, 12); // both + seq + batch + merge + split = 5

        if(type == 0 || type == 1 || type == 2){ // both
            int operatorType = RAND(0, 3); // swap OR ebsr OR efsr

            int job_i = RAND(0, instance->nbJobs);
            int job_j = RAND(0, instance->nbJobs);

            if(job_i == job_j) continue;

            switch(operatorType){
                case 0: Solution_swap_both(instance->solution, job_i, job_j); break;
                case 1: Solution_ebsr_both(instance->solution, job_i, job_j); break;
                case 2: Solution_efsr_both(instance->solution, job_i, job_j); break;
            }

        }
        else if(type == 3 || type == 4 || type == 5){ // seq
            int operatorType = RAND(0, 3); // swap OR ebsr OR efsr

            int pos_i = RAND(0, instance->nbJobs);
            int pos_j = RAND(0, instance->nbJobs);

            if(pos_i == pos_j) continue;

            switch(operatorType){
                case 0: Sequence_swap(instance->solution->sequence, pos_i, pos_j); break;
                case 1: Sequence_ebsr(instance->solution->sequence, pos_i, pos_j); break;
                case 2: Sequence_efsr(instance->solution->sequence, pos_i, pos_j); break;
            }
        }
        else if(type == 6 || type == 7 || type == 8){ // batch
            int operatorType = RAND(0, 3); // swap OR ebsr OR efsr

            int batch_i = RAND(0, instance->solution->batchList->size);
            int batch_j = RAND(0, instance->solution->batchList->size);

            int pos_i = RAND(0, instance->solution->batchList->batches[batch_i]->size);
            int pos_j = RAND(0, instance->solution->batchList->batches[batch_j]->size);

            if(batch_i == batch_j && pos_i == pos_j) continue;


            switch(operatorType){
                case 0: BatchList_swap(instance->solution->batchList, batch_i, pos_i, batch_j, pos_j); break;
                case 1: BatchList_ebsr(instance->solution->batchList, batch_i, pos_i, batch_j, pos_j); break;
                case 2: BatchList_efsr(instance->solution->batchList, batch_i, pos_i, batch_j, pos_j); break;
            }

        }
        else if(type == 9){ // merge
            int batch = RAND(0, instance->solution->batchList->size - 1);
            // printf("\tDIV -> BatchList_merge\n");
            BatchList_merge(instance->solution->batchList, batch);
        }
        else { // split
            int batch = RAND(0, instance->solution->batchList->size);
            // printf("\tDIV -> BatchList_split\n");
            BatchList_split(instance->solution->batchList, batch);
        }

    }

    // II. merge lonely batches and split too populated batches

    nbOperations = instance->solution->batchList->size;
    for(i = 0; i < nbOperations; i++){
        if(instance->solution->batchList->batches[i]->size == 1){
            BatchList_merge(instance->solution->batchList, i);
            nbOperations--;
        }
        while(instance->solution->batchList->batches[i]->size > 3){
            BatchList_split(instance->solution->batchList, i);
            nbOperations++;
        }
    }
}
