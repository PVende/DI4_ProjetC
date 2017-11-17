
#include "Solution.h"

void Solution_init(Solution * solution) {
    solution->sequence = NULL;
    solution->batchList = NULL;
}

void Solution_finalize(Solution * solution) {
    if(solution->sequence != NULL) {
        Sequence_finalize(solution->sequence);
        free(solution->sequence);
        solution->sequence = NULL;
    }

    if(solution->batchList != NULL) {
        BatchList_finalize(solution->batchList);
        free(solution->batchList);
        solution->batchList = NULL;
    }
}

Solution * Solution_duplicate(Solution * solution) {
	Solution * dup;

    if(solution == NULL)
        return NULL;

    MALLOC(dup, Solution, 1);
    dup->sequence = Sequence_duplicate(solution->sequence);
    dup->batchList = BatchList_duplicate(solution->batchList);

    return dup;
}

int Solution_equals(Solution * s1, Solution * s2) {
    if(s1 == NULL && s2 == NULL)
        return 1;
    else if(s1 == NULL || s2 == NULL)
        return 0;

    if(Sequence_equals(s1->sequence, s2->sequence) == 0 || BatchList_equals(s1->batchList, s2->batchList) == 0)
        return 0;

    return 1;
}

void Solution_setSequence(Solution * solution, Sequence * sequence) {
    if(solution != NULL) {
        if(solution->sequence != NULL) {
            Sequence_finalize(solution->sequence);
            free(solution->sequence);
            solution->sequence = NULL;
        }

        if(sequence != NULL)
            solution->sequence = Sequence_duplicate(sequence);
    }
}

void Solution_setBatchList(Solution * solution, BatchList * batchList) {
    if(solution != NULL) {
        if(solution->batchList != NULL) {
            BatchList_finalize(solution->batchList);
            free(solution->batchList);
            solution->batchList = NULL;
        }

        if(batchList != NULL)
            solution->batchList = BatchList_duplicate(batchList);
    }
}

unsigned int * Solution_getBatchInfo(Solution * solution, unsigned int jobNumber) {
    unsigned int i, totalJob = 0;
    unsigned int * infos = NULL;

	if(solution != NULL && solution->batchList != NULL && solution->sequence->size > jobNumber) {
		for(i = 0; i < solution->sequence->size && totalJob + solution->batchList->batches[i]->size < jobNumber + 1; i++)
			totalJob += solution->batchList->batches[i]->size;
		MALLOC(infos, unsigned int, 2);
		infos[0] = i;
		infos[1] = jobNumber - totalJob;
	}

    return infos;
}

void Solution_swap_sequence(Solution * solution, unsigned int j1, unsigned int j2) {
    if(solution != NULL && solution->sequence != NULL)
            Sequence_swap(solution->sequence, j1, j2);
}

void Solution_ebsr_sequence(Solution * solution, unsigned int job) {
     if(solution != NULL && solution->sequence != NULL)
            Sequence_ebsr(solution->sequence, job);
}
void Solution_efsr_sequence(Solution * solution, unsigned int job) {
    if(solution != NULL)
        if(solution->sequence != NULL)
            Sequence_efsr(solution->sequence, job);
}

void Solution_swap_batchList(Solution * solution, unsigned int j1, unsigned int j2) {
    unsigned int * infosJ1, * infosJ2;
    if(solution != NULL && solution->batchList != NULL){
		infosJ1 = Solution_getBatchInfo(solution, j1);
		infosJ2 = Solution_getBatchInfo(solution, j2);
		if(infosJ1 != NULL && infosJ2 != NULL)
			BatchList_swap(solution->batchList, infosJ1[0], infosJ1[1], infosJ2[0], infosJ2[1]);
		free(infosJ1);
		free(infosJ2);
	}
}

void Solution_ebsr_batchList(Solution * solution, unsigned int job) {
    unsigned int * infosJob;
	if(solution != NULL && solution->batchList != NULL) {
		infosJob = Solution_getBatchInfo(solution, job);
		if(infosJob != NULL)
			BatchList_ebsr(solution->batchList, infosJob[0], infosJob[1]);
		free(infosJob);
	}
}

void Solution_efsr_batchList(Solution * solution, unsigned int job) {
    unsigned int * infosJob;
	if(solution != NULL && solution->batchList != NULL) {
		infosJob = Solution_getBatchInfo(solution, job);
		if(infosJob != NULL)
			BatchList_efsr(solution->batchList, infosJob[0], infosJob[1]);
		free(infosJob);
	}
}

void Solution_swap_both(Solution * solution, unsigned int j1, unsigned int j2) {
    Solution_swap_sequence(solution, j1, j2);
    Solution_swap_batchList(solution, j1, j2);
}

void Solution_ebsr_both(Solution * solution, unsigned int job) {
    Solution_ebsr_sequence(solution, job);
    Solution_ebsr_batchList(solution, job);
}

void Solution_efsr_both(Solution * solution, unsigned int job) {
    Solution_efsr_sequence(solution, job);
    Solution_efsr_batchList(solution, job);
}

void Solution_debug(Solution * solution) {
    printf("\n"DEBUG_SEPARATOR"SOLUTION\n"DEBUG_SEPARATOR);
    if(solution != NULL) {
        Sequence_debug(solution->sequence);
        BatchList_debug(solution->batchList);
    } else
        printf("null\n");

    printf(DEBUG_SEPARATOR);
}
