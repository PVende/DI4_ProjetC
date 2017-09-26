#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED

#include <stdlib.h>
#include "Helper.h"

typedef struct _Batch {
    unsigned int sizeBatch;
    unsigned int * batch;

} Batch;

void Batch_init(Batch * batch);
void Batch_finalize(Batch * batch);

void Batch_insert(Batch * batch, unsigned int value, unsigned int position);
void Batch_remove(Batch * batch, unsigned int position);

#endif
