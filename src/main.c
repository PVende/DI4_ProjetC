#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"
#include "BatchList.h"
#include "Instance.h"
#include "TabuList.h"
#include "ArgsParser.h"
#include "Checker.h"

#ifndef NDEBUG
    #include "../tests/TestsRunner.h"
#endif

void run(Args * args);


int main(int argc, char * argv[])
{
	signal(SIGABRT, &on_sigabrt);
    srand(time(NULL));

    Args * args = Args_build(argc, argv);

    if(args->debugArgs)
        Args_debug(args);

    #ifndef NDEBUG

    if(!args->disableTests)
	   TestRunner_runTests();

	#endif

    if(args->help){
        Args_showHelp();
        exit(0);
    }

	if(*args->check != 0){
        if(fileExists(args->check)){
                if(*args->inputFile != 0 && fileExists(args->inputFile)){
                    Check_checkSolution(args->inputFile, args->check, args);
                }
                else{
                    fatalError("Can't check the output file : the input file is missing or invalid.");
                }
        }
        else{
            fatalError("Invalid file for option --check");
        }
    }

	// with configs
	if(*args->configFile != 0){
        if(!fileExists(args->configFile))
            fatalError("Invalid config file");
    }
    else{
        strcpy(args->configFile, "configs.txt");
    }


    if(*args->outputFile == 0){
        strcpy(args->outputFile, "output.txt");
    }

    if(*args->inputFile != 0){
        if(!fileExists(args->inputFile))
            fatalError("Invalid input file");
    }
    else{
        strcpy(args->inputFile, "input.txt");
    }

    // -----------

    run(args);

    Args_destroy(&args);
    return 0;
}

void run(Args * args){
	unsigned int tabuListSize = 7,
                delta,
                deltaEbfsrSolo = 3,
                nbIteration = 0,
                nbIterationWithoutImprovment = 0,
                nbMaxIteration,
                nbMaxIterationWithoutImprovment,
                diversification = 0,
                startTime,
                endTime,
                improvment,
                bestInstanceEval,
                bestNeighbourEval,
                currentInstanceEval,
                bestInstanceEvalSinceDiversification,
                stop,
                i, j,
                indexI, indexJ;
    double cpuTime = 0,
            timeLimit;
    char move;
	TabuList tabu;
	Instance bestInstance,
            bestNeighbour,
            currentInstance,
            currentInstanceSave;
	FILE * outputFile;
	Config cfg;

    Config_parseFile(&cfg, args->configFile, args);
    nbMaxIteration = cfg.ITERATIONS;

    int increasedAt[nbMaxIteration];
    unsigned int newValue[nbMaxIteration];
    unsigned int increases = 0;

	TabuList_init(&tabu);
	TabuList_setSize(&tabu, tabuListSize);

	Instance_init(&bestInstance);

    Instance_parseInstance(&bestInstance, args->inputFile);

    timeLimit = bestInstance.nbJobs * bestInstance.nbMachine / 4.0;

	batchAllocationStep = 4;
	batchListAllocationStep = bestInstance.nbJobs;

	if(bestInstance.nbJobs == 100) {
        delta = 8;
        nbMaxIterationWithoutImprovment = 5;
	}
    else if(bestInstance.nbJobs == 50) {
        delta = 10;
        nbMaxIterationWithoutImprovment = 8;
    }
    else {
        delta = bestInstance.nbJobs / 4;
        nbMaxIterationWithoutImprovment = 12;
    }

	srand(time(NULL));
	startTime = clock();

	Instance_firstSolution(&bestInstance, &cfg);
	bestInstanceEval = Instance_eval(&bestInstance, diversification);

	bestNeighbour = bestInstance;
	currentInstance = bestInstance;
	currentInstanceSave = bestInstance;

	bestNeighbour.solution = NULL;
	currentInstance.solution = NULL;
	Instance_setSolution(&currentInstance, bestInstance.solution);
	currentInstanceSave.solution = NULL;

	printf("\t%d\n", bestInstanceEval);

    bestInstanceEvalSinceDiversification = bestInstanceEval;

    increasedAt[increases] = -1 ;
    newValue[increases] = bestInstanceEval;
    increases++;

	while(cpuTime < timeLimit && nbIteration != nbMaxIteration) {
        improvment = 0;
        bestNeighbourEval = -1;
        stop = 0;

        if(cfg.SWAP_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_SWAP, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_SWAP;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.SWAP_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_SWAP, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_SWAP;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.SWAP_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_SWAP, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_SWAP;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.EBSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EBSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EBSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        stop = 0;

        if(cfg.EBSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_sequence(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EBSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EBSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        stop = 0;

        if(cfg.EBSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_sequence(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EBSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EBSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.EFSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EFSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EFSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.EFSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EFSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EFSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.EFSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, TABU_MOVE_EFSR, currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], cfg.LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = TABU_MOVE_EFSR;
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(cfg.FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(cfg.SPLIT) {
            for(i = 0; i < currentInstance.solution->batchList->size && !stop; i++){
                Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                BatchList_split(currentInstance.solution->batchList, i);
                currentInstanceEval = Instance_eval(&currentInstance, diversification);
                if(currentInstanceEval < bestNeighbourEval) {
                    bestNeighbourEval = currentInstanceEval;
                    Instance_setSolution(&bestNeighbour, currentInstance.solution);
                    if(cfg.FIRST_IMPROVE) {
                        stop = 1;
                        Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                    }
                }
                Instance_setSolution(&currentInstance, currentInstanceSave.solution);
            }
        }


        if(cfg.MERGE) {
            for(i = 0; i < currentInstance.solution->batchList->size - 1 && !stop; i++){
                Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                BatchList_merge(currentInstance.solution->batchList, i);
                currentInstanceEval = Instance_eval(&currentInstance, diversification);
                if(currentInstanceEval < bestNeighbourEval) {
                    bestNeighbourEval = currentInstanceEval;
                    Instance_setSolution(&bestNeighbour, currentInstance.solution);

                    if(cfg.FIRST_IMPROVE) {
                        stop = 1;
                        Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                    }
                }
                Instance_setSolution(&currentInstance, currentInstanceSave.solution);
            }
        }


        if(bestNeighbourEval != UINT_MAX) {
            Instance_setSolution(&currentInstance, bestNeighbour.solution);
            TabuList_insertTabu(&tabu, move, indexI, indexJ);
            if(args->print)
                printf("%d\n", bestNeighbourEval);
        }

        if(bestNeighbourEval < bestInstanceEval && !diversification) {
            bestInstanceEval = bestNeighbourEval;
            Instance_setSolution(&bestInstance, currentInstance.solution);
            if(args->print)
                printf("\t%d\n", bestInstanceEval);
            improvment = 1;
            nbIterationWithoutImprovment = 0;

            increasedAt[increases] = nbIteration;
            newValue[increases] = bestInstanceEval;
            increases++;
        }

        if(cfg.IMPROVEMENT_BASED_ON_DIVERSIFICATION){
            if(bestNeighbourEval < bestInstanceEvalSinceDiversification){
                improvment = 1;
                bestInstanceEvalSinceDiversification = bestNeighbourEval;
                nbIterationWithoutImprovment = 0;
            }
        }


        if(diversification)
            diversification = 0;


        if(!improvment)
            nbIterationWithoutImprovment++;


        if(nbIterationWithoutImprovment >= nbMaxIterationWithoutImprovment) {
            nbIterationWithoutImprovment = 0;

            if(cfg.DIVERSIFICATION){
                diversification = 1;
                TabuList_finalize(&tabu);
                TabuList_setSize(&tabu, tabuListSize);

                if(cfg.RANDOM_DIVERSIFICATION)
                    Instance_randomizeSolution(&currentInstance);
                
                if(args->print)
                    printf("Diversification\n");

                bestInstanceEvalSinceDiversification = UINT_MAX;
            }
        }

        endTime = clock();
        cpuTime += (double)(endTime - startTime)/CLOCKS_PER_SEC;
        startTime = endTime;
        nbIteration++;
	}

	if(nbIteration == nbMaxIteration + 1)
        nbIteration--;

    printf("\nIncreases profiler: \n");
    for(i = 0; i < increases; i++){
        printf("It# %d\t-->\t%d\n", increasedAt[i], newValue[i]);
    }
    printf("\n");

	printf("%u\t%f s\t%u iterations\n", bestInstanceEval, cpuTime, nbIteration);

    if((outputFile = fopen(args->outputFile, "w")) == NULL)
        fatalError("error open output file");

    if(fprintf(outputFile, "%u\t", bestInstanceEval) == 0)
        fatalError("error write file");

    if(fprintf(outputFile, "%f\t", cpuTime) == 0)
        fatalError("error write file");

    Instance_writeInstance(&bestInstance, outputFile);

    if(fclose(outputFile) != 0)
       fatalError("error close output file");

    TabuList_finalize(&tabu);

    Solution_finalize(currentInstance.solution);
    free(currentInstance.solution);
    Solution_finalize(bestNeighbour.solution);
    free(bestNeighbour.solution);
    Solution_finalize(currentInstanceSave.solution);
    free(currentInstanceSave.solution);
    Instance_finalize(&bestInstance);
}
