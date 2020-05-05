// Header guards
#ifndef WALLET_H
#define WALLET_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "bytes.h"
#include "keygen.h"
#include "hashmap.h"
#include "transactionio.h"

// Estrutura da carteira
typedef struct _wallet {
    rsaKey publicKey;
    rsaKey privateKey;
    hashmap *transactionOuts;
} wallet;

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
#endif