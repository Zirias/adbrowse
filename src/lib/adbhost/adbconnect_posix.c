#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>

#include "bufreader.h"

#include <adbhost/adbconnect.h>

#define BUFSIZE 4096
#define P_IN 0
#define P_OUT 1

struct AdbConnect
{
    const char *executable;
    pid_t adbpid;
    int adbin_wr;
    int adbout_rd;
    char inbuf[BUFSIZE];
    char outbuf[BUFSIZE];
    BufReader *outbufReader;
};

static size_t fillOutBuf(int timeout, void *arg)
{
    fd_set readfds;
    struct timeval tv, *tvp;
    FD_ZERO(&readfds);
    FD_SET(outfd, &readfds);

    if (timeout < 0)
    {
        tvp = 0;
    }
    else
    {
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = 1000 * (timeout % 1000);
        tvp = &tv;
    }

    if (select(nfds, &readfds, 0, 0, tvp) <= 0) return 0;

    if (FD_ISSET(outfd, &readfds))
    {
        return read(outfd, outbuf, BUFSIZE);
    }
}

SOEXPORT AdbConnect *AdbConnect_create(const char *executable)
{
    int adbin[2];
    int adbout[2];

    pipe(adbin);
    pipe(adbout);

    pid_t pid = fork();
    if (pid < 0) return 0;
    if (pid)
    {
        close(adbin[P_IN]);
        close(adbout[P_OUT]);

        AdbConnect *self = malloc(sizeof(AdbConnect));
        self->executable = executable;
        self->adbpid = pid;
        self->adbin_wr = adbin[P_OUT];
        self->adbout_rd = adbout[P_IN];
        self->outbufReader = BufReader_create(&(self->inbuf), fillOutBuf, self);
        return self;
    }
    else
    {
        dup2(adbin[0], STDIN_FILENO);
        dup2(adbout[1], STDOUT_FILENO);
        dup2(adbout[1], STDERR_FILENO);
        close(adbin[0]);
        close(adbin[1]);
        close(adbout[0]);
        close(adbout[1]);
        execl(executable, "adb", "shell", 0);
    }
}

SOEXPORT char *AdbConnect_readLine(AdbConnect *self, int timeout, int contTimeout)
{
    return BufReader_readLine(self->outbufReader, timeout, contTimeout);
}

SOEXPORT void AdbConnect_destroy(AdbConnect *self)
{
    BufReader_destroy(self->outbufReader);
    close(self->adbin_wr);
    close(self->adbout_rd);
    kill(self->adbpid, SIGTERM);
    int status;
    waitpid(self->adbpid, &status, 0);
    free(self);
}
