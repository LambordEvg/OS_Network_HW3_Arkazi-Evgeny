//
// Created by Arkadi Piven on 11/06/2021.
//

#include "PCQueue.h"
#include "pthread.h"
#include "unistd.h"
#include "stdlib.h"

struct PCQueue_t{
    size_t* array;
    size_t max_size;
    size_t curr_size;
    size_t head;
    size_t tail;
    SCHEDULER_ALGORITHM schedAlg;
    pthread_mutex_t m;
    pthread_cond_t c;
};

static void block(PCQueue PCQ){
    while(PCQ->curr_size == PCQ->max_size) {
        pthread_cond_wait(&PCQ->c, &PCQ->m);
    }
}
static void drop_tail(PCQueue PCQ){
    PCQ->tail = (PCQ->tail - 1) % (PCQ->max_size);
    PCQ->curr_size--;
}
static void drop_head(PCQueue PCQ){
    PCQ->head = (PCQ->head + 1) % (PCQ->max_size);
    PCQ->curr_size--;
}
static void drop_random(PCQueue PCQ){
    size_t iterations = PCQ->max_size / 4;
    srand((unsigned) time(NULL));
    for(size_t i = 0; i < iterations; ++i){
        size_t r = rand() % PCQ->curr_size;
        PCQ->array[(PCQ->head + r) % PCQ->max_size] = PCQ->array[PCQ->tail - 1];
        PCQ->tail = (PCQ->tail - 1) % (PCQ->max_size);
        PCQ->curr_size--;
    }
}

PCQueue initPCQueue(size_t size, SCHEDULER_ALGORITHM schedAlg){
    PCQueue PCQ = malloc(sizeof(struct PCQueue_t));
    if(PCQ == NULL){
        return NULL;
    }
    PCQ->array = malloc(sizeof(size_t)*size);
    if(PCQ->array == NULL){
        free(PCQ);
        return NULL;
    }
    PCQ->max_size = size;
    PCQ->curr_size = 0;
    PCQ->head = 0;
    PCQ->tail = 0;
    PCQ->schedAlg = schedAlg;
    pthread_mutex_init(&PCQ->m, NULL);
    pthread_cond_init(&PCQ->c, NULL);
    return PCQ;
}

PCQueue_STATUS pop(PCQueue PCQ, size_t* connfd){
    pthread_mutex_lock(&PCQ->m);
    while(PCQ->curr_size == 0){
        pthread_cond_wait(&PCQ->c, &PCQ->m);
    }
    *connfd = PCQ->array[PCQ->head];
    PCQ->head = (PCQ->head + 1)%(PCQ->max_size);
    PCQ->curr_size--;
    pthread_cond_broadcast(&PCQ->c);
    pthread_mutex_unlock(&PCQ->m);
    return PCQueue_SUCCESS;
}

PCQueue_STATUS push(PCQueue PCQ, size_t connfd){
    pthread_mutex_lock(&PCQ->m);
    if(PCQ->curr_size == PCQ->max_size){
        switch(PCQ->schedAlg){
            case (BLOCK):
                block(PCQ);
                break;
            case (DROP_TAIL):
                drop_tail(PCQ);
                break;
            case (DROP_HEAD):
                drop_head(PCQ);
                break;
            case (DROP_RANDOM):
                drop_random(PCQ); // UPDATE SIZE INSIDE
                break;
        }
    }
    PCQ->array[PCQ->tail] = connfd;
    PCQ->tail = (PCQ->tail + 1) % (PCQ->max_size);
    PCQ->curr_size++;
    pthread_cond_broadcast(&PCQ->c);
    pthread_mutex_unlock(&PCQ->m);
    return PCQueue_SUCCESS;
}

void PCQueue_destroy(PCQueue PCQ){
    if(PCQ == NULL){
        return;
    }
    if(PCQ->array != NULL){
        free(PCQ->array);
    }
    pthread_mutex_destroy(&PCQ->m);
    pthread_cond_destroy(&PCQ->c);
    free(PCQ);
}