#ifndef HASHMAP_H
#define HASHMAP_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdlib.h>
#endif

#include "bytes.h"

typedef struct _hashnode{
    buffer key;
    void *val;
    struct _hashnode *next;
} hashnode;

typedef struct _hashmap{
    int size;
    struct _hashnode *first;
} hashmap;

hashmap *new_hashmap(){
    hashmap *output = malloc(sizeof(hashmap));
    output->size = 0;
    output->first = NULL;
    return output;
}

hashnode *new_hashnode(buffer key, void *val){
    hashnode *output = malloc(sizeof(hashnode));
    output->key = key;
    output->val = val;
    output->next = NULL;
    return output;
}

bool put_hashnode(hashmap *h, hashnode *n){
    if(!n || !h) return;

    hashnode *temp = h->first;
    if(h->size <= 0 || !temp) {
        h->first = n;
        h->size = 1;
        return true;
    }

    while(temp->next){
        if(compare_buffer(temp->key, n->key)) return false;
        temp = temp->next;
    }
    temp->next = n;
    h->size++;

    return true;
}

void *get_hashmap_val(hashmap *h, buffer key){
    if(!h) return;

    hashnode *temp = h->first;
    if(!temp) return NULL;

    while(temp->next){
        if(compare_buffer(temp->key, key)) return temp->val;
        temp = temp->next;
    }
    
    if(compare_buffer(temp->key, key)) return temp->val;
    return NULL;
}
#endif