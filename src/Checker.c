#include "Checker.h"
#include "helpers.h"
#include "Instance.h"
#include "Solution.h"
#include "Batch.h"
#include "BatchList.h"
#include "Sequence.h"

void Check_checkSolution(char * inputFilePath, char * ouputFilePath, Args * args){

    Checker checker;
    int eval;

    printf("Checking a solution...\n\n");

    Check_parseFile(&checker, inputFilePath, ouputFilePath, args);

    // -----

    printf("Calculating eval...");
    eval = Instance_eval(checker.instance, 0);
    printf(" OK\n\n");

    printf("Eval of the solution to check: %d\n", checker.eval);
    printf("Calcultated eval: %d\n\n", eval);
    if(eval == checker.eval){
        printf("The solution is correct!\n");
    }
    else{
        printf("Wut did u do, the solution is not correct... :(\n");
        printf("If there is a huge difference, mind checking that the solution file '%s', does indeed correspond to the instance '%s'.\n", ouputFilePath, inputFilePath);
    }

    // -----

    Instance_finalize(checker.instance);
    free(checker.instance);

    exit(0);
}

void Check_parseFile(Checker * checker, char * inputFilePath, char * outputFilePath, Args * args)
{
    FILE * output = fopen(outputFilePath, "r");

    if(!output){
        fatalError("Can't open solution file.");
    }
    fclose(output);

    printf("Initializing instance...");
    MALLOC(checker->instance, Instance, 1);
    Instance_init(checker->instance);
    printf(" OK\n");

    printf("Initializing solution...");
    MALLOC(checker->instance->solution, Solution, 1);
    Solution_init(checker->instance->solution);
    printf(" OK\n");

    printf("Initializing sequence...");
    MALLOC(checker->instance->solution->sequence, Sequence, 1);
    Sequence_init(checker->instance->solution->sequence);
    printf(" OK\n");

    printf("Initializing batch list...");
    MALLOC(checker->instance->solution->batchList, BatchList, 1);
    BatchList_init(checker->instance->solution->batchList);
    printf(" OK\n");

    printf("Parsing instance from file '%s'...", inputFilePath);
    Instance_parseInstance(checker->instance, inputFilePath, NULL);
    printf(" OK\n");

    printf("Parsing solution from file '%s'...", outputFilePath);
    Check_parseSolution(outputFilePath, checker);
    printf(" OK\n");

    printf("Nb jobs: %d\n", checker->instance->nbJobs);
    printf("Nb machines: %d\n", checker->instance->nbMachine);

    if(args->checkDetails){
        printf("Solution details: ");
        Solution_debug(checker->instance->solution);
        printf("\n\n");
    }

    if(checker->instance->solution->sequence->size != checker->instance->nbJobs){
        printf("\nThe solution file (%s) does not correspond to this instance file (%s): \n", outputFilePath, inputFilePath);
        printf("  --> Nb jobs in instance file: %d\n", checker->instance->nbJobs);
        printf("  --> tNb jobs in solution file: %d\n", checker->instance->solution->sequence->size);
        exit(0);
    }
}

void Check_parseSolution(char * solutionFilePath, Checker * checker){

    float tmp;
    FILE * file = fopen(solutionFilePath, "r");

    if(!file){
        fatalError("Impossible to open solution file.");
    }

    if(fscanf(file, "%d\t%f", &checker->eval, &tmp) == 0)
        fatalError("fscanf failed, impossible to find the eval value.");

    Check_parseSequence(file, checker);
    fgetc(file);
    Check_parseBatchList(file, checker);
}


void Check_parseSequence(FILE * file, Checker * checker){
    int i;
    int nbJobs = checker->instance->nbJobs;

    Sequence_allocate(checker->instance->solution->sequence, nbJobs);

    for(i = 0; i < nbJobs; i++){
        int job;

        cursorOnNextInt(file);
        if(fscanf(file, "%d", &job) == 0)
            break;

        Sequence_add(checker->instance->solution->sequence, job);
    }
}


void Check_parseBatchList(FILE * file, Checker * checker){
    int nbFoundJobs = 0;
    int nbJobs = checker->instance->nbJobs;
    BatchList * list = checker->instance->solution->batchList;

    Batch * batch;
    MALLOC(batch, Batch, 1);
    Batch_init(batch);

    while(1){
        char next = cursorOnNextIntOrClosingBrace(file);

        if(next == ']'){
            // new batch
            fgetc(file);
            BatchList_addBatch(list, batch); // the batch is duplicated
            Batch_finalize(batch);
            Batch_init(batch);

            if(nbFoundJobs == nbJobs)
                break;
        }
        else{
            // same batch
            int job;

            if(fscanf(file, "%d", &job) == 0)
                break;

            Batch_addJob(batch, job);

            nbFoundJobs++;
        }
    }

    Batch_finalize(batch);
    free(batch);
}
