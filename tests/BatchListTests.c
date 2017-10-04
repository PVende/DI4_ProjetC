#include "BatchListTests.h"
#include "../src/BatchList.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchListTests_launchTests(){
	BatchListTests_basicOperations();
	BatchListTests_swapEbsrEfsr();
}

void BatchListTests_basicOperations(){
	BatchList list;
	Batch b1, b2, b3;
	Batch * toRemove = &b2;

	// INIT

	BatchList_init(&list);

	custom_assert(list.size == 0);
	custom_assert(list.batches == NULL);

	prepareJobs(&b1, &b2, &b3);

	// ADD

	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	custom_assert(list.size == 3);
	custom_assert(list.batches[0] == &b1);
	custom_assert(list.batches[1] == &b2);
	custom_assert(list.batches[2] == &b3);

	// GET

	custom_assert(BatchList_getBatch(&list, 0) == &b1);
	custom_assert(BatchList_getBatch(&list, 1) == &b2);
	custom_assert(BatchList_getBatch(&list, 2) == &b3);

	// REMOVE

	BatchList_removeBatch(&list, toRemove);

	custom_assert(list.size == 2);
	custom_assert(list.batches[0] == &b1);
	custom_assert(list.batches[1] == &b3);

	// FINALIZE

	BatchList_finalize(&list);
}

void BatchListTests_swapEbsrEfsr(){

}


void prepareJobs(Batch * b1, Batch * b2, Batch * b3)
{
	Batch_init(b1);
	Batch_init(b2);
	Batch_init(b3);

	Batch_addJob(b1, 2);
	Batch_addJob(b1, 7);
	Batch_addJob(b1, 4);

	Batch_addJob(b2, 9);
	Batch_addJob(b2, 5);
	Batch_addJob(b2, 6);
	Batch_addJob(b2, 1);

	Batch_addJob(b3, 21);
	Batch_addJob(b3, 3);
}
