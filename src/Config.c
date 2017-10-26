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


/** \brief Parse a config file and set the values of the config variable in parameter
 *
 * \param cfg Config* the config variable to set
 * \param filepath char* the path of the config file
 * \return void
 *
 */
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


/** \brief parse a line and update the config variable if necessary
 *
 * \param cfg Config* the config variable to set
 * \param line char* the line to parse
 * \return void
 *
 */
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
}


/** \brief Print a config to the console
 *
 * \param cfg Config* the config to print to the console
 * \return void
 *
 */
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
    printf(DEBUG_SEPARATOR);
}
