#include "BatchList.h"
#include "Batch.h";


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
	REALLOC(list->batches, Batch, list->size);
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
		fatalError("Error while to get a Batch from a list by its position: index out of range...");

	return list->batches[pos];
}

void BatchList_swap(Batch * batch1, unsigned int pos1, Batch * batch2, unsigned int pos2){
	if(pos1 >= batch1->size || pos2 >= batch2->size)
		fatalError("Error in BatchList_swap(): index out of range...");


	unsigned int tmp = batch1->batch[pos1];
	batch1->batch[pos1] = batch2->batch[pos2];
	batch2->batch[pos2] = tmp;
}

void BatchList_ebsr(BatchList * list, Batch * batch, unsigned int pos){

}

void BatchList_efsr(BatchList * list, Batch * batch, unsigned int pos){

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
