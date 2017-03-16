#ifndef ADBCONNECT_H
#define ADBCONNECT_H

#include <adbhost/common.h>

typedef struct AdbConnect AdbConnect;

DECLEXPORT AdbConnect *AdbConnect_create(const char *executable);

DECLEXPORT char *AdbConnect_readLine(AdbConnect *self, int timeout, int contTimeout);
DECLEXPORT int AdbConnect_write(AdbConnect *self, const char *str);

DECLEXPORT void AdbConnect_destroy(AdbConnect *self);

#endif

