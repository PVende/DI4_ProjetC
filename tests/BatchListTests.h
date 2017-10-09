#ifndef BATCHLISTTESTS_H
#define BATCHLISTTESTS_H

#include "../src/BatchList.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchListTests_launchTests();

void BatchListTests_basicOperations();

void BatchListTests_swapEbsrEfsr();

void prepareJobs(Batch * b1, Batch * b2, Batch * b3);

#endif
