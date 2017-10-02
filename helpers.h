//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_HELPERS_H
#define DI4_PROJETC_HELPERS_H

#define MALLOC(varname, type, size) type* varname = (type*) malloc(size * sizeof(type)); \
                                    if(varname == NULL) \
                                        fatalError("Malloc failed");

#include <stdio.h>
#include <stdlib.h>

void fatalError(const char * error);

int isSpace(const char c);

#endif //DI4_PROJETC_HELPERS_H
