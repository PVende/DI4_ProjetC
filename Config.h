//
// Created by Utilisateur on 01/10/2017.
//

#ifndef DI4_PROJETC_CONFIG_H
#define DI4_PROJETC_CONFIG_H

typedef struct {
    byte SWAP_SEQ = 1;
    byte SWAP_BATCH = 1;
    byte SWAP_BOTH = 1;
    byte EBSR_BOTH = 1;
    byte EBSR_SEQ = 1;
    byte EBSR_BATCH = 1;
    byte EFSR_BOTH = 1;
    byte EFSR_SEQ = 1;
    byte EFSR_BATCH = 1;
} Config;

void Config_parseFile(Config * cfg, char * filename);

void Config_parseLine(Config cfg, char * line)




#endif //DI4_PROJETC_CONFIG_H
