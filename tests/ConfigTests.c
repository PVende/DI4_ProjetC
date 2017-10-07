#include "ConfigTests.h"
#include "../src/Config.h"
#include "../src/helpers.h"

#include <stdio.h>

#include <assert.h>

#define TEST_FILENAME "../unit_test_config.txt"

void ConfigTests_launchTests(){
	Config cfg;
	FILE * file;

	file = fopen(TEST_FILENAME, "r");
	if(file == NULL)
	{
		fatalError("Failed to open a file");
	}

	fputs(file, "# Comment line\n");
	fputs(file, "SWAP_SEQ =1\n");
	fputs(file, "# Comment   \n");
	fputs(file, "SWAP_BATCH=1\n");
	fputs(file, "SWAP_BOTH=0\n\n");
	fputs(file, "EBSR_SEQ= 0\n");
	fputs(file, "	EBSR_BATCH=0\n");
	fputs(file, " EFSR_BOTH=0\n");
	fputs(file, "EFSR_SEQ=1\n\n");
	fputs(file, "EFSR_BATCH=1\n");
	fputs(file, " # This is a comment\n");

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
	custom_assert(cfg.EBSR_BOTH == 0);
	custom_assert(cfg.EBSR_SEQ == 0);
	custom_assert(cfg.EBSR_BATCH == 0);
	custom_assert(cfg.EFSR_BOTH == 0);
	custom_assert(cfg.EFSR_SEQ == 1);
	custom_assert(cfg.EFSR_BATCH == 1);

}
