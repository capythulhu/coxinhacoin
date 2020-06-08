#define STDIO_H
#include <stdio.h>
#define STDBOOL_H
#include <stdbool.h>
#define TIME_H
#include <time.h>

#include "utils/bytes.h"
#include "crypto/hash.h"
#include "blockchain/block.h"
#include "blockchain/transaction.h"
#include "blockchain/wallet.h"
#include "utils/list.h"
#include "utils/hashmap.h"

list *blockchain;
transaction *gTransaction;

int main(void);
void add_block(block *b);

// Adiciona um bloco à blockchain
void add_block(block *b) {
    mine_block(b);
    put_val_on_list(blockchain, b);
}

// Checa a integridade da blockchain
bool check_blockchain_integrity(list *l) {
    block *b;
    block *c;
    hashmap *transactionOuts = new_hashmap();

    put_val_on_hashmap(
        transactionOuts,
        ((transactionout*)get_val_from_list(gTransaction->outputs, 0))->id,
        get_val_from_list(gTransaction->outputs, 0)
    );

    int i, j;
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

        if(!mine(b->id, b->gold, false)) {
            printf("Block has not been mined.\n");
            return false;
        }

        transactionout *out;
        for(j = 0; j < b->transactions->size; j++) {
            transaction *t = get_val_from_list(b->transactions, j);

            if(!check_signature(t)) {
                printf("Transaction signature does not match.\n");
                return false;
            }

            if(get_transaction_inputs_value(t) != get_transaction_outputs_value(t)) {
                printf("Transactions inputs and outputs are not equal.\n");
                return false;
            }

            listnode *temp = t->inputs->first;
            while(temp) {
                out = (transactionout*)get_val_from_hashmap(
                    transactionOuts,
                    ((transactionin*)temp->val)->outputId
                );
                
                if(!out) {
                    printf("Transaction references to invalid input.\n");
                    return false;
                }

                if(((transactionin*)temp->val)->output->value != out->value) {
                    printf("Transaction input value is invalid.\n");
                    return false;
                }

                rem_key_from_hashmap(
                    transactionOuts,
                    ((transactionin*)temp->val)->outputId
                );

                temp = temp->next;
            }

            temp = t->outputs->first;
            while(temp) {
                put_val_on_hashmap(
                    transactionOuts,
                    ((transactionout*)temp->val)->id,
                    temp->val
                );
                temp = temp->next;
            }

            if(((transactionout*)get_val_from_list(t->outputs, 0))->recipientKey
            != t->recipientKey) {
                printf("Transaction output recipient is invalid.\n");
                return false;
            }

            if(((transactionout*)get_val_from_list(t->outputs, 1))->recipientKey
            != t->senderKey.key) {
                printf("Transaction output change recipient is invalid.\n");
                return false;
            }
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
    gTransaction->id = (buffer) {"0", 1};

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
