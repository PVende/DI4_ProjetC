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

#endif
