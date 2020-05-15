#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#include "bytes.h"
#include "hash.h"
#include "block.h"
#include "transaction.h"
#include "list.h"
#include "hashmap.h"

int main(){
    list *blockchain = new_list();
    hashmap *transactionOuts = new_hashmap();

    srand(time(NULL));
    wallet coinbase = new_wallet();
    wallet w1 = new_wallet();
    wallet w2 = new_wallet();
    
    transaction *g = new_transaction(coinbase.publicKey, w1.publicKey.key, 100, NULL);
    g->signature = encrypt(g->id, coinbase.privateKey);
    g->id = (buffer){{'0'}, 1};
    put_val_on_list(g->outputs, new_transactionout(g->recipientKey, g->value, g->id));
    put_val_on_hashmap(transactionOuts, ((transactionout*)get_val_from_list(g->outputs, 0))->id, get_val_from_list(g->outputs, 0));
}
