#ifndef DI4_PROJETC_SOLUTION_H
#define DI4_PROJETC_SOLUTION_H

#include "Sequence.h"
#include "BatchList.h"

typedef struct{
	Sequence * sequence;
	BatchList * batchList;

} Solution;

double Solution_eval(Solution * solution);

Solution * Solution_createFirstSolution(void);

#endif
