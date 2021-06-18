//
// Created by Arkadi Piven on 11/06/2021.
//

#include "PCQueue.h"
#include "pthread.h"
#include "stdlib.h"
#include "List.h"

struct PCQueue_t{
    List list;
    size_t max_size;
    SCHEDULER_ALGORITHM schedAlg;
    pthread_mutex_t m;
    pthread_cond_t c;
};

static void block(PCQueue PCQ){
    while(ListGetSize(PCQ->list) == PCQ->max_size) {
        pthread_cond_wait(&PCQ->c, &PCQ->m);
    }
}
static void drop_tail(PCQueue PCQ){
    ListRemoveLast(PCQ->list);
}
static void drop_head(PCQueue PCQ){
    ListRemoveFirst(PCQ->list);
}
static void drop_random(PCQueue PCQ){
    size_t iterations = PCQ->max_size / 4;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand((unsigned)tv.tv_sec);
    for(size_t i = 0; i < iterations; ++i){
        size_t r = rand() % (size_t)ListGetSize(PCQ->list);
        ListRemoveByIndex(PCQ->list, (int)r);
    }
}

PCQueue initPCQueue(size_t size, SCHEDULER_ALGORITHM schedAlg){
    PCQueue PCQ = malloc(sizeof(*PCQ));
    if(PCQ == NULL){
        return NULL;
    }
    PCQ->list = initList();
    if(PCQ->list == NULL){
        free(PCQ);
        return NULL;
    }
    PCQ->max_size = size;
    PCQ->schedAlg = schedAlg;
    pthread_mutex_init(&PCQ->m, NULL);
    pthread_cond_init(&PCQ->c, NULL);
    return PCQ;
}

requestWithShit pop(PCQueue PCQ){
    pthread_mutex_lock(&PCQ->m);
    while(ListGetSize(PCQ->list) == 0){
        pthread_cond_wait(&PCQ->c, &PCQ->m);
    }
    requestWithShit ret = ListGetFirst(PCQ->list);
    if(DEBUG) printf("%lu is exiting the game blyat\n", ret.connfd);
    ListRemoveFirst(PCQ->list);
    PCQ->max_size--;
    pthread_cond_broadcast(&PCQ->c);
    pthread_mutex_unlock(&PCQ->m);
    return ret;
}

void push(PCQueue PCQ, requestWithShit connfd){
    pthread_mutex_lock(&PCQ->m);
    if(ListGetSize(PCQ->list) == PCQ->max_size){
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
    ListAddLast(PCQ->list, connfd);
    pthread_cond_broadcast(&PCQ->c);
    pthread_mutex_unlock(&PCQ->m);
}

void PCQueue_update_size(PCQueue PCQ){
    pthread_mutex_lock(&PCQ->m);
    PCQ->max_size++;
    pthread_mutex_unlock(&PCQ->m);

}

void PCQueue_destroy(PCQueue PCQ){
    printf("hehe");
    if(PCQ == NULL){
        return;
    }
    if(PCQ->list != NULL){
        ListDestroy(PCQ->list);
    }
    pthread_mutex_destroy(&PCQ->m);
    pthread_cond_destroy(&PCQ->c);
    free(PCQ);
}