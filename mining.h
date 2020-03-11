#define DIFICULTY 16

bool mine(unsigned char *seed, unsigned char *gold){
    unsigned char* input = malloc(sizeof(char) * HASH_LENGTH * 2 + 1);
    input[0] = '\0';
    strcat(input, seed);
    strcat(input, gold);

    unsigned char* output = hash(input);
    
    print_bytes(output);

    int i = 0;
    while(!(output[i/8] & (128 >> (i % 8))) && i <= DIFICULTY) i++;
    return i >= DIFICULTY;
}