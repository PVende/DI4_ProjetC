#include "Batch.h"

void Batch_init(Batch * batch) {
    batch->size = 0;
    batch->batch = NULL;

}

void Batch_finalize(Batch * batch) {
    free(batch->batch);
    batch->size = 0;

}

void Batch_addJob(Batch * batch, unsigned int job, unsigned int position) {
    if(position > batch->size)
        fatalError("Error argument");

    if(batch->size == 0)
        batch->batch = malloc(sizeof(unsigned int));
    else
        batch->batch = realloc(batch->batch, sizeof(unsigned int) * (batch->size + 1));

    if(batch->batch == NULL)
        fatalError("Error malloc");

    unsigned int i;

    for(i = batch->size; i > position; i--)
        batch->batch[i] = batch->batch[i - 1];

    batch->batch[position] = job;

    batch->size++;

}

void Batch_removeJob(Batch * batch, unsigned int position) {
    if(position >= batch->size)
        fatalError("Error argument");

    unsigned int i;

    for(i = position + 1; i < batch->size; i--)
        batch->batch[i - 1] =  batch->batch[i];

    if(batch->size == 1)
        free(batch->batch);
    else
        if((batch->batch = realloc(batch->batch, sizeof(unsigned int) * (batch->size - 1))) == NULL)
           fatalError("Error malloc");

    batch->size--;
}
