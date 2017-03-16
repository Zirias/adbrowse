#include <adbhost/adbconnect.h>

#include <stdlib.h>
#include <stdio.h>

#define ADB "C:\\Storage\\android\\ADB_Fastboot_Windows\\adb.exe"

int main(int argc, char **argv)
{
    char *line;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <path to adb>\n", argv[0]);
        return EXIT_FAILURE;
    }

    puts("[adbhost] ADB Host tests");
    AdbConnect *connect = AdbConnect_create(argv[1]);
    puts("[adbhost] Connector created");

    while ((line = AdbConnect_readLine(connect, 4000, 500))) free(line);

    AdbConnect_write(connect, "unset PS1\n");
    while ((line = AdbConnect_readLine(connect, 500, 0))) free(line);

    AdbConnect_write(connect, "su\n");
    while ((line = AdbConnect_readLine(connect, 500, 0))) free(line);

    puts("\n[adbhost] getting root directory");
    AdbConnect_write(connect, "ls -l /\n");
    free(AdbConnect_readLine(connect, 500, 0));
    while ((line = AdbConnect_readLine(connect, 500, 0)))
    {
        fputs(line, stdout);
        free(line);
    }
    
    puts("\n[adbhost] getting system directory");
    AdbConnect_write(connect, "ls -l /system\n");
    free(AdbConnect_readLine(connect, 500, 0));
    while ((line = AdbConnect_readLine(connect, 500, 0)))
    {
        fputs(line, stdout);
        free(line);
    }

    puts("\n[adbhost] getting data directory");
    AdbConnect_write(connect, "ls -l /data\n");
    free(AdbConnect_readLine(connect, 500, 0));
    while ((line = AdbConnect_readLine(connect, 500, 0)))
    {
        fputs(line, stdout);
        free(line);
    }

    AdbConnect_write(connect, "exit\n");
    while ((line = AdbConnect_readLine(connect, 500, 0))) free(line);
    AdbConnect_destroy(connect);

    return EXIT_SUCCESS;
}

