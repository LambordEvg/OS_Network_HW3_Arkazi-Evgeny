//
// Created by Arkadi Piven on 17/06/2021.
//

#include "List.h"
#include "stdlib.h"
#include "stdio.h"
#include "PCQueue.h"

struct list_t{
    Node first;
    Node last;
    int size;
};

struct node_t{
    Node next;
    Node prev;
    requestWithShit Data;
};

static void List_ErrorHandler(){
    while(1){
        printf("Error!!!!\n");
    }
}

List initList(){
    List list = malloc(sizeof(*list));
    if(list == NULL){
        printf("WHY ALLOCATION FAILED, WHY?\n");
        List_ErrorHandler();
    };
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void ListAddFirst(List list, requestWithShit Data){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    };
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) return;
    list->size++;
    if(list->first == NULL){
        list->first = new_node;
        list->first->Data.connfd = Data.connfd;
        list->first->Data.type = Data.type;
        list->first->Data.arrival_sec = Data.arrival_sec;
        list->first->Data.arrival_usec = Data.arrival_usec;
        list->first->Data.dispatch_sec = Data.dispatch_sec;
        list->first->Data.dispatch_usec = Data.dispatch_usec;
        list->first->next = NULL;
        list->first->prev = NULL;
        list->last = list->first;
        return;
    }
    new_node->next = list->first;
    list->first->prev = new_node;
    new_node->prev = NULL;
    new_node->Data.connfd = Data.connfd;
    new_node->Data.type = Data.type;
    new_node->Data.arrival_sec = Data.arrival_sec;
    new_node->Data.arrival_usec = Data.arrival_usec;
    new_node->Data.dispatch_sec = Data.dispatch_sec;
    new_node->Data.dispatch_usec = Data.dispatch_usec;
    list->first = new_node;
}

void ListAddLast(List list, requestWithShit Data){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    };
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) return;
    list->size++;
    if(list->first == NULL){
        list->first = new_node;
        list->first->Data.connfd = Data.connfd;
        list->first->Data.type = Data.type;
        list->first->Data.arrival_sec = Data.arrival_sec;
        list->first->Data.arrival_usec = Data.arrival_usec;
        list->first->Data.dispatch_sec = Data.dispatch_sec;
        list->first->Data.dispatch_usec = Data.dispatch_usec;
        list->first->next = NULL;
        list->first->prev = NULL;
        list->last = list->first;
        return;
    }
    list->last->next = new_node;
    new_node->prev = list->last;
    list->last = new_node;
    list->last -> next = NULL;
    list->last->Data.connfd = Data.connfd;
    list->last->Data.type = Data.type;
    list->last->Data.arrival_sec = Data.arrival_sec;
    list->last->Data.arrival_usec = Data.arrival_usec;
    list->last->Data.dispatch_sec = Data.dispatch_sec;
    list->last->Data.dispatch_usec = Data.dispatch_usec;

}

void ListRemoveLast(List list){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    };
    if(list -> first == NULL){
        printf("WHY YOU GAVE ME AN EMPTY LIST, WHY?\n");
        List_ErrorHandler();
    };
    list->size--;
    if(list->last->prev == NULL){
        free(list->last);
        list->first = NULL;
        list->last = NULL;
        return;
    }
    Node temp = list->last->prev;
    free(list->last);
    list->last = temp;
    list->last->next = NULL;
}

requestWithShit ListRemoveFirst(List list){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    }
    if(list -> first == NULL){
        printf("WHY YOU GAVE ME AN EMPTY LIST, WHY?\n");
        List_ErrorHandler();
    }

    requestWithShit res = {0};
    list->size--;
    if(list->first->next == NULL){
        res = list->first->Data;
        free(list->first);
        list->first = NULL;
        list->last = NULL;
        return res;
    }
    Node temp = list->first->next;
    res = list->first->Data;
    free(list->first);
    list->first = temp;
    list->first->prev = NULL;
    return res;
}

requestWithShit ListRemoveByIndex(List list, int index){
    if(index < 0 || index > list->size - 1 || list->size == 0){
        printf("WHY YOU TRYING TO DO SHIT, WHY?\n");
        List_ErrorHandler();
    }
    requestWithShit res = {0};
    if(index == 0){
        res = ListGetFirst(list);
        ListRemoveFirst(list);
        return res;
    }
    if(index == list->size - 1){
        res = list->last->Data;
        ListRemoveLast(list);
        return res;
    }
    Node curr_node = list->first;
    int i = 0;
    while(i < index){
        curr_node = curr_node->next;
        ++i;
    }
    res = curr_node->Data;
    Node next = curr_node->next;
    Node prev = curr_node->prev;
    next -> prev = prev;
    prev -> next = next;
    free(curr_node);
    list->size--;
    return res;
}

static void NodeDestroy(Node node){
    if(node -> next == NULL){
        free(node);
        return;
    }
    NodeDestroy(node->next);
    free(node);
}

int ListGetSize(List list){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    };
    return list->size;
}

requestWithShit ListGetFirst(List list){
    if(list == NULL){
        printf("WHY YOU GAVE ME NULL LIST, WHY?\n");
        List_ErrorHandler();
    };
    if(list -> first == NULL){
        printf("WHY YOU GAVE ME AN EMPTY LIST, WHY?\n");
        List_ErrorHandler();
    };
    return list->first->Data;
}

void ListDestroy(List list){
    if(list == NULL){
        return;
    };
    if(list -> first == NULL){
        free(list);
        return;
    }
    Node first_node = list->first;
    NodeDestroy(first_node);
    free(list);
}

void ListPrinter(List list){
    if(list == NULL || list->first == NULL){
        printf("List is empty\n");
        return;
    }
    Node curr_node = list->first;
    printf("FileDescriptor #%lu\n", curr_node->Data.connfd);
    printf("Request Type is %s\n", (curr_node->Data.type == STATIC_REQUEST ? "STATIC" : "DYNAMIC"));
    printf("Arrived At %s\n", asctime(localtime(&curr_node->Data.arrival_sec)));
    printf("Dispatched At %s", asctime(localtime(&curr_node->Data.dispatch_sec)));
    curr_node = curr_node->next;
    while(curr_node != NULL){
        printf("\n==============================Next NODE==============================\n");
        printf("FileDescriptor #%lu\n",curr_node->Data.connfd);
        printf("Request Type is %s\n", (curr_node->Data.type == STATIC_REQUEST ? "STATIC" : "DYNAMIC"));
        printf("Arrived At %s\n", asctime(localtime(&curr_node->Data.arrival_sec)));
        printf("Dispatched At %s", asctime(localtime(&curr_node->Data.dispatch_sec)));
        curr_node = curr_node->next;
    }
    printf("\n");
}
