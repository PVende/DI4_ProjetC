#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED

#include <stdlib.h>
#include "helpers.h"

/** \brief the type representing a batch. It contains an array of unsigned ints, and its size (unsigned int)
 */
typedef struct _Batch {
    unsigned int size;
    unsigned int * batch;
    unsigned int * relativeDelivery;
	unsigned int allocatedSize;
} Batch;

extern unsigned int batchAllocationStep;
extern unsigned int ** distances;

/**
 * \brief Initialize a batch
 *
 * \param batch Batch* the batch to initialize
 * \return void
 */
void Batch_init(Batch * batch);

/** \brief free allocated resources for a batch
 *
 * \param batch Batch* the batch to finalize
 * \return void
 */
void Batch_finalize(Batch * batch);

/** \brief Duplicate a batch
 *
 * \param batch Batch* the batch to duplicate
 * \return Batch* the duplicata
 */
Batch * Batch_duplicate(Batch * batch);

/** \brief Compare two batches.
 *
 * \param b1 Batch* the first batch
 * \param b2 Batch* the second batch
 * \return int 1 if the batches are equal, 0 otherwise. Two batches are equals if they have the same size and the same jobs in the same order.
 * \note this function starts by checking if the addresses are equal.
 * \note if both batches are NULL, it returns 1.
 */
int Batch_equals(Batch * b1, Batch * b2);


/** \brief Write a batch in a file
 *
 * \param batch Batch* the batch to write
 * \param file FILE* the target file
 * \return void
 *
 */
void Batch_writeBatch(Batch * batch, FILE * file);

/** \brief Add a job to a batch
 *
 * \param batch Batch* the target batch
 * \param job unsigned int the job to add to the batch. The job should not already exist in the batch
 * \return void
 */
void Batch_addJob(Batch * batch, unsigned int job);

/** \brief  Add a job to a batch to a specific position
 *
 * \param batch Batch* The target batch
 * \param job unsigned int The job to add to the batch
 * \param position unsigned int The position of the new batch. This value should not be greater than the batch's size.
 * \return void
 */
void Batch_addJobAt(Batch * batch, unsigned int job, unsigned int position);


/** \brief Remove a job of a batch at a specific position
 *
 * \param batch Batch* The batch in which the job will be removed
 * \param position unsigned int the position of the job to remove. This value should not be greater than the batch's size
 * \return void
 */
void Batch_removeJobAt(Batch * batch, unsigned int position);


/** \brief Remove a job of a batch
 *
 * \param batch Batch* The batch in which the job will be removed
 * \param position The job to remove. If the job is not in the job, nothing happens.
 * \return void
 */
void Batch_removeJob(Batch * batch, unsigned int job);


/** \brief Extract a job of a batch
 *
 * \param batch Batch* The target batch
 * \param position unsigned int The position of the job to extract
 * \return unsigned int The extracted job.
 */
unsigned int Batch_extractJobAt(Batch * batch, unsigned int position);


/** \brief print a batch to the console
 *
 * \param batch Batch* the batch to print
 * \return void
 */
void Batch_debug(Batch * batch);

#endif
