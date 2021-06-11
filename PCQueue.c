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
    pthread_mutex_t m;
    pthread_cond_t c;
};



PCQueue initPCQueue(size_t size){
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
    pthread_mutex_init(&PCQ->m, NULL);
    pthread_cond_init(&PCQ->c, NULL);
    return PCQ;
}

PCQueue_STATUS pop(PCQueue PCQ, size_t* connfd){

}

PCQueue_STATUS push(PCQueue PCQ, size_t connfd){

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