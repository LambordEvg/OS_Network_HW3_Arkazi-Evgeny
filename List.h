//
// Created by Arkadi Piven on 17/06/2021.
//

#ifndef HW3_WET_LIST_H
#define HW3_WET_LIST_H

#include "PCQueue.h"

typedef struct list_t* List;

typedef struct node_t* Node;

List initList();

void ListAddFirst(List list, requestWithShit Data);

void ListAddLast(List list, requestWithShit Data);

void ListRemoveLast(List list);

void ListRemoveFirst(List list);

void ListRemoveByIndex(List list, int index);

int ListGetSize(List list);

requestWithShit ListGetFirst(List list);

void ListDestroy(List list);

void ListPrinter(List list);

#endif //HW3_WET_LIST_H
