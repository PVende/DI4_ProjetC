#include "Instance.h"

void Instance_init(Instance * instance) {
    instance->solution = NULL;
    instance->nbJobs = 0;
    instance->nbMachine = 0;
    instance->times = NULL;
    instance->distances = NULL;
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

    if(instance->distances != NULL) {
        for(i = 0; i < instance->nbJobs + 1; i++)
            free(instance->distances[i]);
        free(instance->distances);
        instance->distances = NULL;
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

    MALLOC(dup->distances, unsigned int *, dup->nbJobs + 1);
    for(i = 0; i < dup->nbJobs + 1; i++)
        dup->distances[i] = duplicateArray(instance->distances[i], instance->nbJobs + 1);

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

    MALLOC(instance->distances, unsigned int *, instance->nbJobs + 1);

    for(i = 0; i < instance->nbJobs + 1; i++) {
        MALLOC(instance->distances[i], unsigned int, instance->nbJobs + 1);

        for(j = 0; j < instance->nbJobs + 1; j++)
            if(fscanf(inputFile, "%u", &instance->distances[i][j]) != 1)
            fatalError("error read file");
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
            arrivedTime = startTime + instance->distances[startLocation][currentBatch->batch[j]];
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

        endTime = arrivedTime + instance->distances[currentBatch->batch[currentBatch->size - 1]][stop];
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

void Instance_randomizeSolution(Instance * instance, Config * cfg)
{
    int i;

    srand(time(NULL));

//    if(cfg->SWAP_BOTH){
//        int nbSwaps = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//        for(i = 0; i < nbSwaps; i++){
//            int swap_i, swap_j;
//            int batch_i, batch_j;
//            Batch * batch1, * batch2;
//
//            // SEQ
//
//            TWO_RAND_INT(swap_i, swap_j, 0, instance->nbJobs - 1);
//
//            Sequence_swap(instance->solution->sequence, swap_i, swap_j);
//
//            // BATCH LIST
//
//            TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
//            batch1 = instance->solution->batchList->batches[batch_i];
//            batch2 = instance->solution->batchList->batches[batch_j];
//
//            swap_i = RAND(0, batch1->size - 1);
//            swap_j = RAND(0, batch2->size - 1);
//
//            BatchList_swap(instance->solution->batchList, batch_i, swap_i, batch_j, swap_j);
//        }
//    }
//
//    if(cfg->SWAP_BATCH){
//        int nbSwaps = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//        for(i = 0; i < nbSwaps; i++){
//            int swap_i, swap_j;
//            int batch_i, batch_j;
//            Batch * batch1, * batch2;
//
//            TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
//            batch1 = instance->solution->batchList->batches[batch_i];
//            batch2 = instance->solution->batchList->batches[batch_j];
//
//            swap_i = RAND(0, batch1->size - 1);
//            swap_j = RAND(0, batch2->size - 1);
//
//            BatchList_swap(instance->solution->batchList, batch_i, swap_i, batch_j, swap_j);
//        }
//    }

//    if(cfg->SWAP_SEQ){
//        int nbSwaps = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//        for(i = 0; i < nbSwaps; i++){
//            int swap_i, swap_j;
//            TWO_RAND_INT(swap_i, swap_j, 0, instance->nbJobs - 1);
//            Sequence_swap(instance->solution->sequence, swap_i, swap_j);
//        }
//    }
//
//     if(cfg->EBSR_BOTH)
//     {
//         int nbEbsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//         for(i = 0; i < nbEbsr; i++)
//         {
//             int ebsr_i, ebsr_j;
//             int batch_i, batch_j;
//             Batch * batch1, * batch2;
//
//             // SEQ
//             TWO_RAND_INT(ebsr_i, ebsr_j, 0, instance->nbJobs - 1);
//             Sequence_ebsr(instance->solution->sequence, ebsr_i, ebsr_j);
//
//             // BATCH LIST
//             TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
//             batch1 = instance->solution->batchList->batches[batch_i];
//             batch2 = instance->solution->batchList->batches[batch_j];
//
//             ebsr_i = RAND(0, batch1->size - 1);
//             ebsr_j = RAND(0, batch2->size - 1);
//
//             BatchList_ebsr(instance->solution->batchList, batch_i, ebsr_i, batch_j, ebsr_j);
//         }
//     }


    if(cfg->EBSR_BATCH)
    {
        int nbEbsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
        for(i = 0; i < nbEbsr; i++)
        {
            int ebsr_i, ebsr_j;
            int batch_i, batch_j;
            Batch * batch1, * batch2;

            TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
            batch1 = instance->solution->batchList->batches[batch_i];
            batch2 = instance->solution->batchList->batches[batch_j];

            ebsr_i = RAND(0, batch1->size - 1);
            ebsr_j = RAND(0, batch2->size - 1);

            BatchList_ebsr(instance->solution->batchList, batch_i, ebsr_i, batch_j, ebsr_j);
        }
    }


//    if(cfg->EBSR_SEQ)
//    {
//        int nbEbsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//        for(i = 0; i < nbEbsr; i++)
//        {
//            int ebsr_i, ebsr_j;
//            TWO_RAND_INT(ebsr_i, ebsr_j, 0, instance->nbJobs - 1);
//            Sequence_ebsr(instance->solution->sequence, ebsr_i, ebsr_j);
//        }
//    }

//     if(cfg->EFSR_BOTH)
//     {
//         int nbEfsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//         for(i = 0; i < nbEfsr; i++)
//         {
//             int efsr_i, efsr_j;
//             int batch_i, batch_j;
//             Batch * batch1, * batch2;
//
//             // SEQ
//             TWO_RAND_INT(efsr_i, efsr_j, 0, instance->nbJobs - 1);
//             Sequence_efsr(instance->solution->sequence, efsr_i, efsr_j);
//
//             // BATCH LIST
//             TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
//             batch1 = instance->solution->batchList->batches[batch_i];
//             batch2 = instance->solution->batchList->batches[batch_j];
//
//             efsr_i = RAND(0, batch1->size - 1);
//             efsr_j = RAND(0, batch2->size - 1);
//
//             BatchList_efsr(instance->solution->batchList, batch_i, efsr_i, batch_j, efsr_j);
//         }
//     }


     if(cfg->EFSR_BATCH)
     {
         int nbEfsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
         for(i = 0; i < nbEfsr; i++)
         {
             int efsr_i, efsr_j;
             int batch_i, batch_j;
             Batch * batch1, * batch2;

             TWO_RAND_INT(batch_i, batch_j, 0, instance->solution->batchList->size - 1);
             batch1 = instance->solution->batchList->batches[batch_i];
             batch2 = instance->solution->batchList->batches[batch_j];

             efsr_i = RAND(0, batch1->size - 1);
             efsr_j = RAND(0, batch2->size - 1);

             BatchList_efsr(instance->solution->batchList, batch_i, efsr_i, batch_j, efsr_j);
         }
     }


//    if(cfg->EFSR_SEQ)
//    {
//        int nbEfsr = RAND(instance->nbJobs / 4, instance->nbJobs * 4);
//        for(i = 0; i < nbEfsr; i++)
//        {
//            int efsr_i, efsr_j;
//            TWO_RAND_INT(efsr_i, efsr_j, 0, instance->nbJobs - 1);
//            Sequence_efsr(instance->solution->sequence, efsr_i, efsr_j);
//        }
//    }
}
