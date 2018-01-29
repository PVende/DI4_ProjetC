#include "SolutionTests.h"
#include "../src/Solution.h"
#include "../src/helpers.h"

#include <assert.h>

void SolutionTests_launchTests(void)
{
    SolutionTests_testDuplication();
    SolutionTests_testBatchInfo();
    SolutionTests_testSwapEbsrEfsr();
}

void SolutionTests_testDuplication(void) {
    unsigned int i;
    Solution sol,
            * dup;
    Sequence seq;
    BatchList list;
    Batch batch;

    Solution_init(&sol);

    Sequence_init(&seq);
    Sequence_allocate(&seq, 3);
    for(i = 0; i < 3; i++)
        Sequence_add(&seq, i);
    Solution_setSequence(&sol, &seq);
    Sequence_finalize(&seq);

    Batch_init(&batch);
    Batch_addJob(&batch, 0);

    BatchList_init(&list);
    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Batch_init(&batch);
    Batch_addJob(&batch, 1);
    Batch_addJob(&batch, 2);

    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Solution_setBatchList(&sol, &list);
    BatchList_finalize(&list);

    dup = Solution_duplicate(&sol);
    custom_assert(Solution_equals(&sol, dup) == 1);

    BatchList_removeBatch(sol.batchList, sol.batchList->batches[0]);

    custom_assert(Solution_equals(&sol, dup) == 0);

    Solution_finalize(&sol);

    Solution_finalize(dup);
    free(dup);
}

void SolutionTests_testBatchInfo(void) {
    unsigned int i;
    int infoBatch1[2],
        infoBatch2[2],
        infoBatch3[2],
        infoBatch4[2];
    Solution sol;
    Sequence seq;
    BatchList list;
    Batch batch;

    Solution_init(&sol);

    Sequence_init(&seq);
    Sequence_allocate(&seq, 3);
    for(i = 0; i < 3; i++)
        Sequence_add(&seq, i);
    Solution_setSequence(&sol, &seq);
    Sequence_finalize(&seq);

    Batch_init(&batch);
    Batch_addJob(&batch, 0);

    BatchList_init(&list);
    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Batch_init(&batch);
    Batch_addJob(&batch, 1);
    Batch_addJob(&batch, 2);

    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Solution_setBatchList(&sol, &list);
    BatchList_finalize(&list);

    Solution_getBatchInfo(&sol, 0, infoBatch1);
    Solution_getBatchInfo(&sol, 1, infoBatch2);
    Solution_getBatchInfo(&sol, 2, infoBatch3);
    Solution_getBatchInfo(&sol, 3, infoBatch4);

    custom_assert(infoBatch1[0] == 0);
    custom_assert(infoBatch1[1] == 0);

    custom_assert(infoBatch2[0] == 1);
    custom_assert(infoBatch2[1] == 0);

    custom_assert(infoBatch3[0] == 1);
    custom_assert(infoBatch3[1] == 1);

    custom_assert(infoBatch4[0] == -1);

    Solution_finalize(&sol);
}

void SolutionTests_testSwapEbsrEfsr(void) {
    unsigned int i;
    Solution sol;
    Sequence seq;
    BatchList list;
    Batch batch;

    Solution_init(&sol);

    Sequence_init(&seq);
    Sequence_allocate(&seq, 3);
    for(i = 0; i < 3; i++)
        Sequence_add(&seq, i);
    Solution_setSequence(&sol, &seq);
    Sequence_finalize(&seq);

    Batch_init(&batch);
    Batch_addJob(&batch, 0);

    BatchList_init(&list);
    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Batch_init(&batch);
    Batch_addJob(&batch, 1);
    Batch_addJob(&batch, 2);

    BatchList_addBatch(&list, &batch);
    Batch_finalize(&batch);

    Solution_setBatchList(&sol, &list);
    BatchList_finalize(&list);

    Solution_swap_both(&sol, 0, 1);
    custom_assert(sol.sequence->sequence[0] == 1);
    custom_assert(sol.sequence->sequence[1] == 0);
    custom_assert(sol.batchList->batches[0]->batch[0] == 1);
    custom_assert(sol.batchList->batches[1]->batch[0] == 0);
    Solution_swap_both(&sol, 0, 1);

    Solution_efsr_batchList(&sol, 1, 0);
    custom_assert(sol.sequence->sequence[0] == 0);
    custom_assert(sol.batchList->batches[0]->batch[0] == 1);
    custom_assert(sol.batchList->batches[0]->size == 2);
    custom_assert(sol.batchList->batches[1]->size == 1);

    Solution_ebsr_sequence(&sol, 0, 2);
    custom_assert(sol.sequence->sequence[0] == 2);
    custom_assert(sol.sequence->sequence[1] == 0);
    custom_assert(sol.sequence->sequence[2] == 1);
    custom_assert(sol.sequence->size == 3);

    Solution_finalize(&sol);
}
