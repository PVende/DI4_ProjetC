
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

void Solution_writeSolution(Solution * solution, FILE * file) {
    fprintf(file, "[");

    if(solution != NULL) {
        Sequence_writeSequence(solution->sequence, file);
        fprintf(file, ", ");
        BatchList_writeBatchList(solution->batchList, file);
    } else
        fprintf(file, "null");

    fprintf(file, "]");
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
    unsigned int i, j;
    unsigned int * infos = NULL;

	if(solution != NULL && solution->batchList != NULL && solution->sequence->size > jobNumber) {
		for(i = 0; i < solution->batchList->size; i++)
            for(j = 0; j < solution->batchList->batches[i]->size; j++)
                if(solution->batchList->batches[i]->batch[j] == jobNumber) {
                    MALLOC(infos, unsigned int, 2);
                    infos[0] = i;
                    infos[1] = j;

                    return infos;
                }
	}

    return infos;
}

void Solution_swap_sequence(Solution * solution, unsigned int j1, unsigned int j2) {
    if(solution != NULL && solution->sequence != NULL)
            Sequence_swap(solution->sequence, j1, j2);
}

void Solution_ebsr_sequence(Solution * solution, unsigned int j1, unsigned int j2) {
     if(solution != NULL && solution->sequence != NULL)
            Sequence_ebsr(solution->sequence, j1, j2);
}
void Solution_efsr_sequence(Solution * solution, unsigned int j1, unsigned int j2) {
    if(solution != NULL)
        if(solution->sequence != NULL)
            Sequence_efsr(solution->sequence, j1, j2);
}

void Solution_swap_batchList(Solution * solution, unsigned int j1, unsigned int j2) {
    unsigned int * infosJ1, * infosJ2;
    if(solution != NULL && solution->batchList != NULL){
		infosJ1 = Solution_getBatchInfo(solution, solution->sequence->sequence[j1]);
		infosJ2 = Solution_getBatchInfo(solution, solution->sequence->sequence[j2]);
		if(infosJ1 != NULL && infosJ2 != NULL)
			BatchList_swap(solution->batchList, infosJ1[0], infosJ1[1], infosJ2[0], infosJ2[1]);
		free(infosJ1);
		free(infosJ2);
	}
}

void Solution_ebsr_batchList(Solution * solution, unsigned int j1, unsigned int j2) {
    unsigned int * infosJ1, * infosJ2;
    if(solution != NULL && solution->batchList != NULL){
		infosJ1 = Solution_getBatchInfo(solution, solution->sequence->sequence[j1]);
		infosJ2 = Solution_getBatchInfo(solution, solution->sequence->sequence[j2]);
		if(infosJ1 != NULL && infosJ2 != NULL)
			BatchList_ebsr(solution->batchList, infosJ1[0], infosJ1[1], infosJ2[0], infosJ2[1]);
		free(infosJ1);
		free(infosJ2);
	}
}

void Solution_efsr_batchList(Solution * solution, unsigned int j1, unsigned int j2) {
    unsigned int * infosJ1, * infosJ2;
    if(solution != NULL && solution->batchList != NULL){
		infosJ1 = Solution_getBatchInfo(solution, solution->sequence->sequence[j1]);
		infosJ2 = Solution_getBatchInfo(solution, solution->sequence->sequence[j2]);
		if(infosJ1 != NULL && infosJ2 != NULL)
			BatchList_efsr(solution->batchList, infosJ1[0], infosJ1[1], infosJ2[0], infosJ2[1]);
		free(infosJ1);
		free(infosJ2);
	}
}

void Solution_swap_both(Solution * solution, unsigned int j1, unsigned int j2) {
    Solution_swap_sequence(solution, j1, j2);
    Solution_swap_batchList(solution, j1, j2);
}

void Solution_ebsr_both(Solution * solution, unsigned int j1, unsigned int j2) {
    Solution_ebsr_sequence(solution, j1, j2);
    Solution_ebsr_batchList(solution, j1, j2);
}

void Solution_efsr_both(Solution * solution, unsigned int j1, unsigned int j2) {
    Solution_efsr_sequence(solution, j1, j2);
    Solution_efsr_batchList(solution, j1, j2);
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
