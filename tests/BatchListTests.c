#include "BatchListTests.h"
#include "../src/BatchList.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchListTests_launchTests(void){
	BatchListTests_basicOperations();
	BatchListTests_swapEbsrEfsr();
	BatchListTests_testEbsrAndEfsrOnEmptyBatches();
	BatchListTests_testComparison();
	BatchListTests_testDuplication();
}

void BatchListTests_basicOperations(void){
	BatchList list;
	Batch b1, b2, b3;
	Batch * pb1, * pb2, * pb3;
	Batch * toRemove;

	// Initialization

	BatchList_init(&list);

	custom_assert(list.size == 0);
	custom_assert(list.batches == NULL);

	prepareJobs(&b1, &b2, &b3);

	// Adding of a batch

	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	custom_assert(list.size == 3);
	custom_assert(Batch_equals(list.batches[0], &b1));
	custom_assert(Batch_equals(list.batches[1], &b2));
	custom_assert(Batch_equals(list.batches[2], &b3));

	pb1 = list.batches[0];
	pb2 = list.batches[1];
	pb3 = list.batches[2];

	toRemove = pb2;

	// Getting batch

	custom_assert(BatchList_getBatch(&list, 0) == pb1);
	custom_assert(BatchList_getBatch(&list, 1) == pb2);
	custom_assert(BatchList_getBatch(&list, 2) == pb3);
	custom_assert(BatchList_getBatch(&list, 10) == NULL);

	// Removal of a batch

	BatchList_removeBatch(&list, toRemove);

	custom_assert(list.size == 2);
	custom_assert(list.batches[0] == pb1);
	custom_assert(list.batches[1] == pb3);

	// Finalization

	BatchList_finalize(&list);

	Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);
}

void BatchListTests_swapEbsrEfsr(void){
	BatchList list;
	Batch b1, b2, b3;
	Batch * pb1, * pb2, * pb3;

	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	pb1 = list.batches[0];
	pb2 = list.batches[1];
	pb3 = list.batches[2];

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	// -------------------------------------
	// SWAP
	// -------------------------------------

	BatchList_swap(&list, 0, 1, 1, 2);
	// BATCHES :
	// 	(0) 3->[2][6][4]
	//	(1) 4->[9][5][7][1]
	//	(2) 2->[21][3]


	custom_assert(list.size == 3);
	custom_assert(pb1->size == 3);
	custom_assert(pb1->batch[1] == 6);
	custom_assert(pb2->size == 4);
	custom_assert(pb2->batch[2] == 7);

	BatchList_swap(&list, 0, 1, 0, 2);

	// BATCHES :
	// 	(0) 3->[2][4][6]
	//	(1) 4->[9][5][7][1]
	//	(2) 2->[21][3]

	custom_assert(list.size == 3);
	custom_assert(pb1->size == 3);
	custom_assert(pb1->batch[0] == 2);
	custom_assert(pb1->batch[1] == 4);
	custom_assert(pb1->batch[2] == 6);

	// -------------------------------------
	// EBSR
	// -------------------------------------

	BatchList_ebsr(&list, 1, 1, 2, 0);

	// BATCHES :
	// 	(0) 3->[2][4][6]
	//	(1) 5->[9][21][5][7][1]
	//	(2) 1->[3]

	custom_assert(list.size == 3);

	custom_assert(pb3->size == 1);
	custom_assert(pb3->batch[0] == 3);

	custom_assert(pb2->size == 5);
	custom_assert(pb2->batch[0] == 9);
	custom_assert(pb2->batch[1] == 21);
	custom_assert(pb2->batch[2] == 5);
	custom_assert(pb2->batch[3] == 7);
	custom_assert(pb2->batch[4] == 1);


	BatchList_ebsr(&list, 0, 0, 2, 0);

	// BATCHES :
	//	(0) 4->[3][2][4][6]
	//	(1) 5->[9][21][5][7][1]

	custom_assert(list.size == 2);
	custom_assert(list.batches[0] == pb1);
	custom_assert(list.batches[1] == pb2);

	BatchList_ebsr(&list, 1, 0, 1, 4);
    BatchList_ebsr(&list, 0, 2, 0, 1);

	// BATCHES :
	//	(0) 4->[3][2][4][6]
	//	(1) 5->[1][9][21][5][7]


	custom_assert(pb1->size == 4);
	custom_assert(pb2->size == 5);
	unsigned int tab1[] = {1, 9, 21, 5, 7};
	custom_assert(areArraysEqual(pb2->batch, pb2->size, tab1, 5) == 1);

	// -------------------------------------
	// EFSR
	// -------------------------------------

    Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);

	BatchList_finalize(&list);
	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);
	pb1 = list.batches[0];
	pb2 = list.batches[1];
	pb3 = list.batches[2];

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	BatchList_efsr(&list, 2, 1, 0, 1);

	// BATCHES :
	// 	(0) 4->[2][3][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 1->[21]


	custom_assert(list.size == 3);
	custom_assert(pb1->size == 4);
	unsigned int tab2[] = {2, 3, 7, 4};
	custom_assert(areArraysEqual(pb1->batch, pb1->size, tab2, 4) == 1);
	custom_assert(pb2->size == 4);
	unsigned int tab3[] = {9, 5, 6, 1};
	custom_assert(areArraysEqual(pb2->batch, pb2->size, tab3, 4) == 1);


	BatchList_efsr(&list, 1, 2, 0, 2);
	BatchList_efsr(&list, 2, 0, 0, 2);

	// BATCHES :
	// 	(0) 6->[2][3][21][6][7][4]
	//	(1) 3->[9][5][1]

	custom_assert(list.size == 2);
	custom_assert(pb1->size == 6);
	custom_assert(pb2->size == 3);
	unsigned int tab4[] = {2, 3, 21, 6, 7, 4};
	custom_assert(areArraysEqual(pb1->batch, pb1->size, tab4, 6) == 1);

	BatchList_finalize(&list);

    Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);
}

void BatchListTests_testEbsrAndEfsrOnEmptyBatches(void)
{
	BatchList list;
	Batch b1, b2, b3;
	Batch * pb1/*, * pb2*/, * pb3;

	// -------------------------------------
	// EBSR
	// -------------------------------------

	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	pb1 = list.batches[0];
	//pb2 = list.batches[1];
	pb3 = list.batches[2];

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	BatchList_ebsr(&list, 0, 0, 1, 0);
	BatchList_ebsr(&list, 0, 0, 1, 0);
	BatchList_ebsr(&list, 2, 0, 1, 0);
	BatchList_ebsr(&list, 2, 0, 1, 0);

	// BATCHES :
	// 	(0) 5->[5][9][2][7][4]
	//	(1) 4->[1][6][21][3]

	custom_assert(list.size == 2);
	custom_assert(pb1->size == 5);

	// pb2 went empty, therefore it has been freed

	custom_assert(pb3->size == 4);
	custom_assert(BatchList_getBatch(&list, 1) == pb3);
	unsigned int tab1[] = {1, 6, 21, 3};
	custom_assert(areArraysEqual(pb3->batch, pb3->size, tab1, sizeof(tab1) / sizeof(unsigned int)));

	// -------------------------------------
	// EFSR
	// -------------------------------------

	Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);

	BatchList_finalize(&list);
	BatchList_init(&list);
	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);
	pb1 = list.batches[0];
	//pb2 = list.batches[1];
	pb3 = list.batches[2];

	// BATCHES :
	// 	(0) 3->[2][7][4]
	//	(1) 4->[9][5][6][1]
	//	(2) 2->[21][3]

	BatchList_efsr(&list, 1, 0, 0, 0);
	BatchList_efsr(&list, 1, 0, 0, 0);
	BatchList_efsr(&list, 1, 0, 0, 0);
	BatchList_efsr(&list, 1, 0, 0, 0);

	// BATCHES :
	// 	(0) 7->[1][6][5][9][2][7][4]
	//	(1) 2->[21][3]

	custom_assert(list.size == 2);
	custom_assert(pb1->size == 7);

	// pb2 went empty, therefore it has been freed

	custom_assert(pb3->size == 2);
	custom_assert(BatchList_getBatch(&list, 1) == pb3);
	unsigned int tab2[] = {1, 6, 5, 9, 2, 7, 4};
	custom_assert(areArraysEqual(pb1->batch, pb1->size, tab2, sizeof(tab2) / sizeof(unsigned int)));

	BatchList_finalize(&list);

    Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);
}

void BatchListTests_testComparison(void)
{
	BatchList original, equal, notEqual;
	Batch b1, b2, b3, b4;
	Batch * dupB1, * dupB2, * dupB3, * dupB1_2, * dupB2_2;

	// Initialization

	BatchList_init(&original);
	BatchList_init(&equal);
	BatchList_init(&notEqual);

	prepareJobs(&b1, &b2, &b3);

	Batch_init(&b4);
	Batch_addJob(&b4, 29);
	Batch_addJob(&b4, 25);
	Batch_addJob(&b4, 35);

	// Comparisons with NULL

	custom_assert(BatchList_equals(NULL, NULL) == 1);
	custom_assert(BatchList_equals(&original, NULL) == 0);
	custom_assert(BatchList_equals(NULL, &original) == 0);

	dupB1 = Batch_duplicate(&b1);
	dupB2 = Batch_duplicate(&b2);
	dupB3 = Batch_duplicate(&b3);

	dupB1_2 = Batch_duplicate(&b1);
	dupB2_2 = Batch_duplicate(&b2);

	// Comparison of two different lists

	BatchList_addBatch(&original, &b1);
	BatchList_addBatch(&original, &b2);
	BatchList_addBatch(&original, &b3);

	BatchList_addBatch(&equal, dupB1);
	BatchList_addBatch(&equal, dupB2);

	custom_assert(BatchList_equals(&original, &equal) == 0);
	custom_assert(BatchList_equals(&equal, &original) == 0);

	// Comparison of two equal lists

	BatchList_addBatch(&equal, dupB3);

	custom_assert(BatchList_equals(&original, &equal) == 1);
	custom_assert(BatchList_equals(&equal, &original) == 1);

	BatchList_addBatch(&notEqual, dupB1_2);
	BatchList_addBatch(&notEqual, dupB2_2);

	// Comparison of two list of different size

	custom_assert(BatchList_equals(&original, &notEqual) == 0);

	BatchList_addBatch(&notEqual, &b4);

	// Comparison of two list of equal size

	custom_assert(BatchList_equals(&original, &notEqual) == 0);

	// Finalization

	BatchList_finalize(&original);
	BatchList_finalize(&equal);
	BatchList_finalize(&notEqual);

    Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);
	Batch_finalize(&b4);

	Batch_finalize(dupB1);
	Batch_finalize(dupB2);
	Batch_finalize(dupB3);

	Batch_finalize(dupB1_2);
	Batch_finalize(dupB2_2);

	free(dupB1);
	free(dupB2);
	free(dupB3);

	free(dupB1_2);
	free(dupB2_2);
}

void BatchListTests_testDuplication(void)
{
	BatchList list, * dup;
	Batch b1, b2, b3;

	// Duplicate NULL

	dup = BatchList_duplicate(NULL);
	custom_assert(dup == NULL);

	// Duplicate empty batch list

	BatchList_init(&list);

	dup = BatchList_duplicate(&list);

	custom_assert(BatchList_equals(&list, dup) == 1);

	BatchList_finalize(dup);
	free(dup);

	// Classic duplication

	prepareJobs(&b1, &b2, &b3);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b2);
	BatchList_addBatch(&list, &b3);

	dup = BatchList_duplicate(&list);
	custom_assert(BatchList_equals(&list, dup));

	Batch_finalize(&b1);
	Batch_finalize(&b2);
	Batch_finalize(&b3);

	BatchList_finalize(&list);
	BatchList_finalize(dup);
	free(dup);
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
