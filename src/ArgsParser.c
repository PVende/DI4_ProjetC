#include "ArgsParser.h"

#include "helpers.h"
#include <string.h>

Args * Args_create()
{
    Args * args;
    MALLOC(args, Args, 1);

    strcpy(args->configFile, "");
    strcpy(args->inputFile, "");
    strcpy(args->check, "");
    args->print = 1;
    args->help = 0;

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
        else if(strcmp(arg, "--check") == 0)
        {
            strcpy(args->check, argv[i + 1]);
        }
        else if(strcmp(arg, "-?") == 0 || strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            args->help = 1;
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
        printf("none\n");

    printf("Input: ");
    if(*args->inputFile != 0)
        printf("%s\n", args->inputFile);
    else
        printf("none\n");

    printf("Check: ");
    if(*args->check != 0)
        printf("%s\n", args->check);
    else
        printf("none\n");

    printf("Print: %s\n", (args->print ? "yes" : "no"));
    printf("Help: %s\n", (args->help ? "yes" : "no"));

    printf("========================================\n");
}

void Args_showHelp(){
    printf("\n./DI4_ProjetC [-?|-h|--help] [-i|--input <input file>] [-c|--config <config file>] [-p|-dp] [--check <output file>]\n\n");
    printf("Options : \n");
    printf("\t-?, -h, --help\t\tDisplay command help\n");
    printf("\t-i, --input <file>\tSpecify the input file (default is the file 'input.txt' at the execution context)\n");
    printf("\t-c, --config <file>\tSeccify the config file (default is the file 'config.txt' at the execution context)\n");
    printf("\t-p, --print\t\tEnable prints during research (by default)\n");
    printf("\t-dp, --disable-print\tDisable prints during research\n");
    printf("\t--check <file>\t\tCheck a solution stored in an output file\n");
}

