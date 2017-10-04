//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_HELPERS_H
#define DI4_PROJETC_HELPERS_H

#define MALLOC(varname, type, size) varname = (type*) malloc((size) * sizeof(type)); \
                                    if(varname == NULL) \
                                        fatalError("Malloc failed");

#define REALLOC(varname, type, size) varname = (type*) realloc(varname, (size) * sizeof(type)); \
                                     if(varname == NULL) \
                                         fatalError("Realloc failed");

#define DEBUG_SEPARATOR "============================================================\n"


#define custom_assert(expr) assert(expr); printf(".");

#include <stdio.h>
#include <stdlib.h>

void fatalError(const char * error);

int isSpace(const char c);

int stringIsEmpty(char * str);

#endif //DI4_PROJETC_HELPERS_H
