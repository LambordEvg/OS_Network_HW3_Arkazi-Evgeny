#include <stdbool.h>
#include "segel.h"
#include "request.h"
#include "PCQueue.h"
#include <pthread.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too

#define TEST_SIZE                   5

bool test_PCQueue();

void getargs(int* port, int* thread_count, size_t* PCQ_size, SCHEDULER_ALGORITHM * schedalg,
             int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *thread_count = atoi(argv[2]);
    *PCQ_size = atoi(argv[3]);
    if(strcmp(argv[4], "block")){
        *schedalg = BLOCK;
    }
    else if(strcmp(argv[4], "dt")){
        *schedalg = DROP_TAIL;
    }
    else if(strcmp(argv[4], "dh")){
        *schedalg = DROP_HEAD;
    }
    else *schedalg = DROP_RANDOM;
}

void* Request(void* PCQ){
    while(true) {
        size_t connfd = pop((PCQueue)PCQ);
        printf("Vasya 2 handeling %lu\n", connfd);
        requestHandle((int) connfd);
        PCQueue_update_size((PCQueue)PCQ);
        Close(connfd);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, thread_count;
    size_t PCQ_size;
    SCHEDULER_ALGORITHM schedalg;
    struct sockaddr_in clientaddr;

    /*if(!test_PCQueue()){
        printf("hehehe\n");
        exit(9000);
    }*/

    getargs(&port, &thread_count, &PCQ_size, &schedalg, argc, argv);

    PCQueue PCQ = initPCQueue(PCQ_size, schedalg);
    pthread_t* Workers = malloc(sizeof(pthread_t)*thread_count);
    for(int i = 0; i < thread_count; ++i){
        pthread_create(Workers + i, NULL, Request, (void*) PCQ);
    }

    listenfd = Open_listenfd(port);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	    printf("vasya %d\n", connfd);
        push(PCQ, (size_t)connfd);
    }
    for(int i = 0; i < thread_count; ++i){
        pthread_join(Workers[i], NULL);
    }
    PCQueue_destroy(PCQ);
}

typedef struct{
    PCQueue queue;
    size_t numToPush;
}Queue_struct;

void* push_numbers(void* struct_with_queue){
    PCQueue tmp_queue = ((Queue_struct*)struct_with_queue)->queue;
    size_t tmp_num = ((Queue_struct*)struct_with_queue)->numToPush;
    printf("the %d thread started pushing\n", (int)(tmp_num/2));
    printf("The %d number is %d\n", (int)(tmp_num/2), (int)pop(tmp_queue));
    printf("the %d thread ended\n", (int)(tmp_num/2));
    return NULL;
}


bool test_PCQueue(){

    PCQueue queue = initPCQueue(10, BLOCK);

    if(queue == NULL){
        printf("4to za nahuy?!!!! Arkazi Blyat?!?!?!?!\n");
        return false;
    }

    pthread_t threads[TEST_SIZE];

    Queue_struct worker[TEST_SIZE];

    for(size_t i = 0; i < TEST_SIZE; ++i){
        worker[i].queue = queue;
        worker[i].numToPush = i * 2;
        pthread_create(&threads[i], NULL, push_numbers, (void*)&worker[i]);
        push(queue, i * 2);
    }

    for(int i = 0; i < TEST_SIZE; ++i){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < TEST_SIZE; ++i){
        printf("%d done\n", i);
    }

    PCQueue_destroy(queue);

    return true;
}

    


 
