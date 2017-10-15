#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "helpers.h"

extern unsigned int allocationStep;

typedef struct _Sequence {
	unsigned int allocatedSize;
    unsigned int size;
    unsigned int * sequence;

} Sequence;

void Sequence_init(Sequence * sequence);
void Sequence_finalize(Sequence * sequence);

int Sequence_equals(Sequence * s1, Sequence * s2);
Sequence * Sequence_duplicate(Sequence * sequence);


// Allocate n * (unsigned int)
void Sequence_allocate(Sequence * sequence, unsigned int n);
// add value without any reallocation, unless there isn't enough allocated memory
void Sequence_add(Sequence * sequence, unsigned int value);

void Sequence_swap(Sequence * sequence, unsigned int i, unsigned int j);
void Sequence_ebsr(Sequence * sequence, unsigned int pos);
void Sequence_efsr(Sequence * sequence, unsigned int pos);

void Sequence_debug(Sequence * sequence);

void Sequence_setAllocationStep(unsigned int value);
unsigned int Sequence_getAllocationStep(void);

#endif
