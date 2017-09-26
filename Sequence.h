#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "Helper.h"

typedef struct _Sequence {
    unsigned int sizeSequence;
    unsigned int * sequence;

} Sequence;

void Sequence_init(Sequence * sequence,  unsigned int sizeSequence);
void Sequence_finalize(Sequence * sequence);

void Sequence_swap(Sequence * sequence, unsigned int position1, unsigned int position2);
void Sequence_extractBackwardReinsert(Sequence * sequence, unsigned int position);
void Sequence_extractForewardReinsert(Sequence * sequence, unsigned int position);

#endif
