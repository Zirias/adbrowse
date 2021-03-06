#include <abrutil/stringbuilder.h>
#include <stdlib.h>

#include "bufreader.h"

struct BufReader
{
    void *buffer;
    size_t len;
    size_t pos;
    StringBuilder *builder;
    BufReader_fillBuffer fillBuffer;
    void *fillBufferArg;
};

SOLOCAL BufReader *BufReader_create(void *buffer,
        BufReader_fillBuffer fillBuffer, void *fillBufferArg)
{
    BufReader *self = malloc(sizeof(BufReader));
    self->buffer = buffer;
    self->len = 0;
    self->pos = 0;
    self->builder = StringBuilder_create(0);
    self->fillBuffer = fillBuffer;
    self->fillBufferArg = fillBufferArg;
    return self;
}

SOLOCAL char *BufReader_readLine(BufReader *self, int timeout, int contTimeout)
{
    if (!contTimeout) contTimeout = 50 > timeout ? timeout : 50;
    for (;;)
    {
        while (self->pos < self->len)
        {
            char next = ((char *)self->buffer)[self->pos++];
            if (!next) next = '\n';
            StringBuilder_appendChar(self->builder, next);
            if (next == '\n')
            {
                char *result = StringBuilder_toString(self->builder);
                StringBuilder_clear(self->builder);
                return result;
            }
        }
        self->pos = 0;
        self->len = self->fillBuffer(timeout, self->fillBufferArg);
        timeout = contTimeout;
        if (!self->len) return 0;
    }
}

SOLOCAL void BufReader_destroy(BufReader *self)
{
    StringBuilder_destroy(self->builder);
    free(self);
}

