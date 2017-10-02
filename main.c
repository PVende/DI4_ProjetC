#include <stdio.h>
#include <stdlib.h>

#include "Batch.h"
#include "Sequence.h"
#include "helpers.h"
#include "Config.h"

#define CONFIG_FILENAME "configs.txt"

int main(void)
{

	Config cfg;

	Config_parseFile(&cfg, CONFIG_FILENAME);
	Config_debug(&cfg);

    return 0;
}
