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
    int FLAG_2OPT;
    int DIVERSIFICATION;
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

/** \brief Duplicate a config
 *
 * \param config Config* the config to duplicate
 * \return Config* the created config, which is a copy of the parameter
 *
 */
Config * Config_duplicate(Config * config);

/** \brief Compares to configs
 *
 * \param c1 Config* the first config
 * \param c2 Config* the second config
 * \return int 1 if they are equal, 0 otherwise
 *
 */
int Config_equals(Config * c1, Config * c2);

/** \brief Print a config to the console
 *
 * \param cfg Config* the config to print to the console
 * \return void
 *
 */
void Config_debug(Config * cfg);
#endif //DI4_PROJETC_CONFIG_H
