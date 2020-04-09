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

#ifndef SYS_TIME_H
#define SYS_TIME_H
#include <sys/time.h>
#endif

#include "bytes.h"
#include "ints.h"

// Estrutura de uma chave
typedef struct _rsaKey {
    long key;
    long n;
} rsaKey;

// Divisão de longs que salva o quociente e o resto
static void div_l(long *r, long *q, long a, long b){
    if(a >= 0){
        *q = 0;
        *r = a;

        while(*r >= b){
            *r -= b;
            *q += 1;
        }
    }
}

// Calcula o Algorítmo de Euclides Estendido para "a" e "b"
static int gcd_l(long a, long b){
    long r = a, q, x0 = 1, y0 = 0, x = 0, y = 1, alpha, beta, phi = a;
    while(r != 0){
       div_l(&r, &q, a, b);
       a = b;
       b = r;
       if(r > 0){
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
static long pow_mod_l(long a, long e, long n){
    long p = 1;
    while(1){
        if(e == 0) return p;
        else if(e&1){
            p = a * p % n;
            e = (e-1)/2;
        } else {
            e /= 2;
        }
        a = (a*a) % n;
    }
}

// Verifica se "n" é primo
static bool is_prime(int n){
    int i;
    for(i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// Obtém um "e" coprimo do totiente "phi" através dos primos "p" e "p2" e da chave pública "n"
static long get_e(long phi, int p, int p2, long n){
    long i;
    for(i = phi-p; i >= 2; i++){
        if(phi%i != 0 && i != p && i != p2){
            return i;
        }
    }
}

// Primo aleatório de 1 byte
static int rand_prime(){
    int num = rand() % (1<<8);
    while(is_prime(num++) == false);
    return num-1;
}

// Criptografa a mensagem "m" com as chaves públicas "e" e "n"
ibuffer encrypt(buffer m, rsaKey publicKey){
    int i;
    ibuffer C = new_ibuffer(m.length);
    for(i = 0; i < m.length; i++){
        C.ints[i] = pow_mod_l(m.bytes[i], publicKey.key, publicKey.n);
    }
    return C;
}

// Descriptografa a mensagem "C" com a chave privada "d" e a chave pública "n"
buffer decrypt(ibuffer C, rsaKey privateKey){
    int i;
    buffer m = new_buffer(C.length);
    for(i = 0; i < C.length; i++){
        m.bytes[i] = pow_mod_l(C.ints[i], privateKey.key, privateKey.n);
    }
    return m;
}

// Gera par de chaves
long *get_keys(){
    // Randomização
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(((long long)tv.tv_sec)*1000 + (tv.tv_usec/1000)); 
    // Gera primos "p" e "p2"
    int p = rand_prime(), p2 = rand_prime();
    // Chaves públicas "n" e "e", totiente "phi" e chave privada "d"
    long n, phi, d, e;
     // Calcula "n", "phi", "e" e "d"
    n = p * p2;
    phi = (p - 1)*(p2 - 1);
    e = get_e(phi, p, p2, n);
    d = gcd_l(phi, e);
    long *keys = malloc(sizeof(long) * 3);
    keys[0] = e;
    keys[1] = d;
    keys[2] = n;
    return keys;
}
#endif