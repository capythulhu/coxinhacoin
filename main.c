#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_LENGTH 8

unsigned char *hash(unsigned char *input){
    const static unsigned char randomBytes[] = { 0x7c, 0x16, 0xc3, 0xb7, 0x00, 0xf9, 0xec, 0xaa };
    const static unsigned short int randomBytesLength = sizeof(randomBytes);

    unsigned char *digest = malloc(sizeof(char) * HASH_LENGTH);
    memset(digest, 0, HASH_LENGTH);
    
    int size = strlen(input), result = 0;
    int i, j;
    for(i = 0; i < size; i++){
        for(j = 0; j < HASH_LENGTH; j++){
            digest[i % HASH_LENGTH] = input[i] + i ^ randomBytes[(input[i] + i) % randomBytesLength];  
        }
        result ^= input[i] + i;
    }
    for(i = 0; i < HASH_LENGTH; i++){
        digest[i] ^= result + i;
    }
    return digest;
}

void print_bytes(unsigned char *input){
    int i;
    for(i = 0; i < HASH_LENGTH; i++){
        printf("%02x", input[i]);
    }
}

int main(){
    print_bytes(hash("banana"));
    printf("\n");
    print_bytes(hash("banano"));
}
