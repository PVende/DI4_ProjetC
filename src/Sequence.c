#include "Sequence.h"

void Sequence_init(Sequence * sequence,  unsigned int size) {
    if((sequence->sequence = malloc(sizeof(unsigned int) * size)) == NULL)
        fatalError("Error malloc");

    sequence->size = size;

}

void Sequence_finalize(Sequence * sequence) {
    free(sequence->sequence);
    sequence->size = 0;

}

void Sequence_swap(Sequence * sequence, unsigned int pos1, unsigned int pos2) {
    if(pos1 >= sequence->size || pos2 >= sequence->size)
        fatalError("Error argument");

    if(pos1 != pos2) {
        unsigned int temp = sequence->sequence[pos1];
        sequence->sequence[pos1] = sequence->sequence[pos2];
        sequence->sequence[pos2] = temp;

    }

}

void Sequence_ebsr(Sequence * sequence, unsigned int pos) {
    if(pos >= sequence->size)
        fatalError("Error argument");

    unsigned int i, value = sequence->sequence[pos];

    for(i = pos + 1; i < sequence->size; i++)
        sequence->sequence[i - 1] = sequence->sequence[i];

    sequence->sequence[sequence->size - 1] = value;

}

void Sequence_efsr(Sequence * sequence, unsigned int pos) {
    if(pos >= sequence->size)
        fatalError("Error argument");

    unsigned int i, value = sequence->sequence[pos];

    for(i = pos; i > 0; i--)
        sequence->sequence[i] = sequence->sequence[i - 1];

    sequence->sequence[0] = value;

}
