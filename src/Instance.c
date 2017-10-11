#include "Instance.h"

void Instance_init(Instance * instance) {
    instance->solution = NULL;
    instance->nbJobs = 0;
    instance->nbMachine = 0;
    instance->times = NULL;
    instance->distances = NULL;
    instance->deliveryDates = NULL;

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

    MALLOC(instance->times, unsigned int *, instance->nbMachine);

    for(i = 0; i < instance->nbMachine; i++) {
        MALLOC(instance->times[i], unsigned int, instance->nbJobs);

        for(j = 0; j < instance->nbJobs; j++)
            if(fscanf(inputFile, "%u", &instance->times[i][j]) != 1)
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
    if(instance->solution == NULL)
        return 0;

    unsigned int i, j, job;
    unsigned int * CALLOC(costJob, unsigned int, instance->nbJobs);
    unsigned int ** CALLOC(costJobMachine, unsigned int *, instance->nbJobs);

    for(i = 0; i < instance->nbJobs; i++)
        CALLOC(costJobMachine[i], unsigned int, instance->nbMachine);

    for(i = 0; i < instance->nbJobs; i++) {
        job = instance->solution->sequence->sequence[i];
        costJob[0] += instance->times[job][0];
        costJobMachine[job][0] = costJob[0];

        for(j = 0; j < instance->nbMachine; j++) {
            costJob[j] = (costJob[j - 1] > costJob[j]) ? costJob[j - 1] + instance->times[job][j] : costJob[j] + instance->times[job][j];
            costJobMachine[job][j] = costJob[j];
        }
    }

    unsigned int nbTurn = instance->solution->batchList->size;
    int * CALLOC(soonerStart, int, nbTurn);

    for(i = 0; i < nbTurn; i++) {
        soonerStart[i] = -1;

        for(j = 0; j < instance->solution->batchList->batches[i]->size; j++)
            soonerStart[i] = ((int)costJobMachine[job][instance->nbMachine] > soonerStart[i]) ? (int)costJobMachine[job][instance->nbMachine] : soonerStart[i];
    }

    unsigned int turnNb = 0, stop = instance->nbJobs, startLocation, lagTotal = 0, startTime, endTime = 0, arrivedTime;
    unsigned int * CALLOC(actualDelay, unsigned int, instance->nbJobs);
    unsigned int * CALLOC(lag, unsigned int, instance->nbJobs);
    Batch * currentBatch;

    for(i = 0; i < nbTurn; i++) {
        currentBatch = instance->solution->batchList->batches[i];
        startLocation = stop;
        startTime = (endTime > (unsigned int)soonerStart[turnNb]) ? endTime : (unsigned int)soonerStart[turnNb];

        for(j = 0; j < currentBatch->size; j++) {
            arrivedTime = startTime + instance->distances[startLocation][currentBatch->batch[j]];
            actualDelay[currentBatch->batch[j]] = arrivedTime;

            lag[currentBatch->batch[j]] = (diversification) ? actualDelay[currentBatch->batch[j]] :
                ((actualDelay[currentBatch->batch[j]] - instance->deliveryDates[currentBatch->batch[j]] > 0) ?
                    actualDelay[currentBatch->batch[j]] - instance->deliveryDates[currentBatch->batch[j]] : 0);

            lagTotal += lag[currentBatch->batch[j]];
            startTime = arrivedTime;
            startLocation = currentBatch->batch[j];
        }

        endTime = arrivedTime + instance->distances[currentBatch->batch[currentBatch->size - 1]][stop];
        turnNb++;
    }

    free(costJob);
    for(i = 0; i < instance->nbJobs; i++)
        free(costJobMachine[i]);
    free(costJobMachine);

    free(soonerStart);

    free(actualDelay);
    free(lag);

    return lagTotal;
}
