
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
