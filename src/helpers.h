//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_HELPERS_H
#define DI4_PROJETC_HELPERS_H

#define MALLOC(varname, type, size) varname = (type*) malloc((size) * sizeof(type)); \
                                    if(varname == NULL) \
                                        fatalError("The macro Malloc failed");

#define CALLOC(varname, type, size) varname = (type*) calloc(size, sizeof(type)); \
                                    if(varname == NULL && size != 0) \
                                        fatalError("The macro Calloc failed");

#define REALLOC(varname, type, size) varname = (type*) realloc(varname, (size) * sizeof(type)); \
                                     if(varname == NULL && size != 0) \
                                         fatalError("The macro Realloc failed");

#define DEBUG_SEPARATOR "============================================================\n"


#define custom_assert(expr) assert(expr); printf(".");

#include <stdio.h>
#include <stdlib.h>

void fatalError(const char * error);

int stringIsEmpty(char * str);

int areArraysEqual(unsigned int * a1, unsigned int size1, unsigned int * a2, unsigned int size2);

char * trim(char * str);

void on_sigabrt(int signum);

#endif //DI4_PROJETC_HELPERS_H
