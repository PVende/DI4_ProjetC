#include "Instance.h"

void Instance_init(Instance * instance) {
    instance->solution = NULL;
    instance->nbJobs = 0;
    instance->nbMachine = 0;
    instance->times = NULL;
    instance->distances = NULL;
    instance->deliveryDates = NULL;

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
    if(instance == NULL)
        return NULL;

    unsigned int i;
    Instance * MALLOC(dup, Instance, 1);

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
			for(j = 0; j < i1->nbJobs + 1; i++)
				if(i1->distances[i][j] != i2->distances[i][j])
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
        MALLOC(instance->times[i], unsigned int, instance->nbMachine);

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

unsigned int Instance_eval(Instance * instance, int diversification) {
    if(instance == NULL)
        return 0;

    if(instance->solution == NULL)
        return 0;

    unsigned int i, j, obj = 0;
    unsigned int * CALLOC(costMachine, unsigned int, instance->nbJobs);
    unsigned int ** MALLOC(costJobMachine, unsigned int *, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++)
        CALLOC(costJobMachine[i], unsigned int, instance->nbMachine);

    for(i = 0; i < instance->nbJobs; i++) {
        obj = instance->solution->sequence->sequence[i];
        costMachine[0] += instance->times[obj][0];
        costJobMachine[obj][0] = costMachine[0];

        for(j = 1; j < instance->nbMachine; j++) {
            costMachine[j] = (costMachine[j - 1] > costMachine[j]) ? costMachine[j - 1] : costMachine[j];
            costMachine[j] += instance->times[obj][j];
            costJobMachine[obj][j] = costMachine[j];
        }
    }

    unsigned int nbTurn = instance->solution->batchList->size;
    int * MALLOC(soonerStart, int, nbTurn);

    for(i = 0; i < nbTurn; i++) {
        soonerStart[i] = -1;

        for(j = 0; j < instance->solution->batchList->batches[i]->size; j++)
            soonerStart[i] = ((int)costJobMachine[instance->solution->batchList->batches[i]->batch[j]][instance->nbMachine - 1] > soonerStart[i]) ?
            (int)costJobMachine[instance->solution->batchList->batches[i]->batch[j]][instance->nbMachine - 1] : soonerStart[i];
    }

    unsigned int turnNb = 0, stop = instance->nbJobs, startLocation, lagTotal = 0, startTime = soonerStart[turnNb], endTime = 0, arrivedTime = 0;
    unsigned int * CALLOC(actualDelay, unsigned int, instance->nbJobs);
    unsigned int * CALLOC(lag, unsigned int, instance->nbJobs);
    Batch * currentBatch;

    for(i = 0; i < instance->solution->batchList->size; i++) {
        currentBatch = instance->solution->batchList->batches[i];
        startLocation = stop;
        startTime = (endTime > (unsigned int)soonerStart[turnNb]) ? endTime : (unsigned int)soonerStart[turnNb];

        for(j = 0; j < currentBatch->size; j++) {
            arrivedTime = startTime + instance->distances[startLocation][currentBatch->batch[j]];
            actualDelay[currentBatch->batch[j]] = arrivedTime;

            if(!diversification)
                lag[currentBatch->batch[j]] = (actualDelay[currentBatch->batch[j]] - instance->deliveryDates[currentBatch->batch[j]] > 0) ?
                    actualDelay[currentBatch->batch[j]] - instance->deliveryDates[currentBatch->batch[j]] : 0;
            else
                lag[currentBatch->batch[j]] = actualDelay[currentBatch->batch[j]];

            lagTotal += lag[currentBatch->batch[j]];
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
    free(lag);

    return lagTotal;
}

void Instance_firstSolution(Instance * instance){
	Batch * batch; // Pourquoi des pointeurs et des mallocs ?
	BatchList * batchList;
	Solution * solution;
	Instance * currentSolution;
	Sequence * sequence;
    unsigned int * deliveryDatesCopy;
	unsigned int infinity = 0;
    unsigned int i,
				j,
				indexMin = 0,
				min,
				nbJobBatch = 1,
				evalCurrentSolution,
				evalBestSolution = infinity,
				counter;

    currentSolution = Instance_duplicate(instance);

    infinity--;

    MALLOC(sequence, Sequence, 1);
    Sequence_init(sequence);
    Sequence_allocate(sequence, instance->nbJobs);

    deliveryDatesCopy = duplicateArray(instance->deliveryDates, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++) {
        min = infinity;

        for(j = 0; j < instance->nbJobs; j++){
            if(deliveryDatesCopy[j] < min) {
                min = deliveryDatesCopy[j];
                indexMin = j;
            }
        }

        Sequence_add(sequence, indexMin);
        deliveryDatesCopy[indexMin] = infinity;
    }

    free(deliveryDatesCopy);

    MALLOC(solution, Solution, 1);
    MALLOC(batch, Batch, 1);
    MALLOC(batchList, BatchList, 1);

    while(nbJobBatch <= instance->nbJobs/2){
        Solution_init(solution);
        Solution_setSequence(solution, sequence);

        i = 0;
        BatchList_init(batchList);
        while(i < instance->nbJobs) {
            counter = 0;
            Batch_init(batch);

            while(counter < nbJobBatch && i < instance->nbJobs) {
                Batch_addJob(batch, solution->sequence->sequence[i]);
                i++;
                counter++;
            }

            BatchList_addBatch(batchList, batch);
            Batch_finalize(batch);
        }

        Solution_setBatchList(solution, batchList);
        BatchList_finalize(batchList);

        Instance_setSolution(currentSolution, solution);
        Solution_finalize(solution);

        evalCurrentSolution = Instance_eval(currentSolution, 0);

        if(evalCurrentSolution < evalBestSolution) {
            evalBestSolution = evalCurrentSolution;
            Instance_setSolution(instance, currentSolution->solution);
        }

        nbJobBatch++;
    }

    Sequence_finalize(sequence);
    free(sequence);

    free(batch);
    free(batchList);
    free(solution);

    Instance_finalize(currentSolution);
    free(currentSolution);
}
