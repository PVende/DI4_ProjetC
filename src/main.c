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
#include "flags.h"
#include "ArgsParser.h"
#include "Checker.h"

#include "../tests/TestsRunner.h"

#define INPUT_FILENAME "input.txt"
#define OUTPUT_FILENAME "output.txt"
#define CONFIG_FILENAME "configs.txt"

void run_configPreproc(Args * args);
void run_configTxt(Args * args);


int main(int argc, char * argv[])
{
	signal(SIGABRT, &on_sigabrt);

    Args * args = Args_build(argc, argv);

    #ifndef NDEBUG

	TestRunner_runTests();

	#endif

	// A calculer ?
	batchAllocationStep = 2;
	batchListAllocationStep = 10;

	int withCfg = 0;

    if(args->help){
        Args_showHelp();
        exit(0);
    }

	if(*args->check != 0){
        if(fileExists(args->check)){
                if(*args->inputFile != 0 && fileExists(args->inputFile)){
                    Check_checkSolution(args->inputFile, args->check);
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
        if(fileExists(args->configFile)){
            withCfg = 1;
        }
        else
            fatalError("Invalid config file");
    }
    else{
        strcpy(args->configFile, "configs.txt");
    }

    if(*args->inputFile != 0 && !fileExists(args->inputFile)){
        fatalError("Invalid input file");
    }
    else{
        strcpy(args->inputFile, "input.txt");
    }

    // -----------

    if(!withCfg)
    {
        printf("USING PREPROC FLAGS\n");
        run_configPreproc(args);
    }
    else
    {
        printf("USING CONFIG FILE\n");
        run_configTxt(args);
    }


    Args_destroy(&args);
    return 0;
}

void run_configTxt(Args * args){
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

    Instance_parseInstance(&bestInstance, args->inputFile, args->configFile);

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
        }

        if(diversification)
            diversification = 0;

        if(!improvment)
            nbIterationWithoutImprovment++;

        if(nbIterationWithoutImprovment >= nbMaxIterationWithoutImprovment) {
            nbIterationWithoutImprovment = 0;

            if(bestInstance.config->DIVERSIFICATION){
                diversification = 1;
                TabuList_finalize(&tabu);
                TabuList_setSize(&tabu, tabuListSize);
                if(args->print)
                    printf("Diversification\n");
            }
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

void run_configPreproc(Args * args)
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

    Instance_parseInstance(&bestInstance, args->inputFile, args->configFile);

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

#if PREPROC_SWAP_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 's';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if PREPROC_FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif //PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_SWAP_BOTH


#if PREPROC_SWAP_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 's';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if PREPROC_FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_SWAP_SEQ

#if PREPROC_SWAP_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_swap_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 's', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 't';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if PREPROC_FIRST_IMPROVE == 1
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_SWAP_BATCH

#if PREPROC_EBSR_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'b';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

						#if PREPROC_FIRST_IMPROVE == 1
							stop = 1;
							Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
						#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EBSR_BOTH

        stop = 0;

#if PREPROC_EBSR_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_sequence(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'b';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if PREPROC_FIRST_IMPROVE == 1
						stop = 1;
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EBSR_SEQ

        stop = 0;

#if PREPROC_EBSR_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1 && !stop; i++)
			for(j = i + 1; j < bestInstance.nbJobs && !stop; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_ebsr_sequence(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'b', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'c';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if PREPROC_FIRST_IMPROVE == 1
						stop = 1;
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EBSR_BATCH

#if PREPROC_EFSR_BOTH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= delta) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if PREPROC_FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EFSR_BOTH

#if PREPROC_EFSR_SEQ == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if PREPROC_FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EFSR_SEQ

#if PREPROC_EFSR_BATCH == 1
		for(i = 0; i < bestInstance.nbJobs - 1; i++)
			for(j = i + 1; j < bestInstance.nbJobs; j++)
				if(j - i <= deltaEbfsrSolo) {
					Instance_setSolution(&currentInstanceSave, currentInstance.solution);
					Solution_efsr_both(currentInstance.solution, i, j);
					currentInstanceEval = Instance_eval(&currentInstance, diversification);
					if(currentInstanceEval < bestNeighbourEval &&
					   !TabuList_isTabu(&tabu, 'f', currentInstance.solution->sequence->sequence[i],
												currentInstance.solution->sequence->sequence[j], PREPROC_LOGICAL_TABU)) {
						bestNeighbourEval = currentInstanceEval;
						Instance_setSolution(&bestNeighbour, currentInstance.solution);

						move = 'f';
						indexI = currentInstance.solution->sequence->sequence[i];
						indexJ = currentInstance.solution->sequence->sequence[j];

					#if PREPROC_FIRST_IMPROVE == 1
						Instance_setSolution(&currentInstanceSave, bestNeighbour.solution);
					#endif // PREPROC_FIRST_IMPROVE
					}
					Instance_setSolution(&currentInstance, currentInstanceSave.solution);
				}
#endif // PREPROC_EFSR_BATCH

        if(bestNeighbourEval != (unsigned int)-1) {
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
        }

        if(diversification)
            diversification = 0;

        if(!improvment)
            nbIterationWithoutImprovment++;

        if(nbIterationWithoutImprovment >= nbMaxIterationWithoutImprovment) {
            nbIterationWithoutImprovment = 0;

		#if PREPROC_DIVERSIFICATION == 1
			diversification = 1;
			TabuList_finalize(&tabu);
			TabuList_setSize(&tabu, tabuListSize);
            if(args->print)
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
