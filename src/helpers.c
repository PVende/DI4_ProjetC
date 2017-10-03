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