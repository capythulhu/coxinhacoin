// Header guards
#ifndef HASH_H
#define HASH_H

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#include "coin.h"
#include "bytes.h"

// Calcular a hash de um buffer
buffer hash(buffer input){
    const static unsigned char randomBytes[] = { 0x78, 0x16, 0xc3, 0xb3, 0x52, 0xf9, 0xe4, 0xad };
    const static unsigned short int randomBytesLength = sizeof(randomBytes);

    buffer digest = new_buffer(HASH_LENGTH);
    zero_buffer(digest);
    
    int result = 0;
    int i, j;
    for(i = 0; i < input.length; i++){
        for(j = 0; j < HASH_LENGTH; j++){
            digest.bytes[i % digest.length] = input.bytes[i] + i ^ randomBytes[(input.bytes[i] + i) % randomBytesLength];  
        }
        result ^= input.bytes[i] + i;
    }
    for(i = 0; i < digest.length; i++){
        digest.bytes[i] ^= result + i;
    }
    return digest;
}
#endif