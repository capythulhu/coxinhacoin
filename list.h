#ifndef LIST_H
#define LIST_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdlib.h>
#endif

typedef struct _listnode{
    void *val;
    struct _listnode *next;
} listnode;

typedef struct _list{
    int size;
    struct _listnode *first;
} list;

list *new_list(){
    list *output = malloc(sizeof(list));
    output->size = 0;
    output->first = NULL;
    return output;
}

listnode *new_listnode(buffer key, void *val){
    listnode *output = malloc(sizeof(listnode));
    output->val = val;
    output->next = NULL;
    return output;
}

bool put_listnode(list *l, listnode *n){
    if(!n || !l) return;

    listnode *temp = l->first;
    if(l->size <= 0 || !temp) {
        l->first = n;
        l->size = 1;
        return true;
    }

    while(temp->next) temp = temp->next;
    temp->next = n;
    l->size++;
    
    return true;
}
#endif