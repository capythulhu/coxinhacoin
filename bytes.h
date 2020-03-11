void print_bytes(unsigned char *input){
    int i;
    for(i = 0; i < HASH_LENGTH; i++){
        printf("%02x", input[i]);
    }
}

bool increment_bytes(unsigned char *input){
    int i = 0;
    while(input[i] == UCHAR_MAX){
        if(i == HASH_LENGTH - 1) return false;
        input[i] = 0;
        i++;
    }
    input[i]++;
    return true;
}