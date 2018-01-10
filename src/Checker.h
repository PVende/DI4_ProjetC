#ifndef CHECKER_H_INCLUDED
#define CHECKER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "Solution.h"

/** \brief Check a solution in an output file for an instance
 *
 * \param inputFilePath const char* the path of the instance file
 * \param ouputFilePath const char* the path of the output file
 * \return void
 *
 */
void Check_checkSolution(char * inputFilePath, char * ouputFilePath);

/** \brief parse a solution file to the solution variable
 *
 * \param solution Solution* the target solution
 * \param solutionFilePath char* the solution file path
 * \return void
 *
 */
void Check_parseSolution(Solution * solution, char * solutionFilePath);

#endif // CHECKER_H_INCLUDED
