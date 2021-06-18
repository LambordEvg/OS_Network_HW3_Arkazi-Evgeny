#include "PCQueue.h"

#ifndef __REQUEST_H__

typedef struct{
    int reqFd;

    time_t arrival_sec;
    time_t arrival_usec;
    time_t dispatch_sec;
    time_t dispatch_usec;

    size_t threadID;
    size_t threadTotalCount;
    size_t threadStaticCount;
    size_t threadDynamicCount;
}Statistics_Shit;

int requestHandle(Statistics_Shit fd);

#endif
