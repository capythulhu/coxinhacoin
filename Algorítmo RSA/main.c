#include <stdio.h>

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

int main(){
    // Números primos iniciais
    int p = 3, q = 7;
    // N da chave pública
    int n = p * q;
    // Totiente de Euler
    int t = (p - 1) * (q - 1);
    // E da chave pública
    int e = 2;
    int c;

    // Cálculo de um E coprimo com T
    while(e < t){
        c = mdc(e, t);
        if(c == 1){
            break;
        } else {
            e++;
        }
    }
}