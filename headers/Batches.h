#ifndef BATCHES_H_INCLUDED
#define BATCHES_H_INCLUDED

typedef struct _Batches {
    unsigned int nbBatch;
    Batch * batches;

} Batches;

void Batches_init(Batches * batches);
void Batches_finalize(Batches * batches);

#endif
