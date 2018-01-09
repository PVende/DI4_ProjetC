#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

#include "helpers.h"

extern unsigned int allocationStep;

/** \brief Represents a Sequence
 */
typedef struct _Sequence {
	unsigned int allocatedSize;
    unsigned int size;
    unsigned int * sequence;

} Sequence;

/** \brief Initialize a sequence
 *
 * \param sequence Sequence* the sequence to initialize
 * \return void
 *
 */
void Sequence_init(Sequence * sequence);

/** \brief Free allocated resources
 *
 * \param sequence Sequence* the sequence to finalize
 * \return void
 *
 */
void Sequence_finalize(Sequence * sequence);

/** \brief Figure out if two sequences are equals, based of the values and not the addresses
 *
 * \param s1 Sequence* the first sequence
 * \param s2 Sequence* the second sequence
 * \return int 1 if both sequences are equals, 0 otherwise
 *
 */
int Sequence_equals(Sequence * s1, Sequence * s2);


/** \brief Write a sequence in a file
 *
 * \param sequence Sequence* the sequence to write
 * \param file FILE* the target file
 * \return void
 *
 */
void Sequence_writeSequence(Sequence * sequence, FILE * file);

/** \brief Duplicate a sequence
 *
 * \param sequence Sequence* the sequence to duplicate
 * \return Sequence* the newly created sequence
 *
 */
Sequence * Sequence_duplicate(Sequence * sequence);


// Allocate n * (unsigned int)
/** \brief Allocate of reallocate the sequence
 *
 * \param sequence Sequence* the sequence to reallocate
 * \param n unsigned int the number of elements
 * \return void
 *
 */
void Sequence_allocate(Sequence * sequence, unsigned int n);

/** \brief Add a value to a sequence.
 * \note if sequence->size < sequence->allocatedSize, it will do it without any reallocation because there already is enough allocated memory
 *
 * \param sequence Sequence* the target sequence
 * \param value unsigned int the value to add to the sequence
 * \return void
 *
 */
void Sequence_add(Sequence * sequence, unsigned int value);

/** \brief Swap to elements in a sequence
 *
 * \param sequence Sequence* the target sequence
 * \param i unsigned int the position of the first elements
 * \param j unsigned int the position of the second elements
 * \return void
 *
 */
void Sequence_swap(Sequence * sequence, unsigned int i, unsigned int j);

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the sequence.
 *
 * \param sequence Sequence* the target sequence
 * \param pos unsigned int the position of the element to extract and reinsert backward
 * \return void
 *
 */
void Sequence_ebsr(Sequence * sequence, unsigned int pos1, unsigned int pos2);

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the sequence.
 *
 * \param sequence Sequence* the target sequence
 * \param pos unsigned int the position of the element to extract and reinsert forward
 * \return void
 *
 */
void Sequence_efsr(Sequence * sequence, unsigned int pos1, unsigned int pos2);

/** \brief print a sequence to the output
 *
 * \param sequence Sequence* the sequence to debug
 * \return void
 *
 */
void Sequence_debug(Sequence * sequence);

/** \brief Modify the value of allocationStep
 *
 * \param value unsigned int the new value of allocationStep
 * \return void
 *
 */
void Sequence_setAllocationStep(unsigned int value);


/** \brief Retrieve the value of allocationStep
 *
 * \param void
 * \return unsigned int the value of allocationStep
 *
 */
unsigned int Sequence_getAllocationStep(void);

#endif
