#ifndef KEYGEN_H
#define KEYGEN_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#include "../coin.h"
#include "../utils/bytes.h"
#include "../utils/ints.h"

// Estrutura de uma chave
typedef struct _rsaKey {
    long unsigned key;
    long unsigned n;
} rsaKey;

static void div_l(unsigned long *r, unsigned long *q, long a, long b);
static unsigned long get_d(unsigned long a, unsigned long b);
static long pow_mod_l(unsigned long long a, unsigned long long b, unsigned long n);
unsigned short rand_prime(void);
ibuffer encrypt(buffer m, rsaKey publicKey);
buffer decrypt(ibuffer C, rsaKey privateKey);
unsigned long *get_keys(void);

// Divisão de longs que salva o quociente e o resto
static void div_l(unsigned long *r, unsigned long *q, long a, long b) {
    if(a >= 0) {
        *q = 0;
        *r = a;

        while(*r >= b) {
            *r -= b;
            *q += 1;
        }
    }
}

// Obtém a chave "d" através do algoritmo estendido de Euclides
static unsigned long get_d(unsigned long a, unsigned long b) {
    unsigned long r = a, q, x0 = 1, y0 = 0, x = 0, y = 1, alpha, beta, phi = a;
    int c = 0;
    while(r > 0 && c < MAX_GCD_ITERATIONS) {
        c++;
        div_l(&r, &q, a, b);
        a = b;
        b = r;
        if(r > 0) {
            alpha = x0 - q * x;
            beta = y0 - q * y;

            x0 = x;
            y0 = y;
            x = alpha;
            y = beta;
        }
    }
    if(beta < 0) beta += phi;
    return beta;
}

// Calcula "a^e mod n" através de Exponenciação Quadrática
static long pow_mod_l(unsigned long long a, unsigned long long b, unsigned long n) {
    unsigned long long p = 1;
    while(b != 0) {
        if(b&1) {
            p = (a*p)%n;
            b = (b-1)/2;
        } else {
            b /= 2;
        }
        a = (a*a) % n;
    }
    return p;
}

// Primo aleatório
unsigned short rand_prime(void) {
    static unsigned short min = 1<<sizeof(short);
    unsigned short r, i;
    for(i = 0; i < sizeof(short); i++) r ^= rand() << i * 8;
    r += min;
    short flag = 0;
    while(!flag) {
        flag = 1;
        for(i = 2; i * i <= r; i++) {
            if(r % i == 0) {
                flag = 0;
                break;
            }
        }
        if(!flag)
            if(USHRT_MAX - r <= 2) r = min;
            else r += (r & 1) + 1;
    }
    return r;
}

// Criptografa a mensagem "m" com as chaves públicas "e" e "n"
ibuffer encrypt(buffer m, rsaKey publicKey) {
    int i;
    ibuffer C = new_ibuffer(m.length);
    for(i = 0; i < m.length; i++) {
        C.ints[i] = pow_mod_l(m.bytes[i], publicKey.key, publicKey.n);
    }
    return C;
}

// Descriptografa a mensagem "C" com a chave privada "d" e a chave pública "n"
buffer decrypt(ibuffer C, rsaKey privateKey) {
    int i;
    buffer m = new_buffer(C.length);
    for(i = 0; i < C.length; i++) {
        m.bytes[i] = pow_mod_l(C.ints[i], privateKey.key, privateKey.n);
    }
    return m;
}

// Gera par de chaves
unsigned long *get_keys(void) {
    unsigned short p;
    unsigned short q;
    unsigned int phi;
    unsigned int n;
    unsigned short e;
    unsigned long long d;

    do {
        p = rand_prime();
        q = rand_prime();
        n = p * q;
        phi = (p - 1) * (q - 1);
        e = rand_prime();
        d = get_d(phi, e);
    } while(d * e % phi != 1);
    
    unsigned char a = 'x';
    unsigned long long b = pow_mod_l(a, e, n);
    unsigned char c = pow_mod_l(b, d, n);
    
    unsigned long *keys = malloc(sizeof(long) * 3);
    keys[0] = (unsigned long)e;
    keys[1] = (unsigned long)d;
    keys[2] = (unsigned long)n;
    return keys;
}
#endif