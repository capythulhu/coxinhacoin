// Header guards
#ifndef BLOCK_H
#define BLOCK_H

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "bytes.h"

// Tamanho em bytes do conteúdo de cada bloco
#define DATA_LENGTH 32

// Estrutura do bloco
typedef struct _block {
    buffer hash;
    buffer previousHash;
    buffer data;
    long timestamp;
} block;

// Calcular a hash de um bloco
buffer get_block_hash(block b){
    buffer input = new_buffer(sizeof(long));

    int i;
    for(i = 0; i < sizeof(long); i++){
        input.bytes[i] = b.timestamp & (0xff << (i * 8));
    }

    concat_buffer(&input, b.previousHash);
    concat_buffer(&input, b.data);

    return hash(input);
}

// Gerar novo bloco
block new_block(buffer data, buffer previousHash){
    block output;
    output.previousHash = previousHash;
    output.data = data;
    
    output.timestamp = time(NULL);

    output.hash = get_block_hash(output);

    return output;
}
#endif