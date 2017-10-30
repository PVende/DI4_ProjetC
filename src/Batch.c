#include "Batch.h"
#include "helpers.h"

/**
 * \brief Initialize a batch
 *
 * \param batch Batch* the batch to initialize
 * \return void
 */
void Batch_init(Batch * batch) {
    batch->size = 0;
    batch->batch = NULL;
}

/** \brief free allocated resources for a batch
 *
 * \param batch Batch* the batch to finalize
 * \return void
 */
void Batch_finalize(Batch * batch) {
    free(batch->batch);
    batch->size = 0;
    batch->batch = NULL;
}


/** \brief Duplicate a batch
 *
 * \param batch Batch* the batch to duplicate
 * \return Batch* the duplicata
 */
Batch * Batch_duplicate(Batch * batch)
{
    if(batch == NULL)
        return NULL;

    Batch * MALLOC(dup, Batch, 1);
    dup->size = batch->size;
    dup->batch = duplicateArray(batch->batch, batch->size);

    return dup;
}

/** \brief Compare two batches.
 *
 * \param b1 Batch* the first batch
 * \param b2 Batch* the second batch
 * \return int 1 if the batches are equal, 0 otherwise. Two batches are equals if they have the same size and the same jobs in the same order.
 * \note this function starts by checking if the addresses are equal.
 * \note if both batches are NULL, it returns 1.
 */
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

/** \brief Add a job to a batch
 *
 * \param batch Batch* the target batch
 * \param job unsigned int the job to add to the batch. The job should not already exist in the batch
 * \return void
 */
void Batch_addJob(Batch * batch, unsigned int job)
{
    Batch_addJobAt(batch, job, batch->size);
}

/** \brief  Add a job to a batch to a specific position
 *
 * \param batch Batch* The target batch
 * \param job unsigned int The job to add to the batch
 * \param position unsigned int The position of the new batch. This value should not be greater than the batch's size.
 * \return void
 */
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


/** \brief Remove a job of a batch at a specific position
 *
 * \param batch Batch* The batch in which the job will be removed
 * \param position unsigned int the position of the job to remove. This value should not be greater than the batch's size
 * \return void
 */
void Batch_removeJobAt(Batch * batch, unsigned int position) {
    if(position >= batch->size)
        fatalError("Error argument");

    unsigned int i;

    for(i = position + 1; i < batch->size; i++)
        batch->batch[i - 1] =  batch->batch[i];

    REALLOC(batch->batch, unsigned int, batch->size - 1);

    batch->size--;
}


/** \brief Remove a job of a batch
 *
 * \param batch Batch* The batch in which the job will be removed
 * \param position The job to remove. If the job is not in the job, nothing happens.
 * \return void
 */
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


/** \brief Extract a job of a batch
 *
 * \param batch Batch* The target batch
 * \param position unsigned int The position of the job to extract
 * \return unsigned int The extracted job.
 */
unsigned int Batch_extractJobAt(Batch * batch, unsigned int position)
{
    if(position >= batch->size)
        fatalError("Error argument");

    unsigned int job = batch->batch[position];
    Batch_removeJobAt(batch, position);
    return job;
}


/** \brief print a batch to the console
 *
 * \param batch Batch* the batch to print
 * \return void
 */
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
