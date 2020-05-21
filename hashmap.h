#ifndef HASHMAP_H
#define HASHMAP_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#include "bytes.h"

// Nó do hashmap
typedef struct _hashnode{
    buffer key;
    void *val;
    struct _hashnode *next;
} hashnode;

// Hashmap
typedef struct _hashmap{
    unsigned size;
    struct _hashnode *first;
} hashmap;

hashmap *new_hashmap(void);
hashnode *new_hashnode(buffer key, void *val);
bool put_hashnode(hashmap *h, hashnode *n);
bool rem_key_from_hashmap(hashmap *h, buffer key);
void *get_val_from_hashmap(hashmap *h, buffer key);
bool put_val_on_hashmap(hashmap *h, buffer key, void *val);

// Novo hashmap
hashmap *new_hashmap(void) {
    hashmap *output = malloc(sizeof(hashmap));
    output->size = 0;
    output->first = NULL;
    return output;
}

// Novo nó do hashmap
hashnode *new_hashnode(buffer key, void *val) {
    hashnode *output = malloc(sizeof(hashnode));
    output->key = key;
    output->val = val;
    output->next = NULL;
    return output;
}

// Insere nó no hashmap
bool put_hashnode(hashmap *h, hashnode *n) {
    if(!n || !h) return false;

    hashnode *temp = h->first;
    if(h->size <= 0 || !temp) {
        h->first = n;
        h->size = 1;
        return true;
    }

    while(temp->next) {
        if(compare_buffer(temp->key, n->key)) return false;
        temp = temp->next;
    }
    temp->next = n;
    h->size++;

    return true;
}

// Remove nó do hashmap
bool rem_key_from_hashmap(hashmap *h, buffer key) {
    if(!h) return false;

    hashnode *temp = h->first;
    if(h->size <= 0 || !temp) return false;

    if(compare_buffer(temp->key, key)) {
        h->first = temp->next;
        free(temp);
        h->size--;
        return true;
    }
    if(!temp->next) return false;
    
    hashnode *prev;
    do {
        prev = temp;
        temp = temp->next;
        if(compare_buffer(temp->key, key)) {
            prev->next = temp->next;
            free(temp);
            h->size--;
            return true;
        }
    } while(temp->next);
    return false;
}

// Obtém um valor a partir da chave associada a ele
void *get_val_from_hashmap(hashmap *h, buffer key) {
    if(!h) return NULL;

    hashnode *temp = h->first;
    if(!temp) return NULL;

    while(temp->next) {
        if(compare_buffer(temp->key, key)) return temp->val;
        temp = temp->next;
    }
    
    if(compare_buffer(temp->key, key)) return temp->val;
    return NULL;
}

// Insere o nó no hashmap
bool put_val_on_hashmap(hashmap *h, buffer key, void *val) {
    if(!h) return false;
    
    hashnode *n = malloc(sizeof(hashnode));
    n->key = key;
    n->val = val;
    n->next = NULL;

    hashnode *temp = h->first;
    if(h->size <= 0 || !temp) {
        h->first = n;
        h->size = 1;
        return true;
    }

    while(temp->next) temp = temp->next;
    temp->next = n;
    h->size++;
    
    return true;
}
#endif