//
// Created by Utilisateur on 01/10/2017.
//

#include "helpers.h"
#include <string.h>
#include <ctype.h>

void fatalError(const char * error)
{
    printf("\n*** FATAL ERROR ***\n\t-> %s\n", error);
    exit(1);
}

int stringIsEmpty(char * str)
{
	return (*str == '\0') || (*str == '\n') || (*str == '\r' && *(str + 1) == '\n');
}

int areArraysEqual(unsigned int * a1, unsigned int size1, unsigned int * a2, unsigned int size2)
{
	if(size1 != size2)
		return 0;

    return memcmp(a1, a2, sizeof(*a1) * size1) == 0;
}

char * trim(char * str)
{
	char *end;

	while(*str != 0 && isspace((unsigned char)*str)) str++;

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

unsigned int * duplicateArray(unsigned int * array, unsigned int size)
{
	if(array == NULL)
	{
		if(size == 0)
			return NULL;
		else
			fatalError("Error in duplicateArray(): array is NULL but size > 0.");
	}

	unsigned int * dup;

	MALLOC(dup, unsigned int, size);
    MEMCPY(dup, array, *array, size)

	return dup;
}

unsigned int fileExists(const char * filepath)
{
    FILE * file = fopen(filepath, "r");
    return file != NULL;
}

void cursorOnNextInt(FILE * file)
{
    if(!file)
        return;

    char c;

    do{
        c = fgetc(file);
    }while((c < '0' || c > '9') && c != EOF);

    if(fseek(file, -1, SEEK_CUR) != 0)
        fatalError("fseek failed");
}


char cursorOnNextIntOrClosingBrace(FILE * file)
{
    char c;

    do{
        c = fgetc(file);
    }while((c < '0' || c > '9') && c != ']' && c != EOF);

    if(fseek(file, -1, SEEK_CUR) != 0){
        fatalError("fseek failed");
    }

        return c;
}
