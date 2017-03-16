#include "adbconnect.h"

#include <stdlib.h>
#include <stdio.h>

#define ADB "C:\\Storage\\android\\ADB_Fastboot_Windows\\adb.exe"

int main(void)
{
    char *line;
    puts("[adbhost] ADB Host tests");
    AdbConnect *connect = AdbConnect_create(ADB);
    puts("[adbhost] Connector created");

    while (line = AdbConnect_readLine(connect, 2000)) free(line);

    AdbConnect_write(connect, "unset PS1\n");
    while (line = AdbConnect_readLine(connect, 500)) free(line);

    AdbConnect_write(connect, "su\n");
    while (line = AdbConnect_readLine(connect, 500)) free(line);

    puts("\n[adbhost] getting root directory");
    AdbConnect_write(connect, "ls -l /\n");
    free(AdbConnect_readLine(connect, 500));
    while (line = AdbConnect_readLine(connect, 500))
    {
        fputs(line, stdout);
        free(line);
    }
    
    puts("\n[adbhost] getting system directory");
    AdbConnect_write(connect, "ls -l /system\n");
    free(AdbConnect_readLine(connect, 500));
    while (line = AdbConnect_readLine(connect, 500))
    {
        fputs(line, stdout);
        free(line);
    }

    puts("\n[adbhost] getting data directory");
    AdbConnect_write(connect, "ls -l /data\n");
    free(AdbConnect_readLine(connect, 500));
    while (line = AdbConnect_readLine(connect, 500))
    {
        fputs(line, stdout);
        free(line);
    }

    AdbConnect_write(connect, "exit\n");
    while (line = AdbConnect_readLine(connect, 500)) free(line);
    AdbConnect_destroy(connect);
}

