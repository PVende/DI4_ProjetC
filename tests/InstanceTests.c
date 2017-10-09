#include "InstanceTests.h"
#include "../src/Instance.h"
#include "../src/helpers.h"

#include <assert.h>

void InstanceTests_launchTests(void){
    Instance instance;

    Instance_init(&instance);
    Instance_parseInstance(&instance);
}
