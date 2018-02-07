#include "InstanceTests.h"
#include "../src/Instance.h"
#include "../src/helpers.h"

#include <assert.h>

#define TEST_FILENAME "unit_test_parse_instance.txt"
#define TEST_FILENAME_CONFIG "unit_test_config.txt"

void InstanceTests_launchTests(void){
    FILE * instanceFile, * configFile;

    instanceFile = fopen(TEST_FILENAME, "w");

    if(instanceFile == NULL){
        fatalError("Failed to open test instance file");
    }

    fputs("2 5\n25\t96\t2\t47\t36\t\n116\t3\t78\t65\t10\t\n549\t367\t20\t156\t103\t\n0\t14\t19\t36\t24\t6\t\n14\t0\t5\t8\t65\t1\t\n19\t5\t0\t63\t1\t7\t\n36\t8\t63\t0\t41\t20\t\n24\t65\t1\t41\t0\t75\t\n6\t1\t7\t20\t75\t0",
        instanceFile);

    fclose(instanceFile);

    configFile = fopen(TEST_FILENAME_CONFIG, "w");

    if(configFile == NULL){
        fatalError("Failed to open test config file");
    }

    fputs("SWAP_SEQ=1\n", configFile);
    fputs("SWAP_BATCH=1\n", configFile);
    fputs("EFSR_SEQ=1\n", configFile);
    fputs("EFSR_BATCH=1\n", configFile);

    fclose(configFile);

    InstanceTests_parserTests();
    InstanceTests_firstSolutionTests();
    InstanceTests_testDuplication();

    remove(TEST_FILENAME);
    remove(TEST_FILENAME_CONFIG);
}

void InstanceTests_parserTests(void) {
    extern unsigned int ** distances;
    Instance instance;

    Instance_init(&instance);

    custom_assert(instance.nbJobs == 0);
    custom_assert(instance.nbMachine == 0);

    custom_assert(instance.times == NULL);
    custom_assert(instance.deliveryDates == NULL);
    custom_assert(distances == NULL);
    custom_assert(instance.solution == NULL);

    Instance_parseInstance(&instance, TEST_FILENAME);

    custom_assert(instance.nbJobs == 5);
    custom_assert(instance.nbMachine == 2);

    custom_assert(instance.times[0][0] == 25);
    custom_assert(instance.times[4][1] == 10);
    custom_assert(instance.deliveryDates[0] == 549);
    custom_assert(instance.deliveryDates[4] == 103);
    custom_assert(distances[0][0] == 0);
    custom_assert(distances[5][0] == distances[0][5]);

    custom_assert(instance.solution == NULL);

    custom_assert(Instance_eval(&instance, 0) == 0);

    Instance_finalize(&instance);
}

void InstanceTests_testDuplication(void) {
    Instance instance,
            * dup;
    Instance_init(&instance);
    Instance_parseInstance(&instance, TEST_FILENAME);
    Instance_firstSolution(&instance, NULL);

    dup = Instance_duplicate(&instance);
    custom_assert(Instance_equals(dup, &instance) == 1);

    Instance_setSolution(&instance, NULL);
    custom_assert(Instance_equals(dup, &instance) == 0);

    Instance_finalize(&instance);
    Instance_finalize(dup);
    free(dup);
}

void InstanceTests_firstSolutionTests(void) {
    Instance instance;
    Instance_init(&instance);
    Instance_parseInstance(&instance, TEST_FILENAME);
    Instance_firstSolution(&instance, NULL);

    custom_assert(Instance_eval(&instance, 0) == 125);

    Instance_finalize(&instance);
}
