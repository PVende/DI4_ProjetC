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

void Solution_writeSolution(Solution * solution, FILE * file);

void Solution_setSequence(Solution * solution, Sequence * sequence);
void Solution_setBatchList(Solution * solution, BatchList * batchList);

unsigned int * Solution_getBatchInfo(Solution * solution, unsigned int jobNumber);

void Solution_swap_sequence(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_ebsr_sequence(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_efsr_sequence(Solution * solution, unsigned int j1, unsigned int j2);

void Solution_swap_batchList(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_ebsr_batchList(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_efsr_batchList(Solution * solution, unsigned int j1, unsigned int j2);

void Solution_swap_both(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_ebsr_both(Solution * solution, unsigned int j1, unsigned int j2);
void Solution_efsr_both(Solution * solution, unsigned int j1, unsigned int j2);

void Solution_debug(Solution * solution);

#endif
