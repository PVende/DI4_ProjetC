//
// Created by Utilisateur on 01/10/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "Config.h"
#include "helpers.h"

#define NB_CHARS_TO_READ 256

void Config_parseFile(Config * cfg, char * filepath)
{
	FILE * file;
	char line[NB_CHARS_TO_READ];

	cfg->SWAP_SEQ = 0;
	cfg->SWAP_BATCH = 0;
	cfg->SWAP_BOTH = 0;
	cfg->EBSR_BOTH = 0;
	cfg->EBSR_SEQ = 0;
	cfg->EBSR_BATCH = 0;
	cfg->EFSR_BOTH = 0;
	cfg->EFSR_SEQ = 0;
	cfg->EFSR_BATCH = 0;
	cfg->FLAG_2OPT = 0;
	cfg->DIVERSIFICATION = 0;

	file = fopen(filepath, "r");

	if(file == NULL)
	{
		fatalError("Failed to open config file");
	}

	while(fgets(line, NB_CHARS_TO_READ, file))
	{
		char * lineBreak = strchr(line, '\n');

		if(lineBreak != NULL)
			*lineBreak = '\0';

		Config_parseLine(cfg, line);
	}
}

void Config_parseLine(Config * cfg, char * line)
{

	while(isspace(*line)){
		line++;
	}

	if(stringIsEmpty(line))
		return;

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

	param = trim(param);
	value = trim(value);

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
	else if(strcmp(line, "FLAG_2OPT") == 0) cfg->FLAG_2OPT = intValue;
	else if(strcmp(line, "DIVERSIFICATION") == 0) cfg->DIVERSIFICATION = intValue;
}

int Config_equals(Config * c1, Config * c2) {
    if(c1->SWAP_SEQ != c2->SWAP_SEQ)
        return 0;
    if(c1->SWAP_BATCH != c2->SWAP_BATCH)
        return 0;
    if(c1->SWAP_BOTH != c2->SWAP_BOTH)
        return 0;
    if(c1->EBSR_BOTH != c2->EBSR_BOTH)
        return 0;
    if(c1->EBSR_SEQ != c2->EBSR_SEQ)
        return 0;
    if(c1->EBSR_BATCH != c2->EBSR_BATCH)
        return 0;
    if(c1->EFSR_BOTH != c2->EFSR_BOTH)
        return 0;
    if(c1->EFSR_SEQ != c2->EFSR_SEQ)
        return 0;
    if(c1->EFSR_BATCH != c2->EFSR_BATCH)
        return 0;
    if(c1->FLAG_2OPT != c2->FLAG_2OPT)
        return 0;
    if(c1->DIVERSIFICATION != c2->DIVERSIFICATION)
        return 0;

    return 1;
}

void Config_debug(Config * cfg)
{
	printf("\n"DEBUG_SEPARATOR"CONFIG\n"DEBUG_SEPARATOR);
    printf("SWAP_SEQ: %d\n", cfg->SWAP_SEQ);
    printf("SWAP_BATCH: %d\n", cfg->SWAP_BATCH);
    printf("SWAP_BOTH: %d\n", cfg->SWAP_BOTH);
    printf("EBSR_BOTH: %d\n", cfg->EBSR_BOTH);
    printf("EBSR_SEQ: %d\n", cfg->EBSR_SEQ);
    printf("EBSR_BATCH: %d\n", cfg->EBSR_BATCH);
    printf("EFSR_BOTH: %d\n", cfg->EFSR_BOTH);
    printf("EFSR_SEQ: %d\n", cfg->EFSR_SEQ);
    printf("EFSR_BATCH: %d\n", cfg->EFSR_BATCH);
    printf("FLAG_2OPT: %d\n", cfg->FLAG_2OPT);
    printf("diversification: %d\n", cfg->DIVERSIFICATION);
    printf(DEBUG_SEPARATOR);
}
