// Header guards
#ifndef WALLET_H
#define WALLET_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "../utils/list.h"
#include "../utils/bytes.h"
#include "../crypto/keygen.h"
#include "../utils/hashmap.h"
#include "../blockchain/transaction.h"
#include "../blockchain/transactionio.h"

// Estrutura da carteira
typedef struct _wallet {
    rsaKey publicKey;
    rsaKey privateKey;
    hashmap *transactionOuts;
} wallet;

wallet *new_wallet(void);
float get_balance(const wallet *w, hashmap *outputs);
transaction *send_funds(const wallet *w, unsigned long recipientKey, float value, hashmap *outputs);

// Gerar nova carteira
wallet *new_wallet(void) {
    wallet *output = malloc(sizeof(wallet));
    unsigned long *keys = get_keys();
    output->publicKey.key = keys[0];
    output->privateKey.key = keys[1];
    output->publicKey.n = keys[2];
    output->privateKey.n = keys[2];
    output->transactionOuts = new_hashmap();
    return output;
}

// Obter saldo
float get_balance(const wallet *w, hashmap *outputs) {
    float total = 0;
    hashnode *temp = outputs->first;

    while(temp) {
        transactionout *current = ((transactionout*)temp->val);
        if(current->recipientKey == w->publicKey.key) {
            total += current->value;
            put_val_on_hashmap(w->transactionOuts, current->id, current);
        }
        temp = temp->next;
    };
    return total;
}

// Fazer uma transação
transaction *send_funds(const wallet *w, unsigned long recipientKey, float value, hashmap *outputs) {
    if(get_balance(w, outputs) < value) {
        printf("Not enough funds to complete transaction.\n");
        return NULL;
    }
    list *inputs = new_list();
    
    float total = 0;
    hashnode *temp = w->transactionOuts->first;

    while(temp) {
        transactionout *out = (transactionout*)(temp->val);
        total += out->value;
        put_val_on_list(inputs, new_transactionin(out->id));
        if(total > value) break;
        temp = temp->next;
    };

    transaction *t = new_transaction(w->publicKey, recipientKey, value, inputs);
    t->signature = encrypt(t->id, w->privateKey);

    listnode *input = inputs->first;
    while(input) {
        rem_key_from_hashmap(w->transactionOuts, (((transactionin*)input->val))->outputId);
        input = input->next;
    }

    return t;
}
#endif