#include "InstanceTests.h"
#include "../src/Instance.h"
#include "../src/helpers.h"

#include <assert.h>

void InstanceTests_launchTests(void){
    Instance instance;

    Instance_init(&instance);

    custom_assert(instance.nbJobs == 0);
    custom_assert(instance.nbMachine == 0);

    custom_assert(instance.times == NULL);
    custom_assert(instance.deliveryDates == NULL);
    custom_assert(instance.distances == NULL);
    custom_assert(instance.solution == NULL);

    Instance_parseInstance(&instance);

    custom_assert(instance.nbJobs > 0);
    custom_assert(instance.nbMachine > 0);

    custom_assert(instance.times != NULL);
    custom_assert(instance.deliveryDates != NULL);
    custom_assert(instance.distances != NULL);
    custom_assert(instance.solution == NULL);
}
