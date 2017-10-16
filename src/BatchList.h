#ifndef BATCHLIST_H_INCLUDED
#define BATCHLIST_H_INCLUDED

#include "Batch.h"

typedef struct {
    unsigned int size;
    Batch ** batches;

} BatchList;


void BatchList_init(BatchList * list);
void BatchList_finalize(BatchList * list);

BatchList * BatchList_duplicate(BatchList * list);

int BatchList_equals(BatchList * l1, BatchList * l2);

void BatchList_addBatch(BatchList * list, Batch * batch);
void BatchList_removeBatch(BatchList * list, Batch * batch);

Batch * BatchList_getBatch(BatchList * list, unsigned int pos);

void BatchList_swap(BatchList * list, unsigned int batch1Pos, unsigned int job1Pos, unsigned int batch2Pos, unsigned int job2Pos);
void BatchList_ebsr(BatchList * list, unsigned int batchPos, unsigned int jobPos);
void BatchList_efsr(BatchList * list, unsigned int batchPos, unsigned int jobPos);

void BatchList_debug(BatchList * list);

#endif
