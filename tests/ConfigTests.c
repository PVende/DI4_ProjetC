#include "ConfigTests.h"
#include "../src/Config.h"
#include "../src/helpers.h"

#include <stdio.h>

#include <assert.h>
#include <errno.h>

#define TEST_FILENAME "unit_test_config.txt"

void ConfigTests_launchTests(void){
	Config cfg;

	FILE * file = fopen(TEST_FILENAME, "w");

	if(file == NULL)
	{
		fatalError("Failed to open a file");
	}

	fputs("# Comment line\n", file);
	fputs("SWAP_SEQ =1\n", file);
	fputs("# Comment   \n", file);
	fputs("SWAP_BATCH=1\n", file);
	fputs("SWAP_BOTH=0\n\n", file);
	fputs("EBSR_SEQ= 0\n", file);
	fputs("	EBSR_BATCH=0\n", file);
	fputs(" EFSR_BOTH=0\n", file);
	fputs("EFSR_SEQ=1\n\n", file);
	fputs(" # This is a comment\n", file);
	fputs("EFSR_BATCH=1\n", file);

	/*
		cfg = {
		    int SWAP_SEQ = 1
		    int SWAP_BATCH = 1
		    int SWAP_BOTH = 0
		    int EBSR_BOTH = 1
		    int EBSR_SEQ = 0
		    int EBSR_BATCH = 0
		    int EFSR_BOTH = 0
		    int EFSR_SEQ = 1
		    int EFSR_BATCH = 1
		}
	*/
	fclose(file);

	Config_parseFile(&cfg, TEST_FILENAME);

	custom_assert(cfg.SWAP_SEQ == 1);
	custom_assert(cfg.SWAP_BATCH == 1);
	custom_assert(cfg.SWAP_BOTH == 0);
	custom_assert(cfg.EBSR_SEQ == 0);
	custom_assert(cfg.EBSR_BATCH == 0);
	custom_assert(cfg.EFSR_BOTH == 0);
	custom_assert(cfg.EFSR_SEQ == 1);
	custom_assert(cfg.EFSR_BATCH == 1);

	// not specified, should be 0
	custom_assert(cfg.EBSR_BOTH == 0);
}
