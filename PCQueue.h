//
// Created by Arkadi Piven on 11/06/2021.
//

#include "stdio.h"

#ifndef HW3_WET_PCQUEUE_H
#define HW3_WET_PCQUEUE_H

typedef enum{
    PCQueue_SUCCESS,
    PCQueue_FULL,
    PCQueue_ALLOCATION_ERROR,
    PCQueue_EMPTY
} PCQueue_STATUS;

typedef enum{
    BLOCK,
    DROP_TAIL,
    DROP_HEAD,
    DROP_RANDOM
} SCHEDULER_ALGORITHM;

typedef struct PCQueue_t* PCQueue;

PCQueue initPCQueue(size_t size, SCHEDULER_ALGORITHM schedAlg);

PCQueue_STATUS pop(PCQueue PCQ, size_t* connfd);

PCQueue_STATUS push(PCQueue PCQ, size_t connfd);

void PCQueue_destroy(PCQueue PCQ);









#endif //HW3_WET_PCQUEUE_H
