#include "BatchList.h"
#include "Batch.h"

void BatchList_init(BatchList * list){
	list->size = 0;
	list->batches = NULL;
}


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

void BatchList_writeBatchList(BatchList * list, FILE * file) {
    unsigned int i;

    fprintf(file, "(");

    if(list != NULL) {
        Batch_writeBatch(list->batches[0], file);

        for(i = 1; i < list->size; i++) {
            fprintf(file, ", ");
            Batch_writeBatch(list->batches[i], file);
        }

    } else
        fprintf(file, "null");

    fprintf(file, ")");
}

void BatchList_addBatch(BatchList * list, Batch * batch){
	list->size++;
	REALLOC(list->batches, Batch*, list->size);
	list->batches[list->size - 1] = Batch_duplicate(batch);
}


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


Batch * BatchList_getBatch(BatchList * list, unsigned int pos){
	if(pos >= list->size)
		return NULL;

	return list->batches[pos];
}


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


void BatchList_ebsr(BatchList * list, unsigned int batchPos1, unsigned int jobPos1, unsigned int batchPos2, unsigned int jobPos2){
	unsigned int extractedJob;
	Batch * batchInsertion, * batchExtraction;

	batchInsertion = BatchList_getBatch(list, batchPos1);
	batchExtraction = BatchList_getBatch(list, batchPos2);
	extractedJob = Batch_extractJobAt(batchExtraction, jobPos2);

	if(batchExtraction->size == 0)
	{
		// Removing the batch from the batch list
		BatchList_removeBatch(list, batchExtraction);
	}

	if(batchPos1 == batchPos2 && jobPos2 < jobPos1)
        Batch_addJobAt(batchInsertion, extractedJob, jobPos1 - 1);
    else
        Batch_addJobAt(batchInsertion, extractedJob, jobPos1);
}


void BatchList_efsr(BatchList * list, unsigned int batchPos1, unsigned int jobPos1, unsigned int batchPos2, unsigned int jobPos2){
	BatchList_ebsr(list, batchPos2, jobPos2, batchPos1, jobPos1);
}


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
