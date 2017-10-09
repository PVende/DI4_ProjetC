#include "SequenceTests.h"
#include "../src/Sequence.h"
#include "../src/helpers.h"

#include <assert.h>

#define TO_ALLOCATE 5

#define SWAP_I 2
#define SWAP_J 4

#define EBSR 2

#define EFSR 3

void SequenceTests_launchTests(void){
	Sequence seq;
	unsigned int i;
	unsigned int toAdd[TO_ALLOCATE] = {1, 5, 3, 2, 4};

	Sequence_init(&seq);

	custom_assert(seq.allocatedSize == 0);
	custom_assert(seq.size == 0);
	custom_assert(seq.sequence == NULL);

	Sequence_allocate(&seq, TO_ALLOCATE);

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == 0);
	custom_assert(seq.sequence != NULL);

	for(i = 0; i < TO_ALLOCATE; i++)
	{
		Sequence_add(&seq, toAdd[i]);
	}

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
	custom_assert(seq.sequence != NULL);

	for(i = 0; i < TO_ALLOCATE; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}

	// SWAP

	Sequence_swap(&seq, SWAP_I, SWAP_I);

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
	custom_assert(seq.sequence[SWAP_I] == toAdd[SWAP_I]);

	Sequence_swap(&seq, SWAP_I, SWAP_J);

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
	custom_assert(seq.sequence[SWAP_I] == toAdd[SWAP_J]);
	custom_assert(seq.sequence[SWAP_J] == toAdd[SWAP_I]);

	Sequence_swap(&seq, SWAP_I, SWAP_J);

	for(i = 0; i < TO_ALLOCATE; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}


	// EBSR

	// Sequence = 1 5 3 2 4

	Sequence_ebsr(&seq, seq.size - 1);
	// => 1 5 3 2 4

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
	for(i = 0; i < TO_ALLOCATE; i++)
	{
		custom_assert(seq.sequence[i] == toAdd[i]);
	}


	Sequence_ebsr(&seq, 0);
	// => 5 3 2 4 1

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
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

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);

	custom_assert(seq.sequence[0] == 5);
	custom_assert(seq.sequence[1] == 3);
	custom_assert(seq.sequence[2] == 4);
	custom_assert(seq.sequence[3] == 1);
	custom_assert(seq.sequence[4] == 2);


	Sequence_efsr(&seq, seq.size - 1);
	// => 2 5 3 4 1

	custom_assert(seq.allocatedSize == TO_ALLOCATE);
	custom_assert(seq.size == TO_ALLOCATE);
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
