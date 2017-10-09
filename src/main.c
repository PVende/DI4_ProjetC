#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"
#include "BatchList.h"

#ifndef NDEBUG
	#include "../tests/TestsRunner.h"
#endif

#define CONFIG_FILENAME "configs.txt"


int main(void)
{
	signal(SIGABRT, &on_sigabrt);

	#ifndef NDEBUG

	TestRunner_runTests();

	#endif
	// Config cfg;

	// Config_parseFile(&cfg, CONFIG_FILENAME);
	// Config_debug(&cfg);

    return 0;
}
