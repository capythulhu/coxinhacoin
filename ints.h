// Header guards
#ifndef INTS_H
#define INTS_H

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

// Estrutura de um buffer (conjunto de longs)
typedef struct _iBuffer {
    unsigned int *ints;
    int length;
} iBuffer;

// Gerar novo buffer
iBuffer new_iBuffer(int length){
    iBuffer output;
    output.length = length;
    if(length > 0){
        output.ints = malloc(length * sizeof(long));
    } else {
        output.ints = NULL;
    }
    return output;
}

// Zerar um buffer
void zero_iBuffer(iBuffer input){
    int i;
    for(i = 0; i < input.length; i++){
        input.ints[i] = 0;
    }
}

// Exibir um buffer
void print_iBuffer(iBuffer input){
    int i;
    for(i = 0; i < input.length; i++){
        printf("%i", input.ints[i]);
        if(i < input.length - 1) printf("-");
    }
}

#endif
