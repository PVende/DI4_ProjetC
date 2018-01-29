#ifndef BATCHLISTTESTS_H
#define BATCHLISTTESTS_H

#include "../src/BatchList.h"
#include "../src/Batch.h"
#include "../src/helpers.h"

#include <assert.h>

void BatchListTests_launchTests(void);

void BatchListTests_basicOperations(void);

void BatchListTests_swapEbsrEfsr(void);

void BatchListTests_testEbsrAndEfsrOnEmptyBatches(void);

void BatchListTests_testDuplication(void);

void BatchListTests_testComparison(void);

void BatchListTests_testSplit(void);

void BatchListTests_testMerge(void);

void prepareJobs(Batch * b1, Batch * b2, Batch * b3);

#endif
