#include <string.h>
#include <stdio.h>


extern int cansend(int argc, char**argv);
extern int canrec(int argc, char**argv);
int canburst(int argc, char **argv);
int canecho(int argc, char **argv);

int main(int argc, char**argv)
{
    if(argc < 2)
    {
        printf("usage: %s [rec|send|burst|echo] [canbus] [channel]\r\n", argv[0]);

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
        printf("usage: %s [rec|send] [canbus] [channel]\r\n", argv[0]);
    }

    return 0;
}
