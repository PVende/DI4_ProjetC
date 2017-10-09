
#include "TestsRunner.h"
#include "../src/helpers.h"

void TestRunner_runTests()
{
	printf(DEBUG_SEPARATOR);
	printf("START OF UNIT TESTS\n");
	printf(DEBUG_SEPARATOR);

	printf("Testing Config");
	ConfigTests_launchTests();
	printf(" OK\n");

	printf("Testing Batch");
	BatchTests_launchTests();
	printf(" OK\n");

	printf("Testing BatchList");
	BatchListTests_launchTests();
	printf(" OK\n");

	printf("Testing Sequence");
	SequenceTests_launchTests();
	printf(" OK\n");

	printf("Testing Solution");
	SolutionTests_launchTests();
	printf(" OK\n");

	printf("Testing Instance");
	InstanceTests_launchTests();
	printf(" OK\n");

	printf(DEBUG_SEPARATOR);
	printf("ALL TESTS HAVE PASSED!\n");
	printf(DEBUG_SEPARATOR);
}
