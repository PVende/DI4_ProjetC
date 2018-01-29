#include "Sequence.h"
#include "helpers.h"

unsigned int allocationStep = 5;

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

int Sequence_equals(Sequence * s1, Sequence * s2){

    if(s1 == s2)
        return 1;
    else if(s1 == NULL || s2 == NULL)
        return 0;
    else if(s1->size != s2->size)
        return 0;

    return memcmp(s1->sequence, s2->sequence, sizeof(*s1->sequence) * s1->size) == 0;

}

void Sequence_writeSequence(Sequence * sequence, FILE * file) {
    unsigned int i;

    fprintf(file, "[");

    if(sequence != NULL) {
        fprintf(file, "%d", sequence->sequence[0]);

        for(i = 1; i < sequence->size; i++)
            fprintf(file, ", %d", sequence->sequence[i]);

    } else
        fprintf(file, "null");

    fprintf(file, "]");
}

Sequence * Sequence_duplicate(Sequence * sequence){
    Sequence *dup;

    if(sequence == NULL)
        return NULL;

    MALLOC(dup, Sequence, 1);
	Sequence_init(dup);
    dup->size = sequence->size;
    Sequence_allocate(dup, sequence->size);

    MEMCPY(dup->sequence, sequence->sequence, *sequence->sequence, sequence->size)

	return dup;
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
        Sequence_allocate(sequence, sequence->allocatedSize + allocationStep);

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

void Sequence_ebsr(Sequence * sequence, unsigned int pos1, unsigned int pos2) {
    if(pos1 >= sequence->size || pos2 >= sequence->size)
        fatalError("Error argument");

    unsigned int i, value = sequence->sequence[pos2];

    if(pos1 < pos2) {
        for(i = pos2 - 1; i >= pos1 && i < pos2; i--)
            sequence->sequence[i + 1] = sequence->sequence[i];

        sequence->sequence[pos1] = value;
    } 
    else if (pos1 > pos2 + 1) {
        for(i = pos2 + 1; i < pos1; i++)
            sequence->sequence[i - 1] = sequence->sequence[i];

        sequence->sequence[pos1 - 1] = value;
    }

}

void Sequence_efsr(Sequence * sequence, unsigned int pos1, unsigned int pos2) {
    Sequence_ebsr(sequence, pos2, pos1);

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

void Sequence_setAllocationStep(unsigned int value){
    allocationStep = value;
}

unsigned int Sequence_getAllocationStep(void){
    return allocationStep;
}
