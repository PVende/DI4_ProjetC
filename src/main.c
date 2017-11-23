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
                timeLimit,
                cpuTime = 0,
                startTime,
                endTime;
	TabuList tabu;
	Instance instance;
	FILE * outputFile;

	TabuList_init(&tabu);
	TabuList_setSize(&tabu, tabuListSize);

	Instance_init(&instance);
	Instance_parseInstance(&instance, INPUT_FILENAME, CONFIG_FILENAME);

	timeLimit = instance.nbJobs * instance.nbMachine / 4;

	if(instance.nbJobs == 100) {
        delta = 8;
        nbMaxIterationWithoutImprovment = 5;
	}
    else if(instance.nbJobs == 50) {
        delta = 10;
        nbMaxIterationWithoutImprovment = 8;
    }
    else {
        delta = instance.nbJobs / 4;
        nbMaxIterationWithoutImprovment = 12;
    }

	srand(time(NULL));
	startTime = clock();

	Instance_firstSolution(&instance);

	while(cpuTime/CLOCKS_PER_SEC < timeLimit && nbIteration <= nbMaxIteration) {


        endTime = clock();
        cpuTime = startTime - endTime;
        nbIteration++;
	}

    if((outputFile = fopen(OUTPUT_FILENAME, "w")) == NULL)
        fatalError("error open output file");

    if((fprintf(outputFile, "%d\t", Instance_eval(&instance))) == 0)
        fatalError("error write file");

    if((fprintf(outputFile, "%d\t", cpuTime/CLOCKS_PER_SEC)) == 0)
        fatalError("error write file");

    Instance_writeInstance(&instance, outputFile);

    if(fclose(outputFile) != 0)
       fatalError("error close output file");

    return 0;
}
