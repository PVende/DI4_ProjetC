#include "Sequence.h"

void Sequence_init(Sequence * sequence,  unsigned int sizeSequence) {
    if((sequence->sequence = malloc(sizeof(unsigned int) * sizeSequence)) == NULL)
        fatalError("Error malloc");

    sequence->sizeSequence = sizeSequence;

}

void Sequence_finalize(Sequence * sequence) {
    free(sequence->sequence);
    sequence->sizeSequence = 0;

}

void Sequence_swap(Sequence * sequence, unsigned int position1, unsigned int position2) {
    if(position1 >= sequence->sizeSequence || position2 >= sequence->sizeSequence)
        fatalError("Error argument");

    if(position1 != position2) {
        unsigned int temp = sequence->sequence[position1];
        sequence->sequence[position1] = sequence->sequence[position2];
        sequence->sequence[position2] = temp;

    }

}

void Sequence_extractBackwardReinsert(Sequence * sequence, unsigned int position) {
    if(position >= sequence->sizeSequence)
        fatalError("Error argument");

    unsigned int i, value = sequence->sequence[position];

    for(i = position + 1; i < sequence->sizeSequence; i++)
        sequence->sequence[i - 1] = sequence->sequence[i];

    sequence->sequence[sequence->sizeSequence - 1] = value;

}

void Sequence_extractForewardReinsert(Sequence * sequence, unsigned int position) {
    if(position >= sequence->sizeSequence)
        fatalError("Error argument");

    unsigned int i, value = sequence->sequence[position];

    for(i = position; i > 0; i--)
        sequence->sequence[i] = sequence->sequence[i - 1];

    sequence->sequence[0] = value;

}
