#include "BatchTests.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchTests_launchTests()
{
	Batch b;

	Batch_init(&b);

	custom_assert(b.size == 0);
	custom_assert(b.batch == NULL);

	Batch_addJob(&b, 2);
	Batch_addJob(&b, 7);
	Batch_addJob(&b, 4);

	custom_assert(b.size == 3);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 4);

	Batch_addJobAt(&b, 3, 2);

	custom_assert(b.size == 4);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 3);
	custom_assert(b.batch[3] == 4);

	Batch_removeJob(&b, 3);

	custom_assert(b.size == 3);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 7);
	custom_assert(b.batch[2] == 4);

	Batch_removeJobAt(&b, 1);

	custom_assert(b.size == 2);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 4);

	Batch_removeJob(&b, 3);

	custom_assert(b.size == 2);
	custom_assert(b.batch[0] == 2);
	custom_assert(b.batch[1] == 4);

	Batch_finalize(&b);
}
