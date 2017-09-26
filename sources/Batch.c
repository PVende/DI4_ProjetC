#include "Batch.h"

void Batch_init(Batch * batch) {
    batch->sizeBatch = 0;
    batch->batch = NULL;

}

void Batch_finalize(Batch * batch) {
    free(batch->batch);
    batch->sizeBatch = 0;

}

void Batch_insert(Batch * batch, unsigned int value, unsigned int position) {
    if(position > batch->sizeBatch)
        fatalError("Error argument");

    if(batch->sizeBatch == 0)
        batch->batch = malloc(sizeof(unsigned int));
    else
        batch->batch = realloc(batch->batch, sizeof(unsigned int) * (batch->sizeBatch + 1));

    if(batch->batch == NULL)
        fatalError("Error malloc");

    unsigned int i;

    for(i = batch->sizeBatch; i > position; i--)
        batch->batch[i] = batch->batch[i - 1];

    batch->batch[position] = value;

    batch->sizeBatch++;

}

void Batch_remove(Batch * batch, unsigned int position) {
    if(position >= batch->sizeBatch)
        fatalError("Error argument");

    unsigned int i;

    for(i = position + 1; i < batch->sizeBatch; i--)
        batch->batch[i - 1] =  batch->batch[i];

    if(batch->sizeBatch == 1)
        free(batch->batch);
    else
        if((batch->batch = realloc(batch->batch, sizeof(unsigned int) * (batch->sizeBatch - 1))) == NULL)
           fatalError("Error malloc");

    batch->sizeBatch--;
}
