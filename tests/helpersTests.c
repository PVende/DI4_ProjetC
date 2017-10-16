#include "helpersTests.h"
#include "../src/helpers.h"

#include <stdio.h>

#include <assert.h>
#include <string.h>
#include <errno.h>

void helpersTests_launchTests(void)
{
	helpersTests_stringIsEmpty();
	helpersTests_areArraysEqual();
	helpersTests_trim();
	helpersTests_duplicateArray();
}


void helpersTests_stringIsEmpty(void)
{
	char * empty = "";
	char * linebreak = "\n";
	char * lineBreak2 = "\r\n";
	char * spaces = "  ";
	char * multiSpaceTypes = " \t \n ";
	char * notEmpty = "abc";
	char * notEmptyRoundedBySpaces = "   abc  ";

	custom_assert(stringIsEmpty(empty) == 1);
	custom_assert(stringIsEmpty(linebreak) == 1);
	custom_assert(stringIsEmpty(lineBreak2) == 1);
	custom_assert(stringIsEmpty(spaces) == 0);
	custom_assert(stringIsEmpty(multiSpaceTypes) == 0);
	custom_assert(stringIsEmpty(notEmpty) == 0);
	custom_assert(stringIsEmpty(notEmptyRoundedBySpaces) == 0);
}

void helpersTests_areArraysEqual(void)
{
	unsigned int base[] = {1, 3, 2, 9};
	unsigned int equal[] = {1, 3, 2, 9};
	unsigned int notEqual[] = {1, 3, 456, 9};
	unsigned int smaller[] = {1, 3, 2};
	unsigned int bigger[] = {1, 3, 2, 9, 456};

	custom_assert(areArraysEqual(base, 4, base, 4) == 1);
	custom_assert(areArraysEqual(base, 3, base, 4) == 0);

	custom_assert(areArraysEqual(base, 4, equal, 4) == 1);
	custom_assert(areArraysEqual(base, 4, equal, 3) == 0);

	custom_assert(areArraysEqual(base, 4, notEqual, 4) == 0);
	custom_assert(areArraysEqual(base, 2, notEqual, 2) == 1);

	custom_assert(areArraysEqual(base, 4, smaller, 3) == 0);
	custom_assert(areArraysEqual(base, 4, bigger, 5) == 0);

	custom_assert(areArraysEqual(smaller, 3, bigger, 3) == 1);
}

void helpersTests_trim(void){
	char empty[32];
	char notEmpty[32];
	char nothingToTrim[32];

	strcpy(empty, "  \t  ");
	strcpy(notEmpty, " abc\t ");
	strcpy(nothingToTrim, "abc");

	char * resultTrimEmpty = trim(empty);
	char * resultTrimNotEmpty = trim(notEmpty);
	char * resultTrimNothingToTrim = trim(nothingToTrim);

	custom_assert(strcmp(resultTrimEmpty, "") == 0);
	custom_assert(strcmp(resultTrimNotEmpty, "abc") == 0);
	custom_assert(strcmp(resultTrimNothingToTrim, nothingToTrim) == 0);
}

void helpersTests_duplicateArray(void){
	unsigned int stackArrayOfZeros[10] = {0};
	unsigned int array[9] = {3, 4, 5, 6, 7, 8, 9, 1, 0};
	unsigned int * CALLOC(heapArrayOfZeros, unsigned int, 10);

	unsigned int * resultStackArrayOfZeros = duplicateArray(stackArrayOfZeros, 10);
	unsigned int * resultHeapArrayOfZeros = duplicateArray(heapArrayOfZeros, 10);
	unsigned int * resultSmallerStackArrayOfZeros = duplicateArray(stackArrayOfZeros, 5);
	unsigned int * resultArray = duplicateArray(array, 9);

	custom_assert(duplicateArray(NULL, 0) == NULL);
	custom_assert(areArraysEqual(stackArrayOfZeros, 10, resultStackArrayOfZeros, 10) == 1);
	custom_assert(areArraysEqual(heapArrayOfZeros, 10, resultHeapArrayOfZeros, 10) == 1);
	custom_assert(areArraysEqual(stackArrayOfZeros, 5, resultHeapArrayOfZeros, 5) == 1);
	custom_assert(areArraysEqual(array, 9, resultArray, 9));

	free(resultStackArrayOfZeros);
	free(resultSmallerStackArrayOfZeros);
	free(resultHeapArrayOfZeros);
	free(resultArray);
}
