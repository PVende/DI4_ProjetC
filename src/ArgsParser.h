#ifndef ARGSPARSER_H_INCLUDED
#define ARGSPARSER_H_INCLUDED

typedef struct _Args{
    char configFile[1024];
    char inputFile[1024];
} Args;

Args * Args_create(void);

Args * Args_build(int argc, char * argv[]);

void Args_destroy(Args ** args);

void Args_debug(Args * args);

#endif // ARGSPARSER_H_INCLUDED
