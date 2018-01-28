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

void Config_parseFile(Config * cfg, char * filepath, Args * args)
{
    if(filepath == NULL){
        cfg = NULL;
        return;
    }

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
	cfg->TWO_OPT = 0;
	cfg->DIVERSIFICATION = 0;
	cfg->FIRST_IMPROVE = 0;
	cfg->LOGICAL_TABU = 0;
	cfg->ITERATIONS = 0;
	cfg->RANDOM_DIVERSIFICATION = 0;

	if(args && args->nbIterations > 0)
        cfg->ITERATIONS = args->nbIterations;

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

	fclose(file);

	if(cfg->ITERATIONS == 0)
        cfg->ITERATIONS = 2001;
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
	else if(strcmp(line, "TWO_OPT") == 0) cfg->TWO_OPT = intValue;
	else if(strcmp(line, "DIVERSIFICATION") == 0) cfg->DIVERSIFICATION = intValue;
	else if(strcmp(line, "FIRST_IMPROVE") == 0) cfg->FIRST_IMPROVE = intValue;
	else if(strcmp(line, "LOGICAL_TABU") == 0) cfg->LOGICAL_TABU = intValue;
	else if(strcmp(line, "ITERATIONS") == 0 && cfg->ITERATIONS == 0) cfg->ITERATIONS = intValue;
	else if(strcmp(line, "RANDOM_DIVERSIFICATION") == 0) cfg->RANDOM_DIVERSIFICATION = intValue;
}

Config * Config_duplicate(Config * config) {
    Config * dup;
    MALLOC(dup, Config, 1);

    dup->SWAP_SEQ = config->SWAP_SEQ;
    dup->SWAP_BATCH = config->SWAP_BATCH;
    dup->SWAP_BOTH = config->SWAP_BOTH;
    dup->EBSR_BOTH = config->EBSR_BOTH;
    dup->EBSR_SEQ = config->EBSR_SEQ;
    dup->EBSR_BATCH = config->EBSR_BATCH;
    dup->EFSR_BOTH = config->EFSR_BOTH;
    dup->EFSR_SEQ = config->EFSR_SEQ;
    dup->EFSR_BATCH = config->EFSR_BATCH;
    dup->TWO_OPT = config->TWO_OPT;
    dup->DIVERSIFICATION = config->DIVERSIFICATION;
    dup->FIRST_IMPROVE = config->FIRST_IMPROVE;
    dup->LOGICAL_TABU = config->LOGICAL_TABU;
    dup->RANDOM_DIVERSIFICATION = config->RANDOM_DIVERSIFICATION;
    dup->ITERATIONS = config->ITERATIONS;

    return dup;
}

int Config_equals(Config * c1, Config * c2) {
    (void) c1;
    (void) c2;
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
    printf("2OPT: %d\n", cfg->TWO_OPT);
    printf("diversification: %d\n", cfg->DIVERSIFICATION);
    printf("FIRST_IMPROVE: %d\n", cfg->FIRST_IMPROVE);
    printf("LOGICAL_TABU: %d\n", cfg->LOGICAL_TABU);
    printf("ITERATIONS: %d\n", cfg->ITERATIONS);
    printf("RANDOM_DIVERSIFICATION: %d\n", cfg->RANDOM_DIVERSIFICATION);
    printf(DEBUG_SEPARATOR);
}
