// Header guards
#ifndef WALLET_H
#define WALLET_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "list.h"
#include "bytes.h"
#include "keygen.h"
#include "hashmap.h"
#include "transaction.h"
#include "transactionio.h"

// Estrutura da carteira
typedef struct _wallet {
    rsaKey publicKey;
    rsaKey privateKey;
    hashmap *transactionOuts;
} wallet;

// Gerar nova carteira
wallet new_wallet() {
    wallet output;
    long *keys = get_keys();
    output.publicKey.key = keys[0];
    output.privateKey.key = keys[1];
    output.publicKey.n = keys[2];
    output.privateKey.n = keys[2];
    return output;
}

// Obter saldo
float get_balance(wallet w) {
    float total = 0;
    hashnode *temp = w.transactionOuts->first;

    while(temp) {
        total += ((transactionout*)(temp->val))->value;
        temp = temp->next;
    };
    return total;
}

transaction *send_funds(wallet w, unsigned long recipientKey, float value) {
    if(get_balance(w) < value) {
        printf("\nNot enough funds to complete transaction.");
        return NULL;
    }
    list *inputs = new_list();
    
    float total = 0;
    hashnode *temp = w.transactionOuts->first;

    while(temp) {
        transactionout *out = (transactionout*)(temp->val);
        total += out->value;
        put_val_on_list(inputs, new_transactionin(out->id));
        if(total > value) break;
        temp = temp->next;
    };

    transaction *t = new_transaction(w.publicKey, recipientKey, value, inputs);
    t->signature = encrypt(t->id, w.privateKey);

    listnode *input = inputs->first;
    while(input) {
        rem_key_from_hashmap(w.transactionOuts, (((transactionin*)input->val))->outputId);
    }

    return t;
}
#endif