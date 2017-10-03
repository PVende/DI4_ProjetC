//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_CONFIG_H
#define DI4_PROJETC_CONFIG_H

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

void Config_parseFile(Config * cfg, char * filename);

void Config_parseLine(Config * cfg, char * line);

void Config_debug(Config * cfg);




#endif //DI4_PROJETC_CONFIG_H
