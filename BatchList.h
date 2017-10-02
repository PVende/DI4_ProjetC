#ifndef BATCHLIST_H_INCLUDED
#define BATCHLIST_H_INCLUDED

#include "Batch.h"

typedef struct {
    unsigned int size;
    Batch ** batches;

} BatchList;

void BatchList_init(BatchList * list);
void BatchList_finalize(BatchList * list);

void BatchList_addBatch(BatchList * list, Batch * batch);
void BatchList_removeBatch(BatchList * list, Batch * batch);
void BatchList_removeBatch(BatchList * list, unsigned int pos);

Batch * BatchList_getBatch(BatchList * list, unsigned int pos);

void BatchList_swap(Batch * batch1, unsigned int pos1, Batch * batch2, unsigned int pos2);
void BatchList_ebsr(BatchList * list, Batch * batch, unsigned int pos);
void BatchList_efsr(BatchList * list, Batch * batch, unsigned int pos);

#endif
