#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "bytes.h"
#include "hash.h"
#include "mining.h"
#include "block.h"
#include "transaction.h"

int main(){
    wallet w = new_wallet();
    transaction t = new_transaction(w, 1234, 10);

    
    /*
    // Buffer de gold para a mineração
    buffer bytes = new_buffer(HASH_LENGTH);
    zero_buffer(bytes);

    // Bloco de teste
    block test = new_block((buffer){"coronavirus", 11}, (buffer){"00000000", HASH_LENGTH});

    // Minerar e exibir as hashes e gold
    int i = 0;
    while(!mine(test.hash, bytes)){
        i++;
        printf(" = ");
        print_buffer(bytes);
        printf("\n");
        increment_buffer(bytes);
    }
    printf(" = ");
    print_buffer(bytes);

    printf("\nBLOCO MINERADO!\nHashes: %i\n", i + 1);
    print_buffer(test.data);
    printf("\nTimestamp: %li", test.timestamp);
    */
}
