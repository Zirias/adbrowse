#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <stddef.h>

typedef struct StringBuilder StringBuilder;

StringBuilder *StringBuilder_create(size_t capacity);
void StringBuilder_appendChar(StringBuilder *self, char character);
void StringBuilder_appendStr(StringBuilder *self, const char *string);
void StringBuilder_clear(StringBuilder *self);
size_t StringBuilder_length(const StringBuilder *self);
char *StringBuilder_toString(const StringBuilder *self);
void StringBuilder_destroy(StringBuilder *self);

#endif
