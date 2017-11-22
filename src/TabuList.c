#include "TabuList.h"

void TabuList_init(TabuList * list) {
    list->sizeListLimit = 0;
    list->sizeList = 0;

    list->moves = NULL;
    list->indexI = NULL;
    list->indexJ = NULL;
}

void TabuList_finalize(TabuList * list) {
    list->sizeListLimit = 0;
    list->sizeList = 0;

    free(list->moves);
    list->moves = NULL;

    free(list->indexI);
    list->indexI = NULL;

    free(list->indexJ);
    list->indexJ = NULL;
}

void TabuList_setSize(TabuList * list, unsigned int size) {
    if(list->moves == NULL && list->indexI == NULL && list->indexJ == NULL) {
        list->sizeListLimit = size;

        CALLOC(list->moves, char, size);
        CALLOC(list->indexI, unsigned int, size);
        CALLOC(list->indexJ, unsigned int, size);
    }
}

unsigned int TabuList_isTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ, unsigned int logicalTabu) {
    unsigned int i;

    for(i = 0; i < list->sizeList; i++)
        if(list->moves[i] == move &&
           ((list->indexI[i] == indexI && list->indexJ[i] == indexJ) ||
              (list->indexI[i] == indexJ && list->indexJ[i] == indexI && logicalTabu == 0)))
           return 1;

    return 0;
}

void TabuList_insertTabu(TabuList * list, char move, unsigned int indexI, unsigned int indexJ) {
    unsigned int i;

    if(list->sizeList == list->sizeListLimit)
        for(i = 1; i < list->sizeList; i++) {
            list->moves[i - 1] = list->moves[i];
            list->indexI[i - 1] = list->indexI[i];
            list->indexJ[i - 1] = list->indexJ[i];
        }
    else
        list->sizeList++;

    list->moves[list->sizeList - 1] = move;
    list->indexI[list->sizeList - 1] = indexI;
    list->indexJ[list->sizeList - 1] = indexJ;
}
