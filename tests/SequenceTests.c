#include "SequenceTests.h"
#include "../src/Sequence.h"
#include "../src/helpers.h"

#include <assert.h>

#define SWAP_I 2
#define SWAP_J 4

#define EBSR 2

#define EFSR 3

void SequenceTests_launchTests(void){
	SequenceTests_basics();
	SequenceTests_comparison();
	SequenceTests_duplication();
}

void SequenceTests_basics(void)
{
	Sequence seq;
	unsigned int i;
	unsigned int toAdd[] = {1, 5, 3, 2, 4};
	unsigned int allocationStep = Sequence_getAllocationStep();
	unsigned int allocatedSizeAfterAdding = 4 + allocationStep;

	Sequence_init(&seq);

	custom_assert(seq.allocatedSize == 0);
	custom_assert(seq.size == 0);
	custom_assert(seq.sequence == NULL);

	Sequence_allocate(&seq, 4);

	custom_assert(seq.allocatedSize == 4);
	custom_assert(seq.size == 0);
	custom_assert(seq.sequence != NULL);

	for(i = 0; i < 5; i++)
	{
		Sequence_add(&seq, toAdd[i]);
	}

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size ==  4 + 1);
	custom_assert(seq.sequence != NULL);

	for(i = 0; i < 5; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}

	// SWAP

	Sequence_swap(&seq, SWAP_I, SWAP_I);

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);
	custom_assert(seq.sequence[SWAP_I] == toAdd[SWAP_I]);

	Sequence_swap(&seq, SWAP_I, SWAP_J);

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);
	custom_assert(seq.sequence[SWAP_I] == toAdd[SWAP_J]);
	custom_assert(seq.sequence[SWAP_J] == toAdd[SWAP_I]);

	Sequence_swap(&seq, SWAP_I, SWAP_J);

	for(i = 0; i < 5; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}


	// EBSR

	// Sequence = 1 5 3 2 4

	Sequence_ebsr(&seq, seq.size - 1);
	// => 1 5 3 2 4

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);
	for(i = 0; i < 5; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}


	Sequence_ebsr(&seq, 0);
	// => 5 3 2 4 1

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);
	custom_assert(seq.sequence[0] == 5);
	custom_assert(seq.sequence[1] == 3);
	custom_assert(seq.sequence[2] == 2);
	custom_assert(seq.sequence[3] == 4);
	custom_assert(seq.sequence[4] == 1);


	Sequence_ebsr(&seq, EBSR);
	// => 5 3 4 1 2

	custom_assert(seq.sequence[0] == 5);
	custom_assert(seq.sequence[1] == 3);
	custom_assert(seq.sequence[2] == 4);
	custom_assert(seq.sequence[3] == 1);
	custom_assert(seq.sequence[4] == 2);

	// EFSR


	// Sequence = 5 3 4 1 2

	Sequence_efsr(&seq, 0);
	// => 5 3 4 1 2

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);

	custom_assert(seq.sequence[0] == 5);
	custom_assert(seq.sequence[1] == 3);
	custom_assert(seq.sequence[2] == 4);
	custom_assert(seq.sequence[3] == 1);
	custom_assert(seq.sequence[4] == 2);


	Sequence_efsr(&seq, seq.size - 1);
	// => 2 5 3 4 1

	custom_assert(seq.allocatedSize == allocatedSizeAfterAdding);
	custom_assert(seq.size == 5);
	custom_assert(seq.sequence[0] == 2);
	custom_assert(seq.sequence[1] == 5);
	custom_assert(seq.sequence[2] == 3);
	custom_assert(seq.sequence[3] == 4);
	custom_assert(seq.sequence[4] == 1);


	Sequence_efsr(&seq, EFSR);
	// => 4 2 5 3 1

	custom_assert(seq.sequence[0] == 4);
	custom_assert(seq.sequence[1] == 2);
	custom_assert(seq.sequence[2] == 5);
	custom_assert(seq.sequence[3] == 3);
	custom_assert(seq.sequence[4] == 1);


	Sequence_finalize(&seq);
	custom_assert(seq.size == 0);
	custom_assert(seq.allocatedSize == 0);
	custom_assert(seq.sequence == NULL);
}

void SequenceTests_comparison(void){
	Sequence seq, equal, notEqual;

	Sequence_init(&seq);
	Sequence_init(&equal);
	Sequence_init(&notEqual);

	Sequence_allocate(&seq, 3);
	Sequence_allocate(&equal, 4);
	Sequence_allocate(&notEqual, 3);

	Sequence_add(&seq, 1);
	Sequence_add(&seq, 2);
	Sequence_add(&seq, 3);

	custom_assert(Sequence_equals(NULL, NULL) == 1);
	custom_assert(Sequence_equals(&seq, NULL) == 0);
	custom_assert(Sequence_equals(NULL, &seq) == 0);

	Sequence_add(&equal, 1);
	Sequence_add(&equal, 2);
	Sequence_add(&equal, 3);

	custom_assert(Sequence_equals(&seq, &equal) == 1);
	custom_assert(Sequence_equals(&equal, &seq) == 1);

	Sequence_add(&notEqual, 1);
	Sequence_add(&notEqual, 3);

	custom_assert(Sequence_equals(&seq, &notEqual) == 0);

	Sequence_add(&notEqual, 2);

	custom_assert(Sequence_equals(&seq, &notEqual) == 0);

	Sequence_finalize(&seq);
	Sequence_finalize(&equal);
	Sequence_finalize(&notEqual);
}

void SequenceTests_duplication(void)
{
	Sequence seq, * dup;

	Sequence_init(&seq);
	Sequence_allocate(&seq, 3);
	Sequence_add(&seq, 1);
	Sequence_add(&seq, 2);
	Sequence_add(&seq, 3);

	custom_assert(Sequence_duplicate(NULL) == NULL);

	dup = Sequence_duplicate(&seq);

	custom_assert(Sequence_equals(&seq, dup) == 1);
	custom_assert(dup->allocatedSize == seq.size);

	Sequence_finalize(&seq);
	Sequence_finalize(dup);
	free(dup);
}
