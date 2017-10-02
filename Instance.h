#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include "Solution.h"

typedef struct _Instance {
    Solution * solution
	unsigned ing nbJobs;
    unsigned int nbMachine;
    unsigned int ** times;
    unsigned int ** distances;
    unsigned int * deliveryDates;

} Instance;

double Solution_eval(Solution * solution);



#endif
