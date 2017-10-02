#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "helpers.h"

typedef struct _Sequence {
    unsigned int size;
    unsigned int * sequence;

} Sequence;

void Sequence_init(Sequence * sequence,  unsigned int size);
void Sequence_finalize(Sequence * sequence);

void Sequence_swap(Sequence * sequence, unsigned int i, unsigned int j);
void Sequence_ebsr(Sequence * sequence, unsigned int pos);
void Sequence_efsr(Sequence * sequence, unsigned int pos);

#endif
