#ifndef COIN_H
#define COIN_H

// Máximo de casas após a vírgula de um valor
// de uma transação
#define DECIMAL_PLACES 6
// Tamanho em bytes da hash
#define HASH_LENGTH 8
// Quantidade de bits em sequência iguais a 0
// que uma hash deve começar com para que haja
// a mineração do bloco
#define DIFFICULTY 4
// Tamanho em bits de um primo gerado
#define PRIME_LENGTH 8
// Máximo de iterações do Algoritmo de Euclides
// Estendido para que se pule para o método de
// tentativa e erro
#define MAX_GCD_ITERATIONS 10
// Transação mínima
#define MIN_TRANSACTION 0.000001
#endif