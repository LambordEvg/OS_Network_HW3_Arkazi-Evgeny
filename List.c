//
// Created by Arkadi Piven on 17/06/2021.
//

#include "List.h"
#include "stdlib.h"
#include "stdio.h"

struct list_t{
    Node first;
    Node last;
    int size;
};

struct node_t{
    Node next;
    Node prev;
    int Data;
};

List initList(){
    List list = malloc(sizeof(*list));
    if(list == NULL){
        return NULL;
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void ListAddFirst(List list, int Data){
    if(list == NULL) return;
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) return;
    list->size++;
    if(list->first == NULL){
        list->first = new_node;
        list->first->Data = Data;
        list->first->next = NULL;
        list->first->prev = NULL;
        list->last = list->first;
        return;
    }
    new_node->next = list->first;
    list->first->prev = new_node;
    new_node->prev = NULL;
    new_node->Data = Data;
    list->first = new_node;
}

void ListAddLast(List list, int Data){
    if(list == NULL) return;
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) return;
    list->size++;
    if(list->first == NULL){
        list->first = new_node;
        list->first->Data = Data;
        list->first->next = NULL;
        list->first->prev = NULL;
        list->last = list->first;
        return;
    }
    list->last->next = new_node;
    new_node->prev = list->last;
    list->last = new_node;
    list->last -> next = NULL;
    list->last->Data = Data;
}

void ListRemoveLast(List list){
    if(list == NULL) return;
    if(list->first == NULL) return;
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

void ListRemoveFirst(List list){
    if(list == NULL) return;
    if(list->first == NULL) return;
    list->size--;
    if(list->first->next == NULL){
        free(list->first);
        list->first = NULL;
        list->last = NULL;
        return;
    }
    Node temp = list->first->next;
    free(list->first);
    list->first = temp;
    list->first->prev = NULL;
}

void ListRemoveByIndex(List list, int index){
    if(index < 0 || index > list->size - 1 || list->size == 0) return;
    if(index == 0){
        ListRemoveFirst(list);
        return;
    }
    if(index == list->size - 1){
        ListRemoveLast(list);
        return;
    }
    Node curr_node = list->first;
    int i = 0;
    while(i < index){
        curr_node = curr_node->next;
        ++i;
    }
    Node next = curr_node->next;
    Node prev = curr_node->prev;
    next -> prev = prev;
    prev -> next = next;
    free(curr_node);
    list->size--;
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
    if(list == NULL) return -1;
    return list->size;
}

int ListGetFirst(List list){
    if(list == NULL) return -1;
    if(list -> first == NULL) return -1;
    return list->first->Data;
}

void ListDestroy(List list){
    if(list == NULL) return;
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
    printf("%d ", curr_node->Data);
    curr_node = curr_node->next;
    while(curr_node != NULL){
        printf("-> %d ",curr_node->Data);
        curr_node = curr_node->next;
    }
    printf("\n");
}
