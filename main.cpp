#include <string.h>
#include <stdio.h>

int cansend(int argc, char**argv);
int canrec(int argc, char**argv);
int canburst(int argc, char **argv);
int canecho(int argc, char **argv);

void printUsage(char *appName)
{
    printf("usage: %s [rec|send|burst|echo] [canbus] [channel]\r\n", appName);
}

int main(int argc, char**argv)
{
    if(argc < 2)
    {
        printUsage(argv[0]);

        return 0;
    }

    if(!strcmp(argv[1], "rec"))
    {
        canrec(argc, argv);
    }
    else
    if(!strcmp(argv[1], "send"))
    {
        cansend(argc, argv);
    }
    else
    if(!strcmp(argv[1], "burst"))
    {
        canburst(argc, argv);
    }
    else
    if(!strcmp(argv[1], "echo"))
    {
        canecho(argc, argv);
    }
    else
    {
        printUsage(argv[0]);
    }

    return 0;
}
