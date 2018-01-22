#ifndef DI4_PROJETC_SOLUTION_H
#define DI4_PROJETC_SOLUTION_H

#include "Sequence.h"
#include "BatchList.h"

/** \brief Represents a Solution
 */
typedef struct{
	Sequence * sequence;
	BatchList * batchList;

} Solution;

/** \brief Initialize a solution
 *
 * \param solution Solution* the solution to initialize
 * \return void
 *
 */
void Solution_init(Solution * solution);

/** \brief Free allocated resources
 *
 * \param solution Solution* the solution to finalize
 * \return void
 *
 */
void Solution_finalize(Solution * solution);


/** \brief Duplicate a solution
 *
 * \param solution Solution* the solution to duplicate
 * \return Solution* the newly created solution
 *
 */
Solution * Solution_duplicate(Solution * solution);


/** \brief Figure out if two solutions are equals, based of the values and not the addresses
 *
 * \param s1 Solution* the first solution
 * \param s2 Solution* the second solution
 * \return int 1 if both solutions are equals, 0 otherwise
 *
 */
int Solution_equals(Solution * s1, Solution * s2);

/** \brief Write a solution in a file
 *
 * \param solution Solution* the solution to write
 * \param file FILE* the target file
 * \return void
 *
 */
void Solution_writeSolution(Solution * solution, FILE * file);

/** \brief Set the sequence of an solution
 * \warning The user should always use this function to set the sequence of an solution
 *
 * \param solution Solution* the target solution
 * \param sequence Sequence* the sequence to add to the solution
 * \return void
 *
 */
void Solution_setSequence(Solution * solution, Sequence * sequence);

/** \brief Set the batchList of an solution
 * \warning The user should always use this function to set the batchList of an solution
 *
 * \param solution Solution* the target solution
 * \param batchList BatchList* the batchList to add to the solution
 * \return void
 *
 */
void Solution_setBatchList(Solution * solution, BatchList * batchList);


/** \brief Retrieve the number of batch and the position in this batch of the job
 *
 * \param solution Solution* the target solution
 * \return unsigned int the number of the job
 *
 */
unsigned int * Solution_getBatchInfo(Solution * solution, unsigned int jobNumber);


/** \brief Swap to elements in the sequence's solution
 *
 * \param solution Solution* the target solution
 * \param i unsigned int the position of the first elements
 * \param j unsigned int the position of the second elements
 * \return void
 *
 */
void Solution_swap_sequence(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the sequence's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert backward
 * \return void
 *
 */
void Solution_ebsr_sequence(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the sequence's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert forward
 * \return void
 *
 */
void Solution_efsr_sequence(Solution * solution, unsigned int j1, unsigned int j2);


/** \brief Swap to elements in the batchlist's solution
 *
 * \param solution Solution* the target solution
 * \param i unsigned int the position of the first elements
 * \param j unsigned int the position of the second elements
 * \return void
 *
 */
void Solution_swap_batchList(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the batchlist's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert backward
 * \return void
 *
 */
void Solution_ebsr_batchList(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the batchlist's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert forward
 * \return void
 *
 */
void Solution_efsr_batchList(Solution * solution, unsigned int j1, unsigned int j2);


/** \brief Swap to elements in the batchlist's solution and the sequence's solution
 *
 * \param solution Solution* the target solution
 * \param i unsigned int the position of the first elements
 * \param j unsigned int the position of the second elements
 * \return void
 *
 */
void Solution_swap_both(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the batchlist's solution and the sequence's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert backward
 * \return void
 *
 */
void Solution_ebsr_both(Solution * solution, unsigned int j1, unsigned int j2);

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the batchlist's solution and the sequence's solution.
 *
 * \param solution Solution* the target solution
 * \param pos unsigned int the position of the element to extract and reinsert forward
 * \return void
 *
 */
void Solution_efsr_both(Solution * solution, unsigned int j1, unsigned int j2);


/** \brief print a solution to the output
 *
 * \param solution Solution* the solution to debug
 * \return void
 *
 */
void Solution_debug(Solution * solution);

#endif
