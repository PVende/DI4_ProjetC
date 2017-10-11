#include "InstanceTests.h"
#include "../src/Instance.h"
#include "../src/helpers.h"

#include <assert.h>

#define TEST_FILENAME "tests/unit_test_parse_instance.txt"

void InstanceTests_launchTests(void){
    Instance instance;

    Instance_init(&instance);

    custom_assert(instance.nbJobs == 0);
    custom_assert(instance.nbMachine == 0);

    custom_assert(instance.times == NULL);
    custom_assert(instance.deliveryDates == NULL);
    custom_assert(instance.distances == NULL);
    custom_assert(instance.solution == NULL);

    Instance_parseInstance(&instance, TEST_FILENAME);

    custom_assert(instance.nbJobs == 5);
    custom_assert(instance.nbMachine == 2);

    custom_assert(instance.times[0][0] == 25);
    custom_assert(instance.times[1][4] == 10);
    custom_assert(instance.deliveryDates[0] == 549);
    custom_assert(instance.deliveryDates[4] == 103);
    custom_assert(instance.distances[0][0] == 0);
    custom_assert(instance.distances[5][0] == instance.distances[0][5]);

    custom_assert(instance.solution == NULL);

    custom_assert(Instance_eval(&instance, 0) == 0);
}
