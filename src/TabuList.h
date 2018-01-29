#ifndef TABULIST_H_INCLUDED
#define TABULIST_H_INCLUDED

#include <stdlib.h>
#include "helpers.h"

/** \brief Represents a Tabu List
 */
typedef struct _TabuList {
    unsigned int sizeListLimit;
    unsigned int sizeList;
    char * moves;
    unsigned int * indexI;
    unsigned int * indexJ;
    unsigned int cursor;
} TabuList;

/** \brief Initialize a tabuList
 *
 * \param list TabuList* the tabuList to initialize
 * \return void
 *
 */
void TabuList_init(TabuList * list);

/** \brief Free allocated resources
 *
 * \param list TabuList* the tabuList to finalize
 * \return void
 *
 */
void TabuList_finalize(TabuList * list);

/** \brief Set the size of an tabuList
 * \warning The user should always use this function to set the size of an tabuList
 *
 * \param list TabuList* the target tabuList
 * \param size unsigned int the size of the tabuList
 * \return void
 *
 */
void TabuList_setSize(TabuList * list, unsigned int size);


/** \brief Check if a movement is tabu
 *
 * \param list TabuList* the target tabuList
 * \param move char the movement done
 * \param indexI unsigned int index of the first element of the movement
 * \param indexJ unsigned int index of the second element of the movement
 * \param logicalTabu unsigned int says if tabu is logical
 * \return unsigned int 1 if the movement is tabu else 0
 *
 */
unsigned int TabuList_isTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ, unsigned int logicalTabu);

/** \brief Check if a movement is tabu
 *  \warning the function doesn't check if the movement is tabu
 *
 * \param list TabuList* the target tabuList
 * \param move char the movement done
 * \param indexI unsigned int index of the first element of the movement
 * \param indexJ unsigned int index of the second element of the movement
 * \return void
 *
 */
void TabuList_insertTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ);

#endif // TABULIST_H_INCLUDED
