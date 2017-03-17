#include <adbhost/adbconnect.h>
#include <abrutil/string.h>
#include <abrutil/list.h>
#include <stdlib.h>
#include <string.h>

#include <adbhost/adbshellcommand.h>

struct AdbShellCommand
{
    List *command;
    List *args;
    List *output;
    char *cmdline;
    int timeout;
    int contTimeout;
    int rc;
};


static void *escapeArg(const void *in, void *args)
{
    static const char special[] = "\"'(){};$*?";
    (void) args;

    const char *arg = in;
    char *escaped = malloc(2 * strlen(arg) + 2);
    size_t escapedSize = 0;
    const char *c = arg;
    while (*c)
    {
        if (strchr(special, *c)) escaped[escapedSize++] = '\\';
        escaped[escapedSize++] = *c++;
    }
    escaped[escapedSize++] = 0;
    escaped = realloc(escaped, escapedSize);
    return escaped;
}

SOEXPORT AdbShellCommand *AdbShellCommand_create(const char *command, int timeout)
{
    AdbShellCommand *self = malloc(sizeof(AdbShellCommand));
    self->command = List_createStr(1);
    List_append(self->command, String_copy(command));
    self->args = List_createStr(0);
    self->output = List_createStr(0);
    self->timeout = timeout;
    self->contTimeout = timeout > 50 ? 50 : timeout;
    self->cmdline = 0;
    self->rc = 0;
    return self;
}

SOEXPORT void AdbShellCommand_setArgs(AdbShellCommand *self, const List *args)
{
    AdbShellCommand_clearArgs(self);
    AdbShellCommand_addArgs(self, args);
}

SOEXPORT void AdbShellCommand_addArg(AdbShellCommand *self, const char *arg)
{
    List_append(self->args, String_copy(arg));
}

SOEXPORT void AdbShellCommand_addArgs(AdbShellCommand *self, const List *args)
{
    List *oldArgs = self->args;
    self->args = List_concat(oldArgs, args);
    List_destroy(oldArgs);
}

SOEXPORT void AdbShellCommand_clearArgs(AdbShellCommand *self)
{
    List_clear(self->args);
}

SOEXPORT int AdbShellCommand_execute(AdbShellCommand *self, AdbConnect *connector)
{
    static const char getRc[] = "echo $?\n";
    char *line;

    List *cmd = List_concat(self->command, self->args);
    List *escapedCmd = List_transformSameType(cmd, escapeArg, 0);
    List_append(escapedCmd, "\n");
    free(self->cmdline);
    self->cmdline = List_joinStr(escapedCmd, " ");
    List_destroy(escapedCmd);
    List_destroy(cmd);

    List_clear(self->output);
    while ((line = AdbConnect_readLine(connector, 0, 0))) free(line);

    AdbConnect_write(connector, self->cmdline);
    line = AdbConnect_readLine(connector, self->timeout, 0);
    if (line && !strncmp(line, self->cmdline, strlen(self->cmdline) - 1))
    {
        free(line);
        line = AdbConnect_readLine(connector, self->contTimeout, 0);
    }
    while (line)
    {
        List_append(self->output, line);
        line = AdbConnect_readLine(connector, self->contTimeout, 0);
    }

    AdbConnect_write(connector, getRc);
    line = AdbConnect_readLine(connector, self->contTimeout, 0);
    if (line && !strncmp(line, getRc, strlen(self->cmdline) - 1))
    {
        free(line);
        line = AdbConnect_readLine(connector, self->contTimeout, 0);
    }
    if (line)
    {
        self->rc = atoi(line);
    }
    else
    {
        self->rc = -1;
    }
    free(line);

    return (!self->rc);
}

SOEXPORT const char *AdbShellCommand_lastCommandLine(const AdbShellCommand *self)
{
    return self->cmdline;
}

SOEXPORT const List *AdbShellCommand_lastOutput(const AdbShellCommand *self)
{
    return self->output;
}

SOEXPORT int AdbShellCommand_lastReturnCode(const AdbShellCommand *self)
{
    return self->rc;
}

SOEXPORT void AdbShellCommand_destroy(AdbShellCommand *self)
{
    List_destroy(self->output);
    List_destroy(self->args);
    List_destroy(self->command);
    free(self);
}
