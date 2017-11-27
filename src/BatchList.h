#ifndef BATCHLIST_H_INCLUDED
#define BATCHLIST_H_INCLUDED

#include "Batch.h"

/** \struct The structure that represents a list of batches */
typedef struct {
    unsigned int size;
    Batch ** batches;

} BatchList;


/** \brief initialize a batchlist
 *
 * \param list BatchList* the batchlist to initialize
 * \return void
 */
void BatchList_init(BatchList * list);

/** \brief free allocated resources for a batchlist
 *
 * \param list BatchList* the batchlist to finalize
 * \return void
 */
void BatchList_finalize(BatchList * list);

/** \brief duplicate a batchlist
 *
 * \param list BatchList* the batchlist to duplicate
 * \return BatchList* the created batchlist, allocated on the heap
 *
 */
BatchList * BatchList_duplicate(BatchList * list);

/** \brief Compare to batchlists
 *
 * \param l1 BatchList* the first batchlist
 * \param l2 BatchList* the second batchlist
 * \return int 1 if the batchlists are equal, 0 otherwise.
 * \note two batchlists are equals if they have the same size, and if all their batches are equal (using Batch_equals())
 *
 */
int BatchList_equals(BatchList * l1, BatchList * l2);

void BatchList_writeBatchList(BatchList * list, FILE * file);

/** \brief add to a batchlist
 *
 * \param list BatchList* the target list
 * \param batch Batch* the batch to add
 * \return void
 * \warning the batch added to the list is a duplicate (allocated on the heap) of the batch in parameter
 */
void BatchList_addBatch(BatchList * list, Batch * batch);

/** \brief Remove a specific batch from a batchlist
 *
 * \param list BatchList* the target batchlist
 * \param batch Batch* the batch to remove. The function does not only compare addresses, it also uses Batch_equals() to compare the batches
 * \return void
 */
void BatchList_removeBatch(BatchList * list, Batch * batch);

/** \brief get a batch from a batchlist at a specific position
 *
 * \param list BatchList* the batchlist to analyze
 * \param pos unsigned int the position of the batch to get
 * \return Batch* the batch at this position in the batchlist, or NULL if there is not batch at this position (if position >= list's size)
 *
 */
Batch * BatchList_getBatch(BatchList * list, unsigned int pos);

/** \brief Swap two jobs of two batches in a batchlist
 *
 * \param list BatchList* the batchlist to modify
 * \param batch1Pos unsigned int the position, in the batchlist, of the first batch
 * \param job1Pos unsigned int the position, in the first batch, of the first job
 * \param batch2Pos unsigned int the position, in the batchlist, of the second batch
 * \param job2Pos unsigned int the position, in the second batch, of the second job
 * \return void
 *
 */
void BatchList_swap(BatchList * list, unsigned int batch1Pos, unsigned int job1Pos, unsigned int batch2Pos, unsigned int job2Pos);

/** \brief perform an EBSR (Extraction and Backward Shifted Reinsertion) on the batchlist.
 *
 * \param list BatchList* the batchlist to modify
 * \param batchPos unsigned int the position, in the batchlist, of the batch
 * \param jobPos unsigned int the position, int the batch, of the job to extract and put in the first batch of the batchlist
 * \return void
 *
 */
void BatchList_ebsr(BatchList * list, unsigned int batchPos1, unsigned int jobPos1, unsigned int batchPos2, unsigned int jobPos2);

/** \brief perform an EFSR (Extraction and Forward Shifted Reinsertion) on the batchlist
 *
 * \param list BatchList* the batchlist to modify
 * \param batchPos unsigned int the position, in the batchlist, of the batch
 * \param jobPos unsigned int the position, int the batch, of the job to extract and put in the last batch of the batchlist
 * \return void
 *
 */
void BatchList_efsr(BatchList * list, unsigned int batchPos1, unsigned int jobPos1, unsigned int batchPos2, unsigned int jobPos2);


/** \brief Print a batchlist in the console
 *
 * \param list BatchList* the batchlist to print to the console
 * \return void
 *
 */
void BatchList_debug(BatchList * list);

#endif
