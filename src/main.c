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

#define INPUT_FILENAME "test_files/input.txt"
#define OUTPUT_FILENAME "test_files/output.txt"
#define CONFIG_FILENAME "configs.txt"


int main(void)
{
	signal(SIGABRT, &on_sigabrt);

	#ifndef NDEBUG

	TestRunner_runTests();

	#endif

	unsigned int tabuListSize = 7,
                delta,
                deltaEbfsrSolo = 3,
                nbIteration = 0,
                nbIterationWithoutImprovment = 0,
                nbMaxIteration = 2000,
                nbMaxIterationWithoutImprovment,
                diversification = 0,
                timeLimit,
                startTime,
                endTime,
                improvment,
                bestInstanceEval,
                bestNeighbourEval,
                currentInstanceEval,
                stop,
                i, j,
                indexI, indexJ;
    double cpuTime = 0;
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
	Instance_parseInstance(&bestInstance, INPUT_FILENAME, CONFIG_FILENAME);

	timeLimit = bestInstance.nbJobs * bestInstance.nbMachine / 4;

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

    if(fprintf(outputFile, "%u\t", Instance_eval(&bestInstance, diversification)) == 0)
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

    return 0;
}
