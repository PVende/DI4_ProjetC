
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
    if(solution == NULL)
        return NULL;

    Solution * MALLOC(dup, Solution, 1);
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
