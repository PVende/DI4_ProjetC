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
	}

	free(list->batches);
}


void BatchList_addBatch(BatchList * list, Batch * batch){
	list->size++;
	REALLOC(list->batches, Batch*, list->size);
	list->batches[list->size - 1] = batch;
}

void BatchList_removeBatch(BatchList * list, Batch * batch){
	unsigned int i, j;
	for(i = 0; i < list->size; i++)
	{
		if(list->batches[i] == batch)
		{
			Batch_finalize(list->batches[i]);

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

void BatchList_ebsr(BatchList * list, unsigned int batchPos, unsigned int jobPos){

}

void BatchList_efsr(BatchList * list, unsigned int batchPos, unsigned int jobPos){

}


void BatchList_debug(BatchList * list)
{
	printf(DEBUG_SEPARATOR"BATCH LIST\n"DEBUG_SEPARATOR);
	printf("Size: %u\n", list->size);
	unsigned int i;
	for(i = 0; i < list->size; i++)
	{
		printf("(%u)\t->\t", i);
		Batch_debug(list->batches[i]);
	}

	printf(DEBUG_SEPARATOR);
}
