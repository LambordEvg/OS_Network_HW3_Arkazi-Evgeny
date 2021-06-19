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
    if(strcmp(argv[4], "block") == 0){
        *schedalg = BLOCK;
    }
    else if(strcmp(argv[4], "dt") == 0){
        *schedalg = DROP_TAIL;
    }
    else if(strcmp(argv[4], "dh") == 0){
        *schedalg = DROP_HEAD;
    }
    else *schedalg = DROP_RANDOM;
}

typedef struct{
    pthread_t thread;
    size_t threadID;
    size_t handleCount;
    size_t staticCount;
    size_t dynamicCount;

    PCQueue queue;
}Pthread_with_shit;

void* Request(void* thread){
    while(true) {
        requestWithShit req = pop(((Pthread_with_shit*)thread)->queue);
        struct timeval dispatch;
        gettimeofday(&dispatch, NULL);
        req.dispatch_sec = dispatch.tv_sec;
        req.dispatch_usec = dispatch.tv_usec;

        Statistics_Shit stShit;
        stShit.reqFd = req.connfd;
        stShit.arrival_sec = req.arrival_sec;
        stShit.arrival_usec = req.arrival_usec;
        stShit.dispatch_sec = req.dispatch_sec - req.arrival_sec;
        stShit.dispatch_usec = req.dispatch_usec - req.arrival_usec;
        stShit.threadID = ((Pthread_with_shit*)thread)->threadID;
        stShit.threadStaticCount = ((Pthread_with_shit*)thread)->staticCount;
        stShit.threadDynamicCount = ((Pthread_with_shit*)thread)->dynamicCount;
        stShit.threadTotalCount = ((Pthread_with_shit*)thread)->handleCount + 1;

        int res = requestHandle(stShit);
        PCQueue_update_size(((Pthread_with_shit*)thread)->queue);
        Close((int)req.connfd);

        req.type = (res == 1 ? STATIC_REQUEST : (res == 0 ? DYNAMIC_REQUEST : UNDEFINED_REQUEST));

        if(req.type == STATIC_REQUEST) {
            ((Pthread_with_shit *) thread)->staticCount++;
        }else if(req.type == DYNAMIC_REQUEST) {
            ((Pthread_with_shit *) thread)->dynamicCount++;
        } else{
            /* VERY IMPORTANT FUNCTIONALITY - DO NOTHING! */
        }
        ((Pthread_with_shit*)thread)->handleCount++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, thread_count;
    size_t PCQ_size;
    SCHEDULER_ALGORITHM schedalg;
    struct sockaddr_in clientaddr;

    getargs(&port, &thread_count, &PCQ_size, &schedalg, argc, argv);

    PCQueue PCQ = initPCQueue(PCQ_size, schedalg);
    Pthread_with_shit* workers = malloc(sizeof(Pthread_with_shit) * thread_count);
    for(size_t i = 0; i < thread_count; ++i){
        workers[i].queue = PCQ;
        workers[i].threadID = i;
        workers[i].dynamicCount = 0;
        workers[i].staticCount = 0;
        workers[i].handleCount = 0;
        pthread_create(&workers[i].thread, NULL, Request, (void*)&workers[i]);
    }

    listenfd = Open_listenfd(port);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	    struct timeval arrival;
	    gettimeofday(&arrival, NULL);
	    requestWithShit newReq = {0};
	    newReq.connfd = connfd;
	    newReq.type = UNDEFINED_REQUEST;
        newReq.arrival_sec = arrival.tv_sec;
	    newReq.arrival_usec = arrival.tv_usec;
	    newReq.dispatch_sec = 0;
        newReq.dispatch_usec = 0;

        push(PCQ, newReq);
    }
    for(int i = 0; i < thread_count; ++i){
        pthread_join(workers[i].thread, NULL);
    }
    PCQueue_destroy(PCQ);
}
    


 
