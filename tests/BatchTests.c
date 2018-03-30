#include "BatchTests.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchTests_launchTests(void)
{
    distances = NULL;
	BatchTests_basics();
	BatchTests_testDuplication();
}

void BatchTests_basics(void)
{
	Batch b, equal, notEqual;

	// Initialization

	Batch_init(&b);

	custom_assert(b.size == 0);
	custom_assert(b.batch == NULL);

	// Adding a job

	Batch_addJob(&b, 2);
	Batch_addJob(&b, 7);
	Batch_addJob(&b, 4);

	custom_assert(b.size == 3);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 4);

	// Adding a job at a specific position

	Batch_addJobAt(&b, 3, 2);

	custom_assert(b.size == 4);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 3);
	custom_assert(b.batch[3] == 4);

	// Removing a job by value

	Batch_removeJob(&b, 3);

	custom_assert(b.size == 3);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 4);

	// Removing a job by position

	Batch_removeJobAt(&b, 1);

	custom_assert(b.size == 2);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 4);

	// Removing an absent job

	Batch_removeJob(&b, 3);

	custom_assert(b.size == 2);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 4);

	// Extracting a job

	unsigned int extracted = Batch_extractJobAt(&b, 1);
	custom_assert(extracted == 4);
	custom_assert(b.size == 1);
	custom_assert(b.batch[0] == 2);

	// Comparing two batches

	Batch_init(&equal);
	unsigned int i;
	for(i = 0; i < b.size; i++){
		Batch_addJob(&equal, b.batch[i]);
	}

	Batch_init(&notEqual);
	Batch_addJob(&notEqual, 2);
	Batch_addJob(&notEqual, 1);
	Batch_addJob(&notEqual, 7);

	custom_assert(Batch_equals(NULL, NULL) == 1);

	custom_assert(Batch_equals(&b, NULL) == 0);
	custom_assert(Batch_equals(NULL, &b) == 0);

	custom_assert(Batch_equals(&b, &equal) == 1);
	custom_assert(Batch_equals(&equal, &b) == 1);

	custom_assert(Batch_equals(&b, &notEqual) == 0);
	custom_assert(Batch_equals(&notEqual, &b) == 0);

	// Finalization

	Batch_finalize(&b);
	Batch_finalize(&equal);
	Batch_finalize(&notEqual);

	custom_assert(b.size == 0);
	custom_assert(b.batch == NULL);
}

void BatchTests_testDuplication(void)
{
	// Initialization

	Batch b;
	Batch_init(&b);

	// Duplication of NULL

	Batch * nullBatch = Batch_duplicate(NULL);
	custom_assert(nullBatch == NULL);

	// Duplication of an empty batch

	Batch * emptyBatch = Batch_duplicate(&b);

	custom_assert(emptyBatch->size == 0);
	custom_assert(emptyBatch->batch == NULL);

	free(emptyBatch);

	// Duplication of a not empty batch

	Batch_addJob(&b, 1);
	Batch_addJob(&b, 3);
	Batch_addJob(&b, 2);

	Batch * dup = Batch_duplicate(&b);

	custom_assert(dup->size == b.size);
	custom_assert(areArraysEqual(b.batch, b.size, dup->batch, dup->size));

	// Finalization

	Batch_finalize(&b);
	Batch_finalize(dup);
	free(dup);
}
