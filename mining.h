#ifndef MINING_H
#define MINING_H

#include "bytes.h"
#include "hash.h"

#define DIFICULTY 8

bool mine(buffer seed, buffer gold){
    buffer input = new_buffer(seed.length + gold.length);
    
    concat_buffer(&input, seed);
    concat_buffer(&input, gold);

    buffer output = hash(input);
    
    print_buffer(output);

    int i = 0, j = 1 << (8 - 1);
    while(!(output.bytes[i/8] & (j >> (i % 8))) && i <= DIFICULTY) i++;
    return i >= DIFICULTY;
}
#endif