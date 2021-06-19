//
// Created by Arkadi Piven on 11/06/2021.
//

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#ifndef HW3_WET_PCQUEUE_H
#define HW3_WET_PCQUEUE_H

typedef enum{
    STATIC_REQUEST,
    DYNAMIC_REQUEST,
    UNDEFINED_REQUEST
}requestType;

typedef enum{
    BLOCK,
    DROP_TAIL,
    DROP_HEAD,
    DROP_RANDOM
} SCHEDULER_ALGORITHM;

typedef struct{
    size_t connfd;
    requestType type;

    time_t arrival_sec;
    time_t arrival_usec;
    time_t dispatch_sec;
    time_t dispatch_usec;
}requestWithShit;

typedef struct PCQueue_t* PCQueue;

PCQueue initPCQueue(size_t size, SCHEDULER_ALGORITHM schedAlg);

requestWithShit pop(PCQueue PCQ);

void push(PCQueue PCQ, requestWithShit request);

void PCQueue_update_size(PCQueue PCQ);

void PCQueue_destroy(PCQueue PCQ);

#endif //HW3_WET_PCQUEUE_H
