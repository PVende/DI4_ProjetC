#include <stdio.h>
#include <stdlib.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"

int main(void)
{
    MALLOC(test, char, 5);

    test[0] = 'a';
    test[1] = 'b';
    test[2] = 'd';
    test[3] = 'e';
    test[4] = '\0';

    printf("%s\n", test);

    return 0;
}
