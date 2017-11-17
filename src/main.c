#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"
#include "BatchList.h"
#include "Instance.h"

#ifndef NDEBUG
	#include "../tests/TestsRunner.h"
#endif

#define CONFIG_FILENAME "configs.txt"


int main(void)
{
	signal(SIGABRT, &on_sigabrt);

	#ifndef NDEBUG

//	TestRunner_runTests();

	#endif
	// Config cfg;

	// Config_parseFile(&cfg, CONFIG_FILENAME);
	// Config_debug(&cfg);

    Instance instance;
    Instance_init(&instance);
    Instance_parseInstance(&instance, "test_files/input.txt", "cofnig.txt");
    Instance_firstSolution(&instance);

    Solution_debug(instance.solution);

    return 0;
}
