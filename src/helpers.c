//
// Created by Utilisateur on 01/10/2017.
//

#include "helpers.h"

void fatalError(const char * error)
{
    printf("*** FATAL ERROR ***\n%s\n", error);
    exit(-1);
}

int isSpace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
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
