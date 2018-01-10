#include "Checker.h"
#include "helpers.h"
#include "Instance.h"
#include "Solution.h"
#include "Batch.h"
#include "BatchList.h"

void Check_checkSolution(char * inputFilePath, char * ouputFilePath){

    FILE * output;
    Instance instance;


    // ----

    output = fopen(ouputFilePath, "r");

    if(!output){
        fatalError("Impossible to open solution file.");
    }

    // ----

    Instance_init(&instance);

    Instance_parseInstance(&instance, inputFilePath, NULL);

    printf("\n%d\n", instance.nbJobs);
    printf("\n%d\n", instance.nbMachine);
    printf("\n%d %d %d\n", instance.times[0][0], instance.times[0][1], instance.times[0][2]);

    Check_parseSolution(instance.solution, ouputFilePath);


    printf("Checking\n");
    exit(0);
}

void Check_parseSolution(Solution * solution, char * solutionFilePath){

    FILE * output = fopen(solutionFilePath, "r");

    if(!output){
        fatalError("Impossible to open solution file.");
    }

    printf("ok");
}
