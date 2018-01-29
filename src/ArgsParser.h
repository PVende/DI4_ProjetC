#ifndef ARGSPARSER_H_INCLUDED
#define ARGSPARSER_H_INCLUDED


typedef struct _Args{
    char configFile[1024];
    char inputFile[1024];
    char outputFile[1024];
    int print;
    char check[1024];
    int checkDetails;
    int help;
    int debugArgs;
    int nbIterations;
    int disableTests;
} Args;

/** \brief Create the Args structure
 *
 * \return Args* the created Args
 *
 */
Args * Args_create(void);

/** \brief Build the Args structure from the command line args
 *
 * \param argc int the number of arguments
 * \param argv[] char* the command line args
 * \return Args* the created a built args
 *
 */
Args * Args_build(int argc, char * argv[]);

/** \brief Destroy an args structure
 *
 * \param args Args** The args structure à détruire
 * \return void
 *
 */
void Args_destroy(Args ** args);

/** \brief debug an Args
 *
 * \param args Args* the args to debug
 * \return void
 *
 */
void Args_debug(Args * args);


/** \brief Display command line help
 *
 * \return void
 *
 */
void Args_showHelp(void);

#endif // ARGSPARSER_H_INCLUDED
