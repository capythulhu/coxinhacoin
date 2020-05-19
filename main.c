#define STDIO_H
#include <stdio.h>
#define STDBOOL_H
#include <stdbool.h>
#define TIME_H
#include <time.h>

#include "bytes.h"
#include "hash.h"
#include "block.h"
#include "transaction.h"
#include "wallet.h"
#include "list.h"
#include "hashmap.h"

list *blockchain;
transaction *gTransaction;

int main(void);
void add_block(block *b);

// Adiciona um bloco à blockchain
void add_block(block *b) {
    mine_block(b);
    put_val_on_list(blockchain, b);
}

bool check_blockchain_integrity(list *l) {
    block *b;
    block *c;
    hashmap *transactionOuts = new_hashmap();

    put_val_on_hashmap(
        transactionOuts,
        ((transactionout*)get_val_from_list(gTransaction->outputs, 0))->id,
        get_val_from_list(gTransaction->outputs, 0)
    );

    int i;
    for(i = 1; i < l->size; i++) {
        b = (block*)get_val_from_list(l, i);
        c = (block*)get_val_from_list(l, i - 1);

        if(!compare_buffer(b->id, get_block_hash(b))) {
            printf("Current hashes are not valid.\n");
            return false;
        }

        if(!compare_buffer(c->id, get_block_hash(c))) {
            printf("Previous hashes are not valid.\n");
            return false;
        }

        if(!mine(b->id, b->gold)) {
            printf("Block has not been mined\n");
            return false;
        }
    }

    printf("Blockchain is valid.\n");
    return true;
}

int main(void) {
    blockchain = new_list();
    hashmap *transactionOuts = new_hashmap();

    srand(time(NULL));
    wallet *coinbase = new_wallet();
    wallet *w1 = new_wallet();
    wallet *w2 = new_wallet();

    gTransaction = new_transaction(coinbase->publicKey, w1->publicKey.key, 100, NULL);
    gTransaction->signature = encrypt(gTransaction->id, coinbase->privateKey);
    char gChar = '0';
    gTransaction->id = (buffer) {&gChar, 1};

    put_val_on_list(gTransaction->outputs, 
        new_transactionout(
            gTransaction->recipientKey, 
            gTransaction->value, 
            gTransaction->id
        )
    );

    put_val_on_hashmap(transactionOuts, 
        ((transactionout*)get_val_from_list(gTransaction->outputs, 0))->id,
        get_val_from_list(gTransaction->outputs, 0)
    );
        
    block *gBlock = new_block(new_buffer(1));
    put_transaction_on_block(gBlock, gTransaction, transactionOuts);
    getchar();
    add_block(gBlock);

    // Teste
    block *block1 = new_block(gBlock->id);
    printf("W1: %.6f\n", get_balance(w1, transactionOuts));
    printf("W2: %.6f\n", get_balance(w2, transactionOuts));
    put_transaction_on_block(block1, 
        send_funds(w1, w2->publicKey.key, 40, transactionOuts), 
        transactionOuts
    );
    getchar();
    add_block(block1);
    printf("W1: %.6f\n", get_balance(w1, transactionOuts));
    printf("W2: %.6f\n", get_balance(w2, transactionOuts));

    check_blockchain_integrity(blockchain);
}
