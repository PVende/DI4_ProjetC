#include "ArgsParser.h"

#include "helpers.h"
#include <string.h>

Args * Args_create()
{
    Args * args;
    MALLOC(args, Args, 1);
    strcpy(args->configFile, "");
    strcpy(args->inputFile, "");
    args->print = 1;

    return args;
}

Args * Args_build(int argc, char * argv[])
{
    int i;
    Args * args = Args_create();

    for(i = 1; i < argc; i += 2)
    {
        char * arg = argv[i];
        if(strcmp(arg, "-c") == 0 || strcmp(arg, "--config") == 0)
        {
            strcpy(args->configFile, argv[i + 1]);
        }
        else if (strcmp(arg, "-i") == 0 || strcmp(arg, "--input") == 0)
        {
            strcpy(args->inputFile, argv[i + 1]);
        }
        else if(strcmp(arg, "-dp") == 0 || strcmp(arg, "--disable-print") == 0)
        {
            args->print = 0;
            i--;
        }
        else if(strcmp(arg, "-p") == 0 || strcmp(arg, "--print") == 0)
        {
            args->print = 1;
            i--;
        }
    }

    return args;
}

void Args_destroy(Args ** args)
{
    free(*args);
    *args = NULL;
}

void Args_debug(Args * args)
{
    printf("================  ARGS  ================\n");

    printf("Config: ");
    if(*args->configFile != 0)
        printf("%s\n", args->configFile);
    else
        printf("default\n");

    printf("Input: ");
    if(*args->inputFile != 0)
        printf("%s\n", args->inputFile);
    else
        printf("default\n");

    printf("Print: %s\n", (args->print ? "yes" : "no"));

    printf("========================================\n");
}
