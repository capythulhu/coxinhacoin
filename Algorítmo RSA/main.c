#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
// Tamanho máximo da mensagem
#define MSG_LENGTH 100

// Divisão de longs que salva o quociente e o resto
void div_l(long *r, long *q, long a, long b){
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
int gcd_l(long a, long b){
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
long pow_mod_l(long a, long e, long n){
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
bool is_prime(int n){
    int i;
    for(i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// Obtém um "e" coprimo do totiente "phi" através dos primos "p" e "p2" e da chave pública "n"
long get_e(long phi, int p, int p2, long n){
    long i;
    for(i = phi-p; i >= 2; i++){
        if(phi%i != 0 && i != p && i != p2){
            return i;
        }
    }
}

// Primo aleatório de 1 byte
int rand_prime(){
    int num = rand() % (1<<8);
    while(is_prime(num++) == false);
    return num-1;
}

// Criptografa a mensagem "m" com as chaves públicas "e" e "n"
int *encrypt(char *m, long e, long n){
    int i;
    int *C = malloc(MSG_LENGTH * sizeof(long));
    for(i = 0; i < MSG_LENGTH; i++){
        C[i] = pow_mod_l(m[i], e, n);
    }
    return C;
}

// Descriptografa a mensagem "C" com a chave privada "d" e a chave pública "n"
char *decrypt(int *C, long d, long n){
    int i;
    char *m = malloc(MSG_LENGTH * sizeof(char));
    for(i = 0; i < MSG_LENGTH; i++){
        m[i] = pow_mod_l(C[i], d, n);
    }
    return m;
}


int main(){
    // Randomização
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(((long long)tv.tv_sec)*1000 + (tv.tv_usec/1000));

    // Gera primos "p" e "p2"
    int p = rand_prime(), p2 = rand_prime();
    // Chaves públicas "n" e "e", totiente "phi" e chave privada "d"
    long n, phi, d, e;

    // Mensagem original, array de ints para a mensagem
    // criptografada, e array de chars para a mensagem
    // descriptografada
    char m[MSG_LENGTH];
    int *C;
    char *D;

    // Lê a mensagem original do usuário
    scanf("\n");
    fgets(m, MSG_LENGTH, stdin);

    // Calcula "n", "phi", "e" e "d"
    n = p * p2;
    phi = (p - 1)*(p2 - 1);
    e = get_e(phi, p, p2, n);
    d = gcd_l(phi, e);
    printf("Chave publica: (%ld, %ld)\n", e, n);
    printf("Chave privada: (%ld, %ld)\n", d, n);

    // Criptografa a mensagem original e
    // descriptografa a mensagem criptografada
    C = encrypt(m, e, n);
    D = decrypt(C, d, n);

    // Printa a array de inteiros número por número
    printf("Mensagem criptografada: ");
    int i = 0;
    while(m[i] != '\0'){
        printf("%i ", C[i++]);
    }
    // Printa a string descriptografada
    printf("\n");
    printf("Mensagem descriptografada: %s", D);

    // Libera a memória
    free(C);
    free(D);
}