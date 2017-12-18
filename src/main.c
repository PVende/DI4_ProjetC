#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <time.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"
#include "BatchList.h"
#include "Instance.h"
#include "TabuList.h"

#ifndef NDEBUG
	#include "../tests/TestsRunner.h"
#endif

#define INPUT_FILENAME "input.txt"
#define OUTPUT_FILENAME "output.txt"
#define CONFIG_FILENAME "configs.txt"

void run_configPreproc(int argc, char * argv[]);
void run_configTxt(int argc, char * argv[]);


int main(int argc, char * argv[])
{
	signal(SIGABRT, &on_sigabrt);

	#ifndef NDEBUG

	TestRunner_runTests();

	#endif

	// A calculer ?
	batchAllocationStep = 2;
	batchListAllocationStep = 10;
#if USE_PREPROC_FLAGS == 1
	run_configPreproc(argc, argv);
#else
	run_configTxt(argc, argv);
#endif // USE_PREPROC_FLAGS

    return 0;
}

void run_configTxt(int argc, char * argv[]){
	unsigned int tabuListSize = 7,
                delta,
                deltaEbfsrSolo = 3,
                nbIteration = 0,
                nbIterationWithoutImprovment = 0,
                nbMaxIteration = 2000,
                nbMaxIterationWithoutImprovment,
                diversification = 0,
                startTime,
                endTime,
                improvment,
                bestInstanceEval,
                bestNeighbourEval,
                currentInstanceEval,
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

	TabuList_init(&tabu);
	TabuList_setSize(&tabu, tabuListSize);

	Instance_init(&bestInstance);

	if(argc == 1)
        Instance_parseInstance(&bestInstance, INPUT_FILENAME, CONFIG_FILENAME);
    else if(argc == 2)
        Instance_parseInstance(&bestInstance, argv[1], CONFIG_FILENAME);

	timeLimit = bestInstance.nbJobs * bestInstance.nbMachine / 4.0;

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

	Instance_firstSolution(&bestInstance);
	bestInstanceEval = Instance_eval(&bestInstance, diversification);

	bestNeighbour = bestInstance;
	currentInstance = bestInstance;
	currentInstanceSave = bestInstance;

	bestNeighbour.solution = NULL;
	currentInstance.solution = NULL;
	Instance_setSolution(&currentInstance, bestInstance.solution);
	currentInstanceSave.solution = NULL;

	printf("\t%d\n", bestInstanceEval);

	while(cpuTime < timeLimit && nbIteration <= nbMaxIteration) {
        improvment = 0;
        bestNeighbourEval = -1;
        stop = 0;

        if(bestInstance.config->SWAP_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 's';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->SWAP_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 's';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->SWAP_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_swap_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 't';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->EBSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'b';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        stop = 0;

        if(bestInstance.config->EBSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_sequence(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'b';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        stop = 0;

        if(bestInstance.config->EBSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_ebsr_sequence(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'c';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                            }
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->EFSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'f';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->EFSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'f';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestInstance.config->EFSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentInstanceSave, currentInstance.solution);
                        Solution_efsr_both(currentInstance.solution, i, j);
                        currentInstanceEval = Instance_eval(&currentInstance, diversification);
                        if(currentInstanceEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
                                                    currentInstance.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentInstanceEval;
                            Instance_setSolution(&bestNeighbour, currentInstance.solution);

                            move = 'f';
                            indexI = currentInstance.solution->sequence->sequence[i];
                            indexJ = currentInstance.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
                        }
                        Instance_setSolution(&currentInstance, currentInstanceSave.solution);
                    }
        }

        if(bestNeighbourEval != (unsigned int)-1) {
            Instance_setSolution(&currentInstance, bestNeighbour.solution);
            TabuList_insertTabu(&tabu, move, indexI, indexJ);
            printf("%d\n", bestNeighbourEval);
        }

        if(bestNeighbourEval < bestInstanceEval && !diversification) {
            bestInstanceEval = bestNeighbourEval;
            Instance_setSolution(&bestInstance, currentInstance.solution);
            printf("\t%d\n", bestInstanceEval);
            improvment = 1;
            nbIterationWithoutImprovment = 0;
        }

        if(diversification)
            diversification = 0;

        if(!improvment)
            nbIterationWithoutImprovment++;

        if(nbIterationWithoutImprovment >= nbMaxIterationWithoutImprovment) {
            nbIterationWithoutImprovment = 0;

            if(bestInstance.config->DIVERSIFICATION)
                diversification = 1;
                TabuList_finalize(&tabu);
                TabuList_setSize(&tabu, tabuListSize);
                printf("Diversification\n");
        }

        endTime = clock();
        cpuTime += (double)(endTime - startTime)/CLOCKS_PER_SEC;
        startTime = endTime;
        nbIteration++;
	}

	if(nbIteration == nbMaxIteration + 1)
        nbIteration--;

	printf("%u\t%f s\t%u iterations\n", bestInstanceEval, cpuTime, nbIteration);

    if((outputFile = fopen(OUTPUT_FILENAME, "w")) == NULL)
        fatalError("error open output file");

    //ordre a remettre comme avant FO->cpu->sol

    if(fprintf(outputFile, "%f\t", cpuTime) == 0)
        fatalError("error write file");

    if(fprintf(outputFile, "%u\t", nbIteration) == 0)
        fatalError("error write file");

    if(fprintf(outputFile, "%u\t", bestInstanceEval) == 0)
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

void run_configPreproc(int argc, char * argv[])
{
	unsigned int tabuListSize = 7,
                delta,
                deltaEbfsrSolo = 3,
                nbIteration = 0,
                nbIterationWithoutImprovment = 0,
                nbMaxIteration = 2000,
                nbMaxIterationWithoutImprovment,
                diversification = 0,
                startTime,
                endTime,
                improvment,
                bestInstanceEval,
                bestNeighbourEval,
                currentInstanceEval,
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

	TabuList_init(&tabu);
	TabuList_setSize(&tabu, tabuListSize);

	Instance_init(&bestInstance);

	if(argc == 1)
        Instance_parseInstance(&bestInstance, INPUT_FILENAME, CONFIG_FILENAME);
    else if(argc == 2)
        Instance_parseInstance(&bestInstance, argv[1], CONFIG_FILENAME);

	timeLimit = bestInstance.nbJobs * bestInstance.nbMachine / 4.0;

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

	Instance_firstSolution(&bestInstance);
	bestInstanceEval = Instance_eval(&bestInstance, diversification);

	bestNeighbour = bestInstance;
	currentInstance = bestInstance;
	currentInstanceSave = bestInstance;

	bestNeighbour.solution = NULL;
	currentInstance.solution = NULL;
	Instance_setSolution(&currentInstance, bestInstance.solution);
	currentInstanceSave.solution = NULL;

	printf("\t%d\n", bestInstanceEval);

	while(cpuTime < timeLimit && nbIteration <= nbMaxIteration) {
        improvment = 0;
        bestNeighbourEval = -1;
        stop = 0;

#if SWAP_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 's';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // SWAP_BOTH


#if SWAP_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 's';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // SWAP_SEQ

#if SWAP_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 't';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // SWAP_BATCH

#if EBSR_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'b';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if FIRST_IMPROVE == 1
							stop = 1;
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EBSR_BOTH

	stop = 0;

#if EBSR_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_sequence(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'b';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if FIRST_IMPROVE == 1
						stop = 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EBSR_SEQ

	stop = 0;

#if EBSR_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_sequence(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'c';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if FIRST_IMPROVE == 1
						stop = 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EBSR_BATCH

#if EFSR_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EFSR_BOTH

#if EFSR_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EFSR_SEQ

#if EFSR_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // EFSR_BATCH

        if(bestNeighbourEval != (unsigned int)-1) {
            Instance_setSolution(&currentInstance, bestNeighbour.solution);
            TabuList_insertTabu(&tabu, move, indexI, indexJ);
            printf("%d\n", bestNeighbourEval);
        }

        if(bestNeighbourEval < bestInstanceEval && !diversification) {
            bestInstanceEval = bestNeighbourEval;
            Instance_setSolution(&bestInstance, currentInstance.solution);
            printf("\t%d\n", bestInstanceEval);
            improvment = 1;
            nbIterationWithoutImprovment = 0;
        }

        if(diversification)
            diversification = 0;

        if(!improvment)
            nbIterationWithoutImprovment++;

        if(nbIterationWithoutImprovment >= nbMaxIterationWithoutImprovment) {
            nbIterationWithoutImprovment = 0;

		#if DIVERSIFICATION == 1
			diversification = 1;
			TabuList_finalize(&tabu);
			TabuList_setSize(&tabu, tabuListSize);
			printf("Diversification\n");
		#endif
        }

		endTime = clock();
		cpuTime += (double)(endTime - startTime)/CLOCKS_PER_SEC;
		startTime = endTime;
		nbIteration++;
	}

	if(nbIteration == nbMaxIteration + 1)
        nbIteration--;

	printf("%u\t%f s\t%u iterations\n", bestInstanceEval, cpuTime, nbIteration);

    if((outputFile = fopen(OUTPUT_FILENAME, "w")) == NULL)
        fatalError("error open output file");

    //ordre a remettre comme avant FO->cpu->sol

    if(fprintf(outputFile, "%f\t", cpuTime) == 0)
        fatalError("error write file");

    if(fprintf(outputFile, "%u\t", nbIteration) == 0)
        fatalError("error write file");

    if(fprintf(outputFile, "%u\t", bestInstanceEval) == 0)
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
