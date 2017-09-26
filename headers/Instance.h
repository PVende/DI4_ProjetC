#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

typedef struct _Instance {
    Sequence sequence;
    Batches batches;
    unsigned int nbMachine;
    unsigned int ** jobTimePerMachine;
    unsigned int ** distances;
    unsigned int * deliveryTime;

} Instance;

#endif
