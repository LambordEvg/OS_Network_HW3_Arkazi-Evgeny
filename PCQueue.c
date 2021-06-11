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