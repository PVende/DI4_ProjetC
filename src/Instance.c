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

    free(instance->config);

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

    dup->config = Config_duplicate(instance->config);

    return dup;
}

int Instance_equals(Instance * i1, Instance * i2) {
    if(i1 == NULL && i2 == NULL)
        return 1;
    else if(i1 == NULL || i2 == NULL)
        return 0;

    if(i1->nbJobs != i2->nbJobs || i1->nbMachine != i2->nbMachine)
        return 0;

    unsigned int i, j;

	if(i1->deliveryDates != i2->deliveryDates)
		for(i = 0; i < i1->nbJobs; i++)
			if(i1->deliveryDates[i] != i2->deliveryDates[i])
				return 0;

	if(i1->times != i2->times)
		for(i = 0; i < i1->nbJobs; i++)
			for(j = 0; j < i1->nbMachine; j++)
				if(i1->times[i][j] != i2->times[i][j])
					return 0;

	if(i1->distances != i2->distances)
		for(i = 0; i < i1->nbJobs + 1; i++)
			for(j = 0; j < i1->nbJobs + 1; j++)
				if(i1->distances[i][j] != i2->distances[i][j])
					return 0;

    if(Solution_equals(i1->solution, i2->solution) == 0)
        return 0;

    if(Config_equals(i1->config, i2->config) == 0)
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

void Instance_parseInstance(Instance * instance, char * inputFileName, char * cfgFile) {
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

    MALLOC(instance->config, Config, 1);
    Config_parseFile(instance->config, cfgFile);

    if(fclose(inputFile) != 0)
       fatalError("error close input file");
}

void Instance_writeResults(Instance * instance, char * outputFileName, unsigned int executionTime) {
    FILE * outputFile;

    if((inputFile = fopen(inputFileName, "w")) == NULL)
       fatalError("error open input file");

    if(instance != NULL) {
        if((fprintf(outputFile, "%d\t", Instance_eval(instance))) != 1)
            fatalError("error write file");

        if((fprintf(outputFile, "%d\t", executionTime)) != 1)
            fatalError("error write file");

        /*
        solution_writeResults(FILE*, solution)
        sequence_writeResults(FILE*, sequence)
        fprintf(file, "(%d", sequence[0])
        for(i = 1; i < sequence->size; i++)
            fprintf(file, ", %d", sequence[i])
        fprintf(file, ") ",)
        batchlist_writeResults(FILE*, batchlist)
        fprintf(file, "[",)
        batch_writeResults(FILE*, batch)
        fprintf(file, "(%d", batch[0])
        for(i = 1; i < batch->size; i++)
            fprintf(file, ", %d", batch[i])
        fprintf(file, ") ")
        fprintf(file, "]",)

        */
    }
}

unsigned int Instance_eval(Instance * instance) {
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

            if(!instance->config->DIVERSIFICATION){
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

void Instance_firstSolution(Instance * instance){
	Batch batch;
	BatchList batchList;
	Solution solution;
	Instance * currentSolution;
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

    currentSolution = Instance_duplicate(instance);

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

        Instance_setSolution(currentSolution, &solution);
        Solution_finalize(&solution);

        evalCurrentSolution = Instance_eval(currentSolution);

        if(evalCurrentSolution < evalBestSolution) {
            evalBestSolution = evalCurrentSolution;
            Instance_setSolution(instance, currentSolution->solution);
        }

        nbJobBatch++;
    }

    if(instance->config->FLAG_2OPT) {
        evalBestSolution = Instance_eval(instance);
        Solution_init(&solution);

        for(i = 0; i < instance->nbJobs - 1; i++) {
            solution = *Solution_duplicate(instance->solution);
            Solution_swap_both(&solution, i, i+1);
            Instance_setSolution(currentSolution, &solution);
            evalCurrentSolution = Instance_eval(currentSolution);

            if(evalCurrentSolution < evalBestSolution) {
                evalBestSolution = evalCurrentSolution;
                Instance_setSolution(instance, currentSolution->solution);
            }

            Solution_finalize(&solution);
        }
    }

    Sequence_finalize(&sequence);

    Instance_finalize(currentSolution);
    free(currentSolution);
}
