#ifndef BUFREADER_H
#define BUFREADER_H

#include <stddef.h>

#include <adbhost/decl.h>

typedef struct BufReader BufReader;

typedef size_t (*BufReader_fillBuffer)(int timeout, void *arg);

BufReader *BufReader_create(void *buffer,
        BufReader_fillBuffer fillBuffer, void *fillBufferArg);

char *BufReader_readLine(BufReader *self, int timeout, int contTimeout);

void BufReader_destroy(BufReader *self);

#endif
