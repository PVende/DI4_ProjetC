#include "BatchList.h"
#include "Batch.h"

/** \brief initialize a batchlist
 *
 * \param list BatchList* the batchlist to initialize
 * \return void
 */
void BatchList_init(BatchList * list){
	list->size = 0;
	list->batches = NULL;
}

/** \brief free allocated resources for a batchlist
 *
 * \param list BatchList* the batchlist to finalize
 * \return void
 */
void BatchList_finalize(BatchList * list){
	unsigned int i;
	for(i = 0; i < list->size; i++){
		Batch_finalize(list->batches[i]);
		free(list->batches[i]);
	}

	free(list->batches);

	list->size = 0;
	list->batches = NULL;
}


/** \brief duplicate a batchlist
 *
 * \param list BatchList* the batchlist to duplicate
 * \return BatchList* the created batchlist, allocated on the heap
 *
 */
BatchList * BatchList_duplicate(BatchList * list)
{
    if(list == NULL)
        return NULL;

	BatchList * dup;
	unsigned int i;

	MALLOC(dup, BatchList, 1);
	BatchList_init(dup);

	if(list->size > 0)
	{
		for(i = 0; i < list->size; i++)
		{
			BatchList_addBatch(dup, list->batches[i]);
		}
	}

	return dup;
}

/** \brief Compare to batchlists
 *
 * \param l1 BatchList* the first batchlist
 * \param l2 BatchList* the second batchlist
 * \return int 1 if the batchlists are equal, 0 otherwise.
 * \note two batchlists are equals if they have the same size, and if all their batches are equal (using Batch_equals())
 * \note this function starts by checking if the addresses are equal.
 * \note if both batchlists are NULL, it returns 1.
 */
int BatchList_equals(BatchList * l1, BatchList * l2)
{
    unsigned int i;

    if(l1 == l2)
		return 1;
    else if(l1 == NULL || l2 == NULL)
        return 0;
    else if(l1->size != l2->size)
        return 0;

    for(i = 0; i < l1->size; i++)
    {
        if(!Batch_equals(l1->batches[i], l2->batches[i]))
            return 0;
    }

    return 1;
}

/** \brief add to a batchlist
 *
 * \param list BatchList* the target list
 * \param batch Batch* the batch to add
 * \return void
 * \warning the batch added to the list is a duplicate (allocated on the heap) of the batch in parameter
 */
void BatchList_addBatch(BatchList * list, Batch * batch){
	list->size++;
	REALLOC(list->batches, Batch*, list->size);
	list->batches[list->size - 1] = Batch_duplicate(batch);
}

/** \brief Remove a specific batch from a batchlist
 *
 * \param list BatchList* the target batchlist
 * \param batch Batch* the batch to remove. The function does not only compare addresses, it also uses Batch_equals() to compare the batches
 * \return void
 */
void BatchList_removeBatch(BatchList * list, Batch * batch){
	unsigned int i, j;
	for(i = 0; i < list->size; i++)
	{
		if(batch == list->batches[i] || Batch_equals(list->batches[i], batch))
		{
			Batch_finalize(list->batches[i]);
			free(list->batches[i]);

			for(j = i; j < list->size - 1; j++)
			{
				list->batches[j] = list->batches[j + 1];
			}

			list->batches[j] = NULL;
			list->size--;

			return ;
		}
	}

	fatalError("Trying to remove a Batch that is not in the list...");
}

/** \brief get a batch from a batchlist at a specific position
 *
 * \param list BatchList* the batchlist to analyze
 * \param pos unsigned int the position of the batch to get
 * \return Batch* the batch at this position in the batchlist, or NULL if there is not batch at this position (if position >= list's size)
 *
 */
Batch * BatchList_getBatch(BatchList * list, unsigned int pos){
	if(pos >= list->size)
		return NULL;

	return list->batches[pos];
}

/** \brief Swap two jobs of two batches in a batchlist
 *
 * \param list BatchList* the batchlist to modify
 * \param batch1Pos unsigned int the position, in the batchlist, of the first batch
 * \param job1Pos unsigned int the position, in the first batch, of the first job
 * \param batch2Pos unsigned int the position, in the batchlist, of the second batch
 * \param job2Pos unsigned int the position, in the second batch, of the second job
 * \return void
 *
 */
void BatchList_swap(BatchList * list, unsigned int batch1Pos, unsigned int job1Pos, unsigned int batch2Pos, unsigned int job2Pos)
{
	if(batch1Pos >= list->size || batch2Pos >= list->size)
		fatalError("Error in BatchList_swap(): index out of batch list range...");

	Batch * batch1 = list->batches[batch1Pos];
	Batch * batch2 = list->batches[batch2Pos];

	if(job1Pos >= batch1->size || job2Pos >= batch2->size)
		fatalError("Error in BatchList_swap(): index out of batch range...");


	unsigned int tmp = batch1->batch[job1Pos];
	batch1->batch[job1Pos] = batch2->batch[job2Pos];
	batch2->batch[job2Pos] = tmp;
}

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the batchlist.
 *
 * \param list BatchList* the batchlist to modify
 * \param batchPos unsigned int the position, in the batchlist, of the batch
 * \param jobPos unsigned int the position, int the batch, of the job to extract and put in the first batch of the batchlist
 * \return void
 *
 */
void BatchList_ebsr(BatchList * list, unsigned int batchPos, unsigned int jobPos){
	unsigned int extracted;
	Batch * batch, * lastBatch;

	batch = BatchList_getBatch(list, batchPos);
	extracted = Batch_extractJobAt(batch, jobPos);

	if(batch->size == 0)
	{
		// Removing the batch from the batch list
		BatchList_removeBatch(list, batch);
	}

	lastBatch = list->batches[list->size - 1];
	Batch_addJob(lastBatch, extracted);
}

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the batchlist
 *
 * \param list BatchList* the batchlist to modify
 * \param batchPos unsigned int the position, in the batchlist, of the batch
 * \param jobPos unsigned int the position, int the batch, of the job to extract and put in the last batch of the batchlist
 * \return void
 *
 */
void BatchList_efsr(BatchList * list, unsigned int batchPos, unsigned int jobPos){
	unsigned int extracted;
	Batch * batch, * firstBatch;

	batch = BatchList_getBatch(list, batchPos);
	extracted = Batch_extractJobAt(batch, jobPos);

	if(batch->size == 0)
	{
		// Removing the batch from the batch list
		BatchList_removeBatch(list, batch);
	}


	firstBatch = list->batches[0];
	Batch_addJobAt(firstBatch, extracted, 0);
}


/** \brief Print a batchlist in the console
 *
 * \param list BatchList* the batchlist to print to the console
 * \return void
 *
 */
void BatchList_debug(BatchList * list)
{
	printf("\n"DEBUG_SEPARATOR"BATCH LIST\n"DEBUG_SEPARATOR);
	printf("Size: %u\n", list->size);
	unsigned int i;
	for(i = 0; i < list->size; i++)
	{
		printf("(%u)\t->\t", i);
		Batch_debug(list->batches[i]);
	}

	printf(DEBUG_SEPARATOR);
}
