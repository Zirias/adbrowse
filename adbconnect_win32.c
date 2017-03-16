#include "adbconnect.h"
#include "bufreader.h"
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096

struct AdbConnect
{
    const char *executable;
    HANDLE adbin_wr;
    HANDLE adbout_rd;
    char inbuf[BUFSIZE];
    char outbuf[BUFSIZE];
    BufReader *outbufReader;
    OVERLAPPED rdover;
    int reading;
};

static size_t fillOutBuf(int timeout, void *arg)
{
    AdbConnect *self = arg;
    DWORD n;

    if (!self->reading)
    {
        if (ReadFile(self->adbout_rd, self->outbuf, BUFSIZE, &n, &(self->rdover)))
        {
            return (size_t)n;
        }
        else if (GetLastError() == ERROR_IO_PENDING)
        {
            self->reading = 1;
        }
    }
    if (self->reading &&
            WaitForSingleObject(self->rdover.hEvent, timeout) == WAIT_OBJECT_0)
    {
        GetOverlappedResult(self->adbout_rd, &(self->rdover), &n, 0);
        self->reading = 0;
        return (size_t)n;
    }
    else
    {
        return 0;
    }
}

static int mkpipe(HANDLE *read, HANDLE *write,
        SECURITY_ATTRIBUTES *rdattr, SECURITY_ATTRIBUTES *wrattr,
        DWORD rdMode, DWORD wrMode)
{
    char name[MAX_PATH];
    static volatile long serial = 0;
    HANDLE rd, wr;
    DWORD size = 4096;

    snprintf(name, MAX_PATH, "\\\\.\\Pipe\\RemoteExeAnon.%08x.%08x",
            GetCurrentProcessId(), InterlockedIncrement(&serial));
    rd = CreateNamedPipeA(name, PIPE_ACCESS_INBOUND|rdMode,
            PIPE_TYPE_BYTE|PIPE_WAIT, 1, size, size, 120000, rdattr);
    if (rd == INVALID_HANDLE_VALUE) return 0;
    wr = CreateFileA(name, GENERIC_WRITE, 0, wrattr, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL|wrMode, 0);
    if (wr == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        CloseHandle(rd);
        SetLastError(err);
        return 0;
    }

    *read = rd;
    *write = wr;
    return 1;
}

AdbConnect *AdbConnect_create(const char *executable)
{
    AdbConnect *self = calloc(1, sizeof(AdbConnect));
    self->executable = executable;

    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    HANDLE adbin_rd;
    HANDLE adbout_wr;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = 1;
    sa.lpSecurityDescriptor = 0;

    mkpipe(&adbin_rd, &self->adbin_wr, &sa, 0, 0, 0);
    mkpipe(&self->adbout_rd, &adbout_wr, 0, &sa, FILE_FLAG_OVERLAPPED, 0);
    self->rdover.hEvent = CreateEvent(0, 1, 1, 0);

    strcpy(self->outbuf, self->executable);
    strcat(self->outbuf, " shell");

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = adbout_wr;
    si.hStdOutput = adbout_wr;
    si.hStdInput = adbin_rd;
    si.dwFlags |= STARTF_USESTDHANDLES;
    memset(&pi, 0, sizeof(pi));
    CreateProcess(0, self->outbuf, 0, 0, 1, 0, 0, 0, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(adbin_rd);
    CloseHandle(adbout_wr);

    self->outbufReader = BufReader_create(self->outbuf, fillOutBuf, self);
    return self;
}

char *AdbConnect_readLine(AdbConnect *self, int timeout)
{
    return BufReader_readLine(self->outbufReader, timeout);
}

int AdbConnect_write(AdbConnect *self, const char *str)
{
    char c;
    size_t pos = 0;
    do
    {
        c = *str++;
        self->inbuf[pos++] = c;
        if (pos == BUFSIZE)
        {
            WriteFile(self->adbin_wr, self->inbuf, pos, 0, 0);
            pos = 0;
        }
    } while (c);
    if (pos) WriteFile(self->adbin_wr, self->inbuf, pos, 0, 0);
}

void AdbConnect_destroy(AdbConnect *self)
{
    BufReader_destroy(self->outbufReader);
    CloseHandle(self->adbin_wr);
    CloseHandle(self->adbout_rd);
    CloseHandle(self->rdover.hEvent);
    free(self);
}

