//
// Created by Utilisateur on 01/10/2017.
//

#include "helpers.h"
#include <string.h>
#include <ctype.h>

void fatalError(const char * error)
{
    printf("\n*** FATAL ERROR ***\n\t-> %s\n", error);
    exit(-1);
}

int stringIsEmpty(char * str)
{
	return (*str == '\0') || (*str == '\n') || (*str == '\r' && *(str + 1) == '\n');
}

int areArraysEqual(unsigned int * a1, unsigned int size1, unsigned int * a2, unsigned int size2)
{
	if(size1 != size2)
		return 0;

	unsigned int i;
	for(i = 0; i < size1; i++)
	{
		if(a1[i] != a2[i])
			return 0;
	}

	return 1;
}

char * trim(char * str)
{
	char *end;

	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)
	return str;

	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	*(end + 1) = 0;

	return str;
}

void on_sigabrt(int signum)
{
	(void) signum;
	printf("\nSIGABRT: A unit test might have failed\n");
	exit(-1);
}
