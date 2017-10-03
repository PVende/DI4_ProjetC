#include <stdio.h>
#include <stdlib.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"
#include "BatchList.h"

#define CONFIG_FILENAME "configs.txt"

int main(void)
{

	Config cfg;

	Config_parseFile(&cfg, CONFIG_FILENAME);
	Config_debug(&cfg);

	Batch b1, b2, b3;

	Batch_init(&b1);
	Batch_init(&b2);
	Batch_init(&b3);

	Batch_addJob(&b1, 1);
	Batch_addJob(&b1, 7);
	Batch_addJob(&b1, 3);

	Batch_addJob(&b2, 8);
	Batch_addJob(&b2, 2);

	Batch_addJob(&b3, 4);
	Batch_addJob(&b3, 11);
	Batch_addJob(&b3, 9);
	Batch_addJob(&b3, 10);

	Batch_debug(&b1);
	Batch_debug(&b2);
	Batch_debug(&b3);

	BatchList list;
	BatchList_init(&list);
	BatchList_addBatch(&list, &b1);
	BatchList_addBatch(&list, &b3);
	BatchList_addBatch(&list, &b2);

	BatchList_debug(&list);

	Batch_addJobAt(&b1, 4, 0);
	Batch_addJobAt(&b1, 2, 2);
	Batch_addJob(&b1, 14);

	Batch_debug(&b1);

	Batch_removeJob(&b1, 3);

	Batch_debug(&b1);

	Batch_removeJobAt(&b1, 1);

	Batch_debug(&b1);

	BatchList_debug(&list);

    return 0;
}
