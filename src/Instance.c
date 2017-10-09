#include "Instance.h"

void Instance_init(Instance * instance) {
    instance->solution = NULL;
    instance->nbJobs = 0;
    instance->nbMachine = 0;
    instance->times = NULL;
    instance->distances = NULL;
    instance->deliveryDates = NULL;

}

void Instance_parseInstance(Instance * instance) {
    unsigned int i, j;
    FILE * inputFile;

    if((inputFile = fopen("test_files/input.txt", "r")) == NULL)
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

    fgetc(inputFile);
    fgetc(inputFile);

    unsigned int distributionLocation = 0, * distancesFromFirstLocation = NULL, endline = 0, tmpChar, distance;

    while(!endline) {
        distance = 0;
        tmpChar = fgetc(inputFile);

        if(tmpChar != '\n') {
            while(tmpChar != '\t') {
                distance = 10 * distance + (tmpChar - '0');
                tmpChar = fgetc(inputFile);
            }

            distributionLocation++;
            REALLOC(distancesFromFirstLocation, unsigned int, distributionLocation);
            distancesFromFirstLocation[distributionLocation - 1] = distance;
        } else
            endline++;

    }

    MALLOC(instance->distances, unsigned int *, distributionLocation);

    instance->distances[0] = distancesFromFirstLocation;

    for(i = 1; i < distributionLocation; i++) {
        MALLOC(instance->distances[i], unsigned int, distributionLocation);

        for(j = 0; j < distributionLocation; j++)
            if(fscanf(inputFile, "%u", &instance->distances[i][j]) != 1)
                fatalError("error read file");
    }

    if(fclose(inputFile) != 0)
       fatalError("error close input file");
}
