#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED

#include <stdlib.h>
#include "helpers.h"

typedef struct _Batch {
    unsigned int size;
    unsigned int * batch;

} Batch;

void Batch_init(Batch * batch);
void Batch_finalize(Batch * batch);

void Batch_addJob(Batch * batch, unsigned int job);
void Batch_addJobAt(Batch * batch, unsigned int job, unsigned int position);
void Batch_removeJob(Batch * batch, unsigned int job);
void Batch_removeJobAt(Batch * batch, unsigned int position);

void Batch_debug(Batch * batch);

#endif
