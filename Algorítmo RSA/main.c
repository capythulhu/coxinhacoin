#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Máximo Divisor Comum
int mdc(int a, int b){
    int c;
    while(1){
        c = a%b;
        if(c == 0) return b;
        a = b;
        b = c;
    }
}

// Verifica se é primo
bool is_prime(int n){
    int i;
    for(i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// Primo aleatório
int rand_prime(int min, int max){
    int length = max - min;
    int num = rand() % length + min;
    while(is_prime(num++) == false);
    return num-1;
}

int main(){
    // Randomização
    srand(time(NULL));
    
    printf("p = %i", rand_prime(0, 100));
    printf("\nq = %i", rand_prime(0, 100));

    
    // Números primos iniciais
    double p = 3, q = 7;
    // N da chave pública
    double n = p * q;
    // Totiente de Euler
    double t = (p - 1) * (q - 1);
    // E da chave pública
    double e = 2;
    double i;

    // Cálculo de um E coprimo com T
    while(e < t){
        i = mdc(e, t);
        if(i == 1){
            break;
        } else {
            e++;
        }
    }

    // D da chave privada
    double d;
    // Multiplicando do módulo
    double k = 2;

    d = (1 + (k*t))/e;
    double msg = 12;
    double c = pow(msg, e);
    double m = pow(c, d);
    c = fmod(c, n);
    m = fmod(m, n);

    printf("Mensagem: %lf", msg);
    printf("\np: %lf", p);
    printf("\nq: %lf", q);
    printf("\nn: %lf", n);
    printf("\nt: %lf", t);
    printf("\ne: %lf", e);
    printf("\nd: %lf", d);
    printf("\nc: %lf", c);
    printf("\nm: %lf", m);
    

}