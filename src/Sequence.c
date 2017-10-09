#include "Sequence.h"

void Sequence_init(Sequence * sequence) {
    sequence->allocatedSize = 0;
    sequence->size = 0;
    sequence->sequence = NULL;

}

void Sequence_finalize(Sequence * sequence) {
    free(sequence->sequence);
    sequence->allocatedSize = 0;
    sequence->size = 0;
    sequence->sequence = NULL;

}

// Allocate n * (unsigned int)
void Sequence_allocate(Sequence * sequence, unsigned int n)
{
    sequence->allocatedSize = n;
    REALLOC(sequence->sequence, unsigned int, n);
}

// add value without any reallocation
void Sequence_add(Sequence * sequence, unsigned int value)
{
    if(sequence->size == sequence->allocatedSize)
        fatalError("Not enough allocated size for the sequence");

    sequence->sequence[sequence->size] = value;
    sequence->size++;
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

void Sequence_debug(Sequence * sequence)
{
    printf("\n"DEBUG_SEPARATOR"SEQUENCE\n"DEBUG_SEPARATOR);

    printf("Allocated size: %u\n", sequence->allocatedSize);
    printf("Size: %u\n", sequence->size);

    unsigned int i;

    for(i = 0; i < sequence->size; i++)
    {
       printf("[%u]", sequence->sequence[i]);
    }

    printf("\n");
    printf(DEBUG_SEPARATOR);
}
