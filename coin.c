#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "hash.h"
#include "bytes.h"
#include "mining.h"

int main(){
    unsigned char* bytes = malloc(sizeof(char) * HASH_LENGTH);
    memset(bytes, 0, HASH_LENGTH);
    
    int i = 0;
    while(!mine("aimeodeus", bytes)){
        i++;
        printf(" = ");
        print_bytes(bytes);
        printf("\n");
        increment_bytes(bytes);
    }
    printf(" = ");
    print_bytes(bytes);

    printf("\nBLOCO MINERADO! Hashes: %i\n", i);
    
}
