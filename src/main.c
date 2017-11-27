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
                cpuTime = 0,
                startTime,
                endTime,
                improvment,
                bestInstanceEval,
                bestNeighbourEval,
                currentSolutionEval,
                stop,
                i, j,
                indexI, indexJ;
    char move;
	TabuList tabu;
	Instance bestInstance,
            bestNeighbour,
            currentSolution;
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
	currentSolution = bestInstance;

	bestNeighbour.solution = NULL;
	currentSolution.solution = NULL;

	while(cpuTime/CLOCKS_PER_SEC < timeLimit && nbIteration <= nbMaxIteration) {
        improvment = 0;
        bestNeighbourEval = -1;
        stop = 0;

        if(bestInstance.config->SWAP_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_swap_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 's';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestInstance.config->SWAP_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_swap_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 's';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestInstance.config->SWAP_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_swap_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 's', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 't';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestInstance.config->EBSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_ebsr_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'b';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                            }
                        }
                    }
        }

        stop = 0;

        if(bestInstance.config->EBSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_ebsr_sequence(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'b';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                            }
                        }
                    }
        }

        stop = 0;

        if(bestInstance.config->EBSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
                for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_ebsr_sequence(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'b', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'c';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE) {
                                stop = 1;
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                            }
                        }
                    }
        }

        if(bestInstance.config->EFSR_BOTH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= delta) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_efsr_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'f';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestInstance.config->EFSR_SEQ) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_efsr_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'f';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestInstance.config->EFSR_BATCH) {
            for(i = 0; i < bestInstance.nbJobs - 1; i++)
                for(j = i + 1; j < bestInstance.nbJobs; j++)
                    if(j - i <= deltaEbfsrSolo) {
                        Instance_setSolution(&currentSolution, bestInstance.solution);
                        Solution_efsr_both(currentSolution.solution, i, j);
                        currentSolutionEval = Instance_eval(&currentSolution, diversification);
                        if(currentSolutionEval < bestNeighbourEval &&
                           !TabuList_isTabu(&tabu, 'f', currentSolution.solution->sequence->sequence[i],
                                                    currentSolution.solution->sequence->sequence[j], bestInstance.config->LOGICAL_TABU)) {
                            bestNeighbourEval = currentSolutionEval;
                            Instance_setSolution(&bestNeighbour, currentSolution.solution);

                            move = 'f';
                            indexI = currentSolution.solution->sequence->sequence[i];
                            indexJ = currentSolution.solution->sequence->sequence[j];

                            if(bestInstance.config->FIRST_IMPROVE)
                                Instance_setSolution(&bestInstance, bestNeighbour.solution);
                        }
                    }
        }

        if(bestNeighbourEval != (unsigned int)-1)
            TabuList_insertTabu(&tabu, move, indexI, indexJ);

        if(bestNeighbourEval < bestInstanceEval && !bestInstance.config->DIVERSIFICATION ) {
            bestInstanceEval = bestNeighbourEval;
            Instance_setSolution(&bestInstance, bestNeighbour.solution);
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
        }

        endTime = clock();
        cpuTime += startTime - endTime;
        nbIteration++;
	}

    if((outputFile = fopen(OUTPUT_FILENAME, "w")) == NULL)
        fatalError("error open output file");

    if((fprintf(outputFile, "%d\t", Instance_eval(&bestInstance, diversification))) == 0)
        fatalError("error write file");

    if((fprintf(outputFile, "%d\t", cpuTime/CLOCKS_PER_SEC)) == 0)
        fatalError("error write file");

    Instance_writeInstance(&bestInstance, outputFile);

    if(fclose(outputFile) != 0)
       fatalError("error close output file");

    return 0;
}
