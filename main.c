#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "bytes.h"
#include "hash.h"
#include "mining.h"

int main(){
    buffer bytes = new_buffer(HASH_LENGTH);
    printf("length: %i\n", bytes.length);
    zero_buffer(bytes);

    buffer seed = { "polenta", 7 };

    int i = 0;
    while(!mine(seed, bytes)){
        i++;
        printf(" = ");
        print_buffer(bytes);
        printf("\n");
        increment_buffer(bytes);
    }
    printf(" = ");
    print_buffer(bytes);

    printf("\nBLOCO MINERADO! Hashes: %i\n", i);
    
}
