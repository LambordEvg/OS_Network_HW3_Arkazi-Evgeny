#include "segel.h"
#include "request.h"
#include "PCQueue.h"

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

void Request(PCQueue PCQ){
    size_t connfd = pop(PCQ);
    requestHandle((int)connfd);
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, thread_count;
    size_t PCQ_size;
    SCHEDULER_ALGORITHM schedalg;
    struct sockaddr_in clientaddr;

    getargs(&port, &thread_count, &PCQ_size, &schedalg, argc, argv);

    PCQueue PCQ = initPCQueue(PCQ_size, schedalg);



    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    push(PCQ, (size_t)connfd);
	Close(connfd);
    }

}


    


 
