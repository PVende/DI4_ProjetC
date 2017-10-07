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
	custom_assert(BatchList_getBatch(&list, 10) == NULL);

	// REMOVE

	BatchList_removeBatch(&list, toRemove);

	custom_assert(list.size == 2);
	custom_assert(list.batches[0] == &b1);
	custom_assert(list.batches[1] == &b3);

	// FINALIZE

	BatchList_finalize(&list);
}

void BatchListTests_swapEbsrEfsr(){
	BatchList list;
	Batch b1, b2, b3;

	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	// SWAP

	BatchList_swap(&list, 0, 1, 1, 2);
	// BATCHES :
	// 	(0) 3->[2][6][4]
	//	(1) 4->[9][5][7][1]
	//	(2) 2->[21][3]


	custom_assert(list.size == 3);
	custom_assert(b1.size == 3);
	custom_assert(b1.batch[1] == 6);
	custom_assert(b2.size == 4);
	custom_assert(b2.batch[2] == 7);

	BatchList_swap(&list, 0, 1, 0, 2);

	// BATCHES :
	// 	(0) 3->[2][4][6]
	//	(1) 4->[9][5][7][1]
	//	(2) 2->[21][3]

	custom_assert(list.size == 3);
	custom_assert(b1.size == 3);
	custom_assert(b1.batch[0] == 2);
	custom_assert(b1.batch[1] == 4);
	custom_assert(b1.batch[2] == 6);

	// EBSR

	BatchList_ebsr(&list, 0, 1);

	// BATCHES :
	// 	(0) 2->[2][6]
	//	(1) 4->[9][5][7][1]
	//	(2) 3->[21][3][4]

	custom_assert(list.size == 3);

	custom_assert(b1.size == 2);
	custom_assert(b1.batch[0] == 2);
	custom_assert(b1.batch[1] == 6);

	custom_assert(b3.size == 3);
	custom_assert(b3.batch[0] == 21);
	custom_assert(b3.batch[1] == 3);
	custom_assert(b3.batch[2] == 4);


	BatchList_ebsr(&list, 0, 0);
	BatchList_ebsr(&list, 0, 0);

	// BATCHES :
	//	(0) 4->[9][5][7][1]
	//	(1) 5->[21][3][4][2][6]

	custom_assert(list.size == 2);
	custom_assert(list.batches[0] == &b2);
	custom_assert(list.batches[1] == &b3);

	BatchList_ebsr(&list, 1, 1);
	BatchList_ebsr(&list, 1, 4);

	// BATCHES :
	//	(0) 4->[9][5][7][1]
	//	(1) 5->[21][4][2][6][3]

	custom_assert(b2.size == 4);
	custom_assert(b3.size == 5);
	unsigned int tab[] = {21, 3, 4, 2, 6};
	custom_assert(areArraysEqual(b3.batch, b3.size, tab, 5) == 1);


	// EFSR

	BatchList_finalize(&list);
	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	BatchList_efsr(&list, 1, 2);

	// BATCHES :
	// 	(0) 4->[6][2][7][4]
	//	(1) 3->[9][5][1]
	//	(2) 2->[21][3]


	custom_assert(list.size == 3);
	custom_assert(b1.size == 4);
	unsigned int tab2[] = {6, 2, 7, 4};
	custom_assert(areArraysEqual(b1.batch, b1.size, tab, 4) == 1);
	custom_assert(b2.size == 3);
	unsigned int tab3[] = {9, 5, 1};
	custom_assert(areArraysEqual(b2.batch, b2.size, tab, 3) == 1);


	BatchList_efsr(&list, 2, 0);
	BatchList_efsr(&list, 2, 0);

	// BATCHES :
	// 	(0) 6->[3][21][6][2][7][4]
	//	(1) 3->[9][5][1]

	custom_assert(list.size == 2);

	BatchList_efsr(&list, 0, 2);
	BatchList_efsr(&list, 0, 0);

	// BATCHES :
	// 	(0) 6->[6][3][21][2][7][4]
	//	(1) 3->[9][5][1]


	custom_assert(b1.size == 6);
	custom_assert(b2.size == 3);
	unsigned int tab4[] = {6, 3, 21, 2, 7, 4};
	custom_assert(areArraysEqual(b1.batch, b1.size, tab, 6) == 1);


	BatchList_finalize(&list);
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
