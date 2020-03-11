#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "bytes.h"
#include "hash.h"
#include "mining.h"

int main(){
    buffer aaa = new_buffer(HASH_LENGTH);
    printf("length: %i\n", aaa.length);
    zero_buffer(aaa);

    buffer seed = { "polenta", 7 };

    int i = 0;
    while(!mine(seed, aaa)){
        i++;
        printf(" = ");
        print_buffer(aaa);
        printf("\n");
        increment_buffer(aaa);
    }
    printf(" = ");
    print_buffer(aaa);

    printf("\nBLOCO MINERADO! Hashes: %i\n", i);
    
}
