//
// Created by Utilisateur on 01/10/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Config.h"
#include "helpers.h"

#define NB_CHARS_READ 256


void Config_parseFile(Config * cfg, char * filename)
{

	int eof = 0;
	FILE * file;
	char line[NB_CHARS_READ];

	file = fopen(filename, "r");

	if(file == NULL)
	{
		fatalError("Failed to open config file");
	}

	while(fgets(line, NB_CHARS_READ, file))
	{
		char * lineBreak = strchr(line, '\n');

		if(lineBreak != NULL)
			*lineBreak = '\0';

		Config_parseLine(cfg, line);
	}
}


void Config_parseLine(Config * cfg, char * line)
{
	while(isSpace(*line))
		line++;

	// if it's a comment
	if(*line == '#')
		return ;

	int intValue = 0;
	char * param, * value;
	char * equalChar = strchr(line, '=');


	if(equalChar == NULL)
		fatalError("Syntax Error: an '=' symbol is missing.");

	param = line;
	value = equalChar + 1;
	*equalChar = '\0';

	intValue = (int) strtol(value, NULL, 10); // Vaut bien 1 ou 0

	if(intValue != 0 && intValue != 1)
		fatalError("Syntax Error: the config values should either be '0' or '1'.");

	if(	strcmp(line, "SWAP_SEQ") == 0) cfg->SWAP_SEQ = intValue;
	else if(strcmp(line, "SWAP_BATCH") == 0) cfg->SWAP_BATCH = intValue;
	else if(strcmp(line, "SWAP_BOTH") == 0) cfg->SWAP_BOTH = intValue;
	else if(strcmp(line, "EBSR_SEQ") == 0) cfg->EBSR_SEQ = intValue;
	else if(strcmp(line, "EBSR_BATCH") == 0) cfg->EBSR_BATCH = intValue;
	else if(strcmp(line, "EBSR_BOTH") == 0) cfg->EBSR_BOTH = intValue;
	else if(strcmp(line, "EFSR_SEQ") == 0) cfg->EFSR_SEQ = intValue;
	else if(strcmp(line, "EFSR_BATCH") == 0) cfg->EFSR_BATCH = intValue;
	else if(strcmp(line, "EFSR_BOTH") == 0) cfg->EFSR_BOTH = intValue;
}


void Config_debug(Config * cfg)
{
	printf("===============\nCONFIG\n===============\n");
    printf("SWAP_SEQ: %d\n", cfg->SWAP_SEQ);
    printf("SWAP_BATCH: %d\n", cfg->SWAP_BATCH);
    printf("SWAP_BOTH: %d\n", cfg->SWAP_BOTH);
    printf("EBSR_BOTH: %d\n", cfg->EBSR_BOTH);
    printf("EBSR_SEQ: %d\n", cfg->EBSR_SEQ);
    printf("EBSR_BATCH: %d\n", cfg->EBSR_BATCH);
    printf("EFSR_BOTH: %d\n", cfg->EFSR_BOTH);
    printf("EFSR_SEQ: %d\n", cfg->EFSR_SEQ);
    printf("EFSR_BATCH: %d\n", cfg->EFSR_BATCH);
    printf("===============\n");
}
