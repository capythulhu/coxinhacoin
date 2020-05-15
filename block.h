// Header guards
#ifndef BLOCK_H
#define BLOCK_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "bytes.h"

// Estrutura do bloco
typedef struct _block {
    buffer  id;
    buffer  previousHash;
    list    *transactions;
    buffer  gold;
    long    timestamp;
} block;

// Gerar novo bloco
block *new_block(buffer data, buffer previousHash){
    block *output = malloc(sizeof(block));
    output->previousHash = previousHash;
    output->timestamp = time(NULL);
    output->id = get_block_hash(output);

    return output;
}

// Calcular a hash de um bloco
buffer get_block_hash(const block *b){
    buffer input = new_buffer(sizeof(long));

    int i;
    for(i = 0; i < sizeof(long); i++){
        input.bytes[i] = b->timestamp & (0xff << (i * 8));
    }

    listnode *temp = b->transactions->first;
    while(temp) concat_buffer(&input, ((transaction*)temp->val)->id);
    
    concat_buffer(&input, b->previousHash);

    return hash(input);
}

// Verifica se um buffer "gold" é uma hash de
// mineração válida para um buffer "seed"
static bool mine(buffer seed, buffer gold){
    buffer input = new_buffer(seed.length + gold.length);
    
    concat_buffer(&input, seed);
    concat_buffer(&input, gold);

    buffer output = hash(input);
    
    print_buffer(output);

    int i = 0, j = 1 << (8 - 1);
    while(!(output.bytes[i/8] & (j >> (i % 8))) && i <= DIFFICULTY) i++;
    return i >= DIFFICULTY;
}

// Minera um bloco
void mine_block(block *b) {
    b->gold = new_buffer(HASH_LENGTH);
    while(!mine(b->id, b->gold)) increment_buffer(b->gold);
}

// Adiciona transação
bool put_transaction_on_block(block *b, transaction *t, hashmap *outputs) {
    if(!b || !t) return false;
    if(b->previousHash.length > 0 && b->previousHash.bytes) {
        if(!process_transaction(t, outputs)) {
            printf("\nTransaction failed to process.");
        }
    }
    printf("\nTransaction processes successfully.");
    put_val_on_list(b->transactions, t);
    return true;
}
#endif