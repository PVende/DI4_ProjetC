#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include "Solution.h"
#include "Config.h"

#include <stdio.h>
#include <string.h>

/**
 * \brief Type representing an Instance
 */
typedef struct _Instance {
    Solution * solution;
    unsigned int nbJobs;
    unsigned int nbMachine;
    unsigned int ** times;
    unsigned int ** distances;
    unsigned int * deliveryDates;
    Config * config;

} Instance;

/** \brief Initialize an instance
 *
 * \param instance Instance* the instance to initialize
 * \return void
 *
 */
void Instance_init(Instance * instance);

/** \brief Free all allocated resources for an instance
 *
 * \param instance Instance* the instance to finalize
 * \return void
 *
 */
void Instance_finalize(Instance * instance);

/** \brief Duplicate an instance
 *
 * \param instance Instance* the instance to duplicate
 * \return Instance* the created instance, which is a copy of the parameter
 *
 */
Instance * Instance_duplicate(Instance * instance);

/** \brief Compares to instances
 *
 * \param i1 Instance* the first instance
 * \param i2 Instance* the second instance
 * \return int 1 if they are equal, 0 otherwise
 *
 */
int Instance_equals(Instance * i1, Instance * i2);

/** \brief Set the solution of an instance
 * \warning The user should always use this function to set the solution of an instance
 *
 * \param instance Instance* the target instance
 * \param solution Solution* the solution to add to the instance
 * \return void
 *
 */
void Instance_setSolution(Instance * instance, Solution * solution);

/** \brief Create an instance from a file's content
 *
 * \param instance Instance* the target instance
 * \param inputFileName char* the filename
 * \return void
 *
 */
void Instance_parseInstance(Instance * instance, char * inputFileName, char * cfgFile);

void Instance_writeInstance(Instance * instance, FILE * file);

/** \brief Objective function, used to evaluate a solution
 *
 * \param instance Instance* the instance
 * \param diversification int 1 if a diversification is wanted, 0 otherwise
 * \return unsigned int the value of the objective function for this solution
 *
 */
unsigned int Instance_eval(Instance * instance);

/** \brief Create the first solution of the program
 *
 * \param instance Instance* the instance with all required data, in which the solution will be set.
 * \return void
 *
 */
void Instance_firstSolution(Instance * instance);

#endif
