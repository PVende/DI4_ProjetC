#ifndef DI4_PROJETC_SOLUTION_H
#define DI4_PROJETC_SOLUTION_H

#include "Sequence.h"
#include "BatchList.h"

typedef struct{
	Sequence * sequence;
	BatchList * batchList;

} Solution;

void Solution_init(Solution * solution);
void Solution_finalize(Solution * solution);

Solution * Solution_duplicate(Solution * solution);

int Solution_equals(Solution * s1, Solution * s2);

void Solution_setSequence(Solution * solution, Sequence * sequence);
void Solution_setBatchList(Solution * solution, BatchList * batchList);

unsigned int * Solution_getBatchInfo(Solution * solution, unsigned int jobNumber);

#endif
