#ifndef CHECKER_H_INCLUDED
#define CHECKER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"
#include "Instance.h"
#include "ArgsParser.h"

typedef struct _Checker{
    Instance * instance;
    int eval;
} Checker;

/** \brief Check a solution in an output file for an instance
 *
 * \param inputFilePath char* the path of the instance file
 * \param ouputFilePath char* the path of the output file
 * \param args Args* the command line args
 * \return void
 *
 */
void Check_checkSolution(char * inputFilePath, char * ouputFilePath, Args * args);



/** \brief parse the solution and the instance from files
 *
 * \param checker Checker* the Checker instance
 * \param inputFilePath char* the path of the instance file
 * \param ouputFilePath char* the path of the output file
 * \param args Args* the command line args
 * \return void
 *
 */
void Check_parseFile(Checker * checker, char * inputFilePath, char * ouputFilePath, Args * args);

/** \brief parse a solution file to the solution variable
 *
 * \param solution Solution* the target solution
 * \param solutionFilePath char* the solution file path
 * \return void
 *
 */
void Check_parseSolution(char * solutionFilePath, Checker * checker);

/** \brief Parse the sequence of the output file to check
 *
 * \param file FILE* the file containing the solution
 * \param checker Checker* the target checker
 * \return void
 *
 */
void Check_parseSequence(FILE * file, Checker * checker);

/** \brief Parse the batchlist
 *
 * \param file FILE* the file containing the solution
 * \param checker Checker* the target checker
 * \return void
 *
 */
void Check_parseBatchList(FILE * file, Checker * checker);

#endif // CHECKER_H_INCLUDED
