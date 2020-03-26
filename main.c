#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "bytes.h"
#include "hash.h"
#include "mining.h"
#include "block.h"

int main(){
    buffer bytes = new_buffer(HASH_LENGTH);
    zero_buffer(bytes);

    block test = new_block((buffer){"coronavirus", 11}, (buffer){"00000000", HASH_LENGTH});

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
}
