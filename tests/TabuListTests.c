#include "TabuListTests.h"

#include "../src/TabuList.h"
#include "../src/helpers.h"

#include <assert.h>

void TabuListTests_launchTests(void) {
    TabuListTests_basics();
}

void TabuListTests_basics(void) {
    unsigned int size = 3;
    TabuList list;
    TabuList_init(&list);

    custom_assert(list.sizeListLimit == 0);
    custom_assert(list.sizeList == 0);
    custom_assert(list.moves == NULL);
    custom_assert(list.indexI == NULL);
    custom_assert(list.indexJ == NULL);

    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 0) == 0);
    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 1) == 0);

    TabuList_setSize(&list, size);

    TabuList_insertTabu(&list, 's', 2, 6);

    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 0) == 1);
    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 1) == 1);

    custom_assert(TabuList_isTabu(&list, 's', 6, 2, 0) == 1);
    custom_assert(TabuList_isTabu(&list, 's', 6, 2, 1) == 0);

    TabuList_insertTabu(&list, 'b', 3, 6);
    TabuList_insertTabu(&list, 'b', 3, 2);

    custom_assert(list.sizeList == list.sizeListLimit);

    TabuList_insertTabu(&list, 'f', 1, 2);

    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 0) == 0);
    custom_assert(TabuList_isTabu(&list, 's', 2, 6, 1) == 0);

    TabuList_finalize(&list);

}
