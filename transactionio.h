#ifndef TRANSACTIONIO_H
#define TRANSACTIONIO_H

#include "bytes.h"

// Output de transação
typedef struct _transactionout {
    buffer id;
    unsigned long reciepientKey;
    float value;
    buffer transactionId;
} transactionout;

// Input de transação
typedef struct _transactionin {
    buffer outId;
    transactionout out;
} transctionin;
#endif