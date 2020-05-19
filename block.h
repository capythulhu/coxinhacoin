// Header guards
#ifndef BLOCK_H
#define BLOCK_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "bytes.h"
#include "list.h"
#include "transaction.h"

// Estrutura do bloco
typedef struct _block {
    buffer  id;
    buffer  previousHash;
    list    *transactions;
    buffer  gold;
    long    timestamp;
} block;

buffer get_block_hash(const block *b);
block *new_block(buffer previousHash);
static bool mine(buffer seed, buffer gold);
void mine_block(block *b);
bool put_transaction_on_block(block *b, transaction *t, hashmap *outputs);


// Calcular a hash de um bloco
buffer get_block_hash(const block *b) {
    buffer input = new_buffer(sizeof(long));

    int i;
    for(i = 0; i < sizeof(long); i++) {
        input.bytes[i] = b->timestamp & (0xff << (i * 8));
    }

    listnode *temp = b->transactions->first;
    while(temp) {
        concat_buffer(&input, ((transaction*)temp->val)->id);
        temp = temp->next;
    }
    
    concat_buffer(&input, b->previousHash);

    return hash(input);
}

// Gerar novo bloco
block *new_block(buffer previousHash) {
    block *output = malloc(sizeof(block));
    output->previousHash = previousHash;
    output->timestamp = time(NULL);
    output->transactions = new_list();
    output->id = new_buffer(1);    

    return output;
}

// Minera um bloco
void mine_block(block *b) {
    printf("Mining block...\n");
    if(b->id.length > 0) free_buffer(b->id);

    b->id = get_block_hash(b);
    b->gold = new_buffer(HASH_LENGTH);

    while(!mine(b->id, b->gold)) increment_buffer(b->gold);
    printf("Block mined. Gold: ");
    print_buffer(b->gold);
    printf("\n");
}

// Adiciona transação
bool put_transaction_on_block(block *b, transaction *t, hashmap *outputs) {
    if(!b || !t) return false;
    if(b->previousHash.length > 0
        && !compare_buffer_with_int(b->previousHash, 0)
        && b->previousHash.bytes) {
        if(!process_transaction(t, outputs)) {
            printf("Transaction failed to process.\n");
        }
    }
    printf("Transaction processed successfully.\n");
    put_val_on_list(b->transactions, t);
    return true;
}
#endif