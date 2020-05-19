#ifndef LIST_H
#define LIST_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Nó da lista
typedef struct _listnode {
    void *val;
    struct _listnode *next;
} listnode;

// Lista
typedef struct _list{
    unsigned size;
    struct _listnode *first;
} list;

list *new_list(void);
listnode *new_listnode(buffer key, void *val);
bool put_val_on_list(list *l, void *val);
void *get_val_from_list(list *l, int index);

// Nova lista
list *new_list(void) {
    list *output = malloc(sizeof(list));
    output->size = 0;
    output->first = NULL;
    return output;
}

// Novo nó da lista
listnode *new_listnode(buffer key, void *val) {
    listnode *output = malloc(sizeof(listnode));
    output->val = val;
    output->next = NULL;
    return output;
}

// Insere o nó na lista
bool put_val_on_list(list *l, void *val) {
    if(!l) return false;
    
    listnode *n = malloc(sizeof(listnode));
    n->val = val;
    n->next = NULL;

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

// Obter valor de lista
void *get_val_from_list(list *l, int index) {
    if(!l) return NULL;

    int i = 0;
    listnode *temp = l->first;
    while(temp) {
        if(i == index) return temp->val;
        i++;
        temp = temp->next;
    }
    return NULL;
}
#endif