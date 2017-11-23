#include "Batch.h"
#include "helpers.h"

void Batch_init(Batch * batch) {
    batch->size = 0;
    batch->batch = NULL;
}


void Batch_finalize(Batch * batch) {
    free(batch->batch);
    batch->size = 0;
    batch->batch = NULL;
}


Batch * Batch_duplicate(Batch * batch)
{
	Batch * dup;

    if(batch == NULL)
        return NULL;

    MALLOC(dup, Batch, 1);
    dup->size = batch->size;
    dup->batch = duplicateArray(batch->batch, batch->size);

    return dup;
}


int Batch_equals(Batch * b1, Batch * b2)
{
    unsigned int i;

    if(b1 == b2)
		return 1;
    else if(b1 == NULL || b2 == NULL)
        return 0;
    else if(b1->size != b2->size)
        return 0;

    for(i = 0; i < b1->size; i++)
    {
        if(b1->batch[i] != b2->batch[i])
            return 0;
    }

    return 1;
}

void Batch_writeBatch(Batch * batch, FILE * file) {
    unsigned int i;

    fprintf(file, "(");

    if(batch != NULL) {
        fprintf(file, "%d", batch->batch[0]);

        for(i = 1; i < batch->size; i++)
            fprintf(file, ", %d", batch->batch[i]);


    } else
        fprintf(file, "null");

    fprintf(file, ")");
}

void Batch_addJob(Batch * batch, unsigned int job)
{
    Batch_addJobAt(batch, job, batch->size);
}


void Batch_addJobAt(Batch * batch, unsigned int job, unsigned int position) {
    unsigned int i;

    if(position > batch->size)
        fatalError("Error argument");

    REALLOC(batch->batch, unsigned int, batch->size + 1)
	//batch->batch = realloc(batch->batch, sizeof(unsigned int) * (batch->size + 1))

    for(i = batch->size; i > position; i--)
        batch->batch[i] = batch->batch[i - 1];

    batch->batch[position] = job;

    batch->size++;

}


void Batch_removeJobAt(Batch * batch, unsigned int position) {
    if(position >= batch->size)
        fatalError("Error argument");

    unsigned int i;

    for(i = position + 1; i < batch->size; i++)
        batch->batch[i - 1] =  batch->batch[i];

    REALLOC(batch->batch, unsigned int, batch->size - 1);

    batch->size--;
}


void Batch_removeJob(Batch * batch, unsigned int job)
{
    unsigned int i;
    unsigned int size = batch->size;

    for(i = 0; i < size; i++)
    {
        if(batch->batch[i] == job)
        {
            Batch_removeJobAt(batch, i);
            break;
        }
    }
}


unsigned int Batch_extractJobAt(Batch * batch, unsigned int position)
{
    if(position >= batch->size)
        fatalError("Error argument");

    unsigned int job = batch->batch[position];
    Batch_removeJobAt(batch, position);
    return job;
}


void Batch_debug(Batch * batch)
{
    printf("%u ", batch->size);
    unsigned int i;

    for(i = 0; i < batch->size; i++)
    {
       printf("[%u]", batch->batch[i]);
    }

    printf("\n");
}
