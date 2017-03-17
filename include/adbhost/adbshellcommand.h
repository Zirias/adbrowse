#ifndef ADBSHELLCOMMAND_H
#define ADBSHELLCOMMAND_H

struct List;
struct AdbConnect;

#include <adbhost/decl.h>

typedef struct AdbShellCommand AdbShellCommand;

DECLEXPORT AdbShellCommand *AdbShellCommand_create(const char *command, int timeout);
DECLEXPORT void AdbShellCommand_setArgs(AdbShellCommand *self, const List *args);
DECLEXPORT void AdbShellCommand_addArg(AdbShellCommand *self, const char *arg);
DECLEXPORT void AdbShellCommand_addArgs(AdbShellCommand *self, const List *args);
DECLEXPORT void AdbShellCommand_clearArgs(AdbShellCommand *self);
DECLEXPORT int AdbShellCommand_execute(AdbShellCommand *self, struct AdbConnect *connector);
DECLEXPORT const char *AdbShellCommand_lastCommandLine(const AdbShellCommand *self);
DECLEXPORT int AdbShellCommand_lastReturnCode(const AdbShellCommand *self);
DECLEXPORT const struct List *AdbShellCommand_lastOutput(const AdbShellCommand *self);
DECLEXPORT void AdbShellCommand_destroy(AdbShellCommand *self);

#endif
