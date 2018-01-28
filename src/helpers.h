//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_HELPERS_H
#define DI4_PROJETC_HELPERS_H

#include <stdio.h>
#include <stdlib.h>


/** \brief allocate a pointer with malloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \param msg the message to display in case of error
 * \def #define MALLOC(varname, type, size)
 * \code
 * // Alocating a 2D array of 5 columns of integers:
 * int ** 2DArray;
 * MALLOC(2DArray, int*, 5, "error");
 * \endcode
 */
#define MALLOC_M(varname, type, size, msg) {varname = malloc(sizeof(type) * (size)); \
                                    if(varname == NULL) \
                                        fatalError(msg != NULL ? msg : "The macro Malloc failed");}


/** \brief allocate a pointer with malloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \def #define MALLOC(varname, type, size)
 * \code
 * // Alocating a 2D array of 5 columns of integers:
 * int ** 2DArray;
 * MALLOC(2DArray, int*, 5);
 * \endcode
 */
#define MALLOC(varname, type, size) MALLOC_M(varname, type, size, NULL)


/** \brief allocate a pointer with calloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \param msg the message to display in case of error
 * \def #define CALLOC(varname, type, size)
 * \code
 * // Alocating a 2D array of 5 columns of integers:
 * int ** 2DArray;
 * CALLOC_M(2DArray, int*, 5, "error");
 * \endcode
 */
#define CALLOC_M(varname, type, size, msg) {varname = calloc(size, sizeof(type)); \
                                    if(varname == NULL && size != 0) \
                                        fatalError(msg != NULL ? msg : "The macro Calloc failed");}


/** \brief allocate a pointer with calloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \param msg the message to display in case of error
 * \def #define CALLOC(varname, type, size)
 * \code
 * // Alocating a 2D array of 5 columns of integers:
 * int ** 2DArray;
 * CALLOC(2DArray, int*, 5);
 * \endcode
 */
#define CALLOC(varname, type, size) CALLOC_M(varname, type, size, NULL)


/** \brief reallocate a pointer with realloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \param msg the message to display in case of error
 * \def #define REALLOC(varname, type, size)
 * \code
 * // Realocating a 2D array of 5 columns of integers:
 * REALLOC_M(2DArray, int*, 10, NULL);
 * \endcode
 */
#define REALLOC_M(varname, type, size, msg) {varname = realloc(varname, sizeof(type) * (size)); \
                                     if(varname == NULL && size != 0) \
                                         fatalError(msg != NULL ? msg : "The macro Realloc failed");}

/** \brief reallocate a pointer with realloc() and check the value
 *
 * \param varname the name of the variable
 * \param type the type of each element
 * \param size the number of element to allocate
 * \def #define REALLOC(varname, type, size)
 * \code
 * // Realocating a 2D array of 5 columns of integers:
 * REALLOC(2DArray, int*, 10);
 * \endcode
 */
#define REALLOC(varname, type, size) REALLOC_M(varname, type, size, NULL)


/** \brief copy memory from a pointer to another and check the result value
 *
 * \param dest_varname the name of the destination variable
 * \param from_varname the name of the source variable
 * \param type the type of each element
 * \param size the number of element to copy
 * \def #define MEMCPY(dest_varname, from_varname, type, size)
 * \code
 * // Copy an array of 5 integer to another:
 * int from[5] = {1, 3, 2, 5, 4};
 * int dup[5];
 * MEMCPY(dup, from, *from, 5); 
 * MEMCPY(dup, from, int, 5);
 * \endcode
 */
#define MEMCPY(dest_varname, from_varname, type, size) MEMCPY_M(dest_varname, from_varname, type, size, NULL)

/** \brief copy memory from a pointer to another and check the result value
 *
 * \param dest_varname the name of the destination variable
 * \param from_varname the name of the source variable
 * \param type the type of each element
 * \param size the number of element to copy
 * \param msg the message to display in case of error
 * \def #define MEMCPY_m(dest_varname, from_varname, type, size, msg)
 * \code
 * // Copy an array of 5 integer to another:
 * int from[5] = {1, 3, 2, 5, 4};
 * int dup[5];
 * MEMCPY(dup, from, *from, 5, "It Failed"); 
 * MEMCPY(dup, from, int, 5, "It Failed");
 * \endcode
 */
#define MEMCPY_M(dest_varname, from_varname, type, size, msg) {dest_varname = memcpy(dest_varname, from_varname, sizeof(type) * (size)); \
							                                     if(dest_varname == NULL && size != 0) \
							                                         fatalError(msg != NULL ? msg : "The macro Memcpy failed");}


#define DEBUG_SEPARATOR "============================================================\n"

#define custom_assert(expr) assert(expr); printf(".");


/** \brief Calculate the maximum of two variables
 *
 * \param a the first variable
 * \param b the second variable
 *
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/** \brief Rand int between to integers
 *
 * \param min from
 * \param max to
 *
 */
#define RAND(min, max) min + (rand() / ((long double) RAND_MAX)) * ((max) - (min))

/** \brief generate 2 different random ints
 *
 * \param a first int
 * \param b second int
 * \param min min value
 * \param max max value
 *
 */
#define TWO_RAND_INT(a, b, min, max) { a = RAND(min, max);\
                                        do{\
                                            b = RAND(min, max);\
                                        }while(a == b); }


/** \brief Stop the program and display an error message to the output
 *
 * \param error const char* the message to display
 * \return void
 *
 */
void fatalError(const char * error);

/** \brief Figure out if a given string if empty
 *
 * \param str char* the string to analyse
 * \return int 1 if the string is empty, 0 otherwise
 * \code
 * stringIsEmpty(""); // => 1
 * stringIsEmpty("\n"); // => 1
 * stringIsEmpty("\n\r"); => 1
 * stringIsEmpty("a"); // => 0
 * \endcode
 */
int stringIsEmpty(char * str);

/** \brief Figure out if two arrays of unsigned int are strictly equals
 *
 * \param a1 unsigned int* the first array
 * \param size1 unsigned int the number of elements of the first array
 * \param a2 unsigned int* the second array
 * \param size2 unsigned int the number of elements of the second array
 * \return int 1 if they are equals, 0 otherwise
 *
 */
int areArraysEqual(unsigned int * a1, unsigned int size1, unsigned int * a2, unsigned int size2);

/** \brief Trim a string (remove left and right space characters: normal space of \r, \n, \v, \t, \f)
 *
 * \param str char* the string to trim. This string must NOT be in read only memory
 * \return char* a pointer to the trimmed string (which is int the string passed in parameter)
 * \code
 * trim("\t abc \t \n\r"); // ERROR : "abc" in read only memory
 * char * str = "\t abc \t \n\r";
 * char * trimmed = trim(str); // => "abc". Note that trimmed is a pointer to a char of str.
 * \endcode
 * \warning The parameter string might be modified (only if it has space characters at his left)
 */
char * trim(char * str);

/** \brief Catch a SIGABRT signal
 *
 * \param signum int the numero of the signal
 * \return void
 *
 */
void on_sigabrt(int signum);

/** \brief Duplicate an array of unsigned int
 *
 * \param array unsigned int* the array to duplicate
 * \param size unsigned int the number of elements to duplicate
 * \return unsigned int* the created array
 * \warning the new array is allocated on the heap
 */
unsigned int * duplicateArray(unsigned int * array, unsigned int size);


/** \brief Check if a file exist
 *
 * \param filepath const char* the path of the file
 * \return unsigned int 1 if the file exists, 0 otherwise
 *
 */
unsigned int fileExists(const char * filepath);

/** \brief Place the cursor to the next int
 *
 * \param file FILE* the file (not NULL)
 * \return void
 *
 */
void cursorOnNextInt(FILE * file);

/** \brief Place the cursor to the next int or closing brace
 *
 * \param file FILE* the file (not NULL)
 * \return char the the next int or closing brace character
 *
 */
char cursorOnNextIntOrClosingBrace(FILE * file);


#endif //DI4_PROJETC_HELPERS_H
