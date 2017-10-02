#ifndef BATCHLIST_H_INCLUDED
#define BATCHLIST_H_INCLUDED

##include "Batch.h"

typedef struct {
    unsigned int size;
    Batch * batches;

} BatchList;

void BatchList_init(BatchList * list);
void BatchList_finalize(BatchList * list);

void BatchList_addBatch(Batch * batch);
void BatchList_removeBatch(Batch * batch);

Batch * BatchList_getBatch(unsigned int pos);

void BatchList_swap(Batch * batch1, unsigned int pos1, Batch * batch2, unsigned int pos2);
void BatchList_ebsr(Batch * batch, unsigned int pos);
void BatchList_efsr(Batch * batch, unsigned int pos);

#endif
