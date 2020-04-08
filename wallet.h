// Header guards
#ifndef WALLET_H
#define WALLET_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "bytes.h"
#include "keygen.h"

// Estrutura da carteira
typedef struct _wallet {
    rsaKey publicKey;
    rsaKey privateKey;
} wallet;

// Gerar nova carteira
wallet new_wallet(){
    wallet output;
    long *keys = get_keys();
    output.publicKey.key = keys[0];
    output.privateKey.key = keys[1];
    output.publicKey.n = keys[2];
    output.privateKey.n = keys[2];
    return output;
}
#endif