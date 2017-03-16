#ifndef ADBCONNECT_H
#define ADBCONNECT_H

typedef struct AdbConnect AdbConnect;

AdbConnect *AdbConnect_create(const char *executable);

char *AdbConnect_readLine(AdbConnect *self, int timeout, int contTimeout);
int AdbConnect_write(AdbConnect *self, const char *str);

void AdbConnect_destroy(AdbConnect *self);

#endif

