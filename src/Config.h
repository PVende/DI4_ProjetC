//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_CONFIG_H
#define DI4_PROJETC_CONFIG_H

/** \struct The program's configuration */
typedef struct {
    int SWAP_SEQ;
    int SWAP_BATCH;
    int SWAP_BOTH;
    int EBSR_BOTH;
    int EBSR_SEQ;
    int EBSR_BATCH;
    int EFSR_BOTH;
    int EFSR_SEQ;
    int EFSR_BATCH;
} Config;

/** \brief Parse a config file and set the values of the config variable in parameter
 *
 * \param cfg Config* the config variable to set
 * \param filepath char* the path of the config file
 * \return void
 *
 */
void Config_parseFile(Config * cfg, char * filepath);

/** \brief parse a line and update the config variable if necessary
 *
 * \param cfg Config* the config variable to set
 * \param line char* the line to parse
 * \return void
 *
 */
void Config_parseLine(Config * cfg, char * line);

/** \brief Print a config to the console
 *
 * \param cfg Config* the config to print to the console
 * \return void
 *
 */
void Config_debug(Config * cfg);
#endif //DI4_PROJETC_CONFIG_H
