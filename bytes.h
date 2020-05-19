// Header guards
#ifndef BYTES_H
#define BYTES_H

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

#ifndef LIMITS_H
#define LIMITS_H
#include <limits.h>
#endif

// Estrutura de um buffer (conjunto de bytes)
typedef struct _buffer {
    unsigned char *bytes;
    unsigned length;
} buffer;

buffer new_buffer(int length);
void zero_buffer(buffer input);
void print_buffer(buffer input);
void concat_buffer(buffer *a, buffer b);
bool increment_buffer(buffer input);
bool compare_buffer(buffer a, buffer b);
bool compare_buffer_with_int(buffer a, int b);

// Gerar novo buffer
buffer new_buffer(int length) {
    buffer output;
    output.length = length;
    
    if(length > 0) {
        output.bytes = calloc(sizeof(char), length);
    } else {
        output.bytes = NULL;
    }
    return output;
}

// Zerar um buffer
void zero_buffer(buffer input) {
    int i;
    for(i = 0; i < input.length; i++) {
        input.bytes[i] = 0;
    }
}

// Limpa um buffer da memória
void free_buffer(buffer b) {
    int i;
    free(b.bytes);
}

// Exibir um buffer
void print_buffer(buffer input) {    
    int i;
    for(i = 0; i < input.length; i++) {
        printf("%02x", input.bytes[i]);
    }
}

// Concatenar um buffer "b" em um buffer "a"
void concat_buffer(buffer *a, buffer b) {
    int len = a->length;
    unsigned char *output = malloc(len + b.length);
    
    int i;
    for(i = 0; i < len; i++) {
        output[i] = a->bytes[i];
    }
    for(i = 0; i < b.length; i++) {
        output[i + len] = b.bytes[i];
    }

    free(a->bytes);
    a->bytes = output;
    a->length += b.length;
}

// Incrementar um buffer
bool increment_buffer(buffer input) {
    int i = 0;
    while(input.bytes[i] == UCHAR_MAX) {
        if(i == input.length - 1) return false;
        input.bytes[i] = 0;
        i++;
    }
    input.bytes[i]++;
    return true;
}

// Comparar dois buffers
bool compare_buffer(buffer a, buffer b) {
    int i;
    if(a.length != b.length) return false;
    for(i = 0; i < a.length; i++) {
        if(a.bytes[i] != b.bytes[i]) return false;
    }
    return true;
}

// Comparar um buffer e um número
bool compare_buffer_with_int(buffer a, int b) {
    int i;
    for(i = 0; i < a.length; i++) {
        if(a.bytes[i] != b & (0xff << (i * 8))) return false;
    }
    return true;
}
#endif
