#ifndef ABRUTIL_STRINGBUILDER_H
#define ABRUTIL_STRINGBUILDER_H

#include <stddef.h>

#include <abrutil/decl.h>

typedef struct StringBuilder StringBuilder;

DECLEXPORT StringBuilder *StringBuilder_create(size_t capacity);
DECLEXPORT void StringBuilder_appendChar(StringBuilder *self, char character);
DECLEXPORT void StringBuilder_appendStr(StringBuilder *self, const char *string);
DECLEXPORT void StringBuilder_clear(StringBuilder *self);
DECLEXPORT size_t StringBuilder_length(const StringBuilder *self);
DECLEXPORT char *StringBuilder_toString(const StringBuilder *self);
DECLEXPORT void StringBuilder_destroy(StringBuilder *self);

#endif
