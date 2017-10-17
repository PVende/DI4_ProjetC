#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include "Solution.h"

#include <stdio.h>
#include <string.h>

typedef struct _Instance {
    Solution * solution;
    unsigned int nbJobs;
    unsigned int nbMachine;
    unsigned int ** times;
    unsigned int ** distances;
    unsigned int * deliveryDates;

} Instance;

void Instance_init(Instance * instance);
void Instance_finalize(Instance * instance);

int Instance_equals(Instance * i1, Instance * i2);

void Instance_parseInstance(Instance * instance, char * inputFileName);

unsigned int Instance_eval(Instance * instance, int diversification);

#endif
