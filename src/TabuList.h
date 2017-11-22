#ifndef TABULIST_H_INCLUDED
#define TABULIST_H_INCLUDED

#include <stdlib.h>
#include "helpers.h"

typedef struct _TabuList {
    unsigned int sizeListLimit;
    unsigned int sizeList;
    char * moves;
    unsigned int * indexI;
    unsigned int * indexJ;
} TabuList;

void TabuList_init(TabuList * list);
void TabuList_finalize(TabuList * list);

void TabuList_setSize(TabuList * list, unsigned int size);

unsigned int TabuList_isTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ, unsigned int logicalTabu);
void TabuList_insertTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ);

#endif // TABULIST_H_INCLUDED
