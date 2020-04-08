// Header guards
#ifndef TRANSACTION_H
#define TRANSACTION_H

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#include "keygen.h"
#include "hash.h"
#include "bytes.h"
#include "ints.h"
#include "wallet.h"

// Máximo de casas após a vírgula de um valor de uma transação
#define DECIMAL_PLACES 6

// Estrutura de uma transação
typedef struct _transaction {
    buffer hash;
    rsaKey senderKey;
    long reciepientKey;
    float value;
    iBuffer signature;
} transaction;

// Obtenho a hash pequena
buffer get_transaction_small_hash(transaction t){
    buffer input = new_buffer(sizeof(long) + sizeof(float) + sizeof(rsaKey));

    int i;
    for(i = 0; i < sizeof(long); i++){
        input.bytes[i] = t.reciepientKey & (0xff << (i * 8));
    }
    int v = t.value * pow(10, DECIMAL_PLACES);
    for(i; i < sizeof(long) + sizeof(float); i++){
        input.bytes[i] = v & (0xff << (i * 8));
    }
    for(i; i < sizeof(long) + sizeof(float) + sizeof(long); i++){
        input.bytes[i] = t.senderKey.key & (0xff << (i * 8));
    }
    for(i; i < sizeof(long) + sizeof(float) + sizeof(rsaKey); i++){
        input.bytes[i] = t.senderKey.n & (0xff << (i * 8));
    }

    buffer output = hash(input);
    free(input.bytes);
    return output;
}

// Gerar nova transação
transaction new_transaction(wallet senderWallet, long reciepientKey, float value){
    transaction output;
    output.senderKey = senderWallet.publicKey;
    output.reciepientKey = reciepientKey;
    output.value = value;

    buffer smallHash = get_transaction_small_hash(output);
    iBuffer signature = encrypt(smallHash, senderWallet.publicKey);
    
    printf("Hash da transacao: ");
    print_buffer(smallHash);

    printf("\n");

    printf("Assinatura da transacao: ");
    print_iBuffer(signature);

    printf("\n");

    printf("Hash da transacao desencriptada: ");
    print_buffer(decrypt(signature, senderWallet.publicKey));
    
    return output;
}
#endif