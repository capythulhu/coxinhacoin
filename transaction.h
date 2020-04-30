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

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
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
    rsaKey senderKey;
    unsigned long reciepientKey;
    float value;
    ibuffer signature;
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

// Checar assinatura
bool check_signature(transaction t){
    buffer claimedHash = decrypt(t.signature, t.senderKey);
    buffer originalHash = get_transaction_small_hash(t);
    return compare_buffer(claimedHash, originalHash);
}

// Gerar nova transação
transaction new_transaction(wallet senderWallet, long unsigned reciepientKey, float value){
    transaction output;
    output.senderKey = senderWallet.publicKey;
    output.reciepientKey = reciepientKey;
    output.value = value;

    buffer smallHash = get_transaction_small_hash(output);
    output.signature = encrypt(smallHash, senderWallet.privateKey);

    printf("Transacao de: %lu (%lu)\n", senderWallet.publicKey.key, senderWallet.publicKey.n);
    printf("Valor: %f\n", value);
    printf("Para: %lu\n", reciepientKey);
    printf("\n");
    printf("Hash da transacao: ");
    print_buffer(smallHash);

    printf("\n");

    printf("Assinatura da transacao: ");
    print_ibuffer(output.signature);
    printf("\n");
    printf("Hash da transacao desencriptada: ");
    print_buffer(decrypt(output.signature, senderWallet.publicKey));

    printf("\n");
    printf("Assinatura verificada? %i\n", check_signature(output));
    
    return output;
}
#endif