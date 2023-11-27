#include "canwrapper.h"
#include <stdio.h>

#define TX_MESSAGES 5000000

int canburst(int argc, char **argv)
{
    CanWrapper can;
    struct can_frame msg;
    int messagesSent = 0;
    int err;
    int canid = 0;

    if (argc != 4)
    {
        printf("usage: %s send [canbus] [channel]\r\n", argv[0]);

        return 0;
    }

    if (!can.Init(argv[2], err))
    {
        printf("can init failed with code %d (%s)\r\n", err, argv[2]);

        return 0;
    }

    can.DisableEcho();

    if (!strcmp(argv[3], "1"))
    {
        canid = 1;
    }

    while(1)
    {
        messagesSent = 0;

        while (messagesSent < 1000)
        {
            msg.can_id = canid;
            msg.data[0] = canid;
            msg.data[1] = canid >> 8;
            msg.data[2] = canid >> 16;
            msg.data[3] = canid >> 24;
            msg.data[4] = canid;
            msg.data[5] = canid >> 8;
            msg.data[6] = canid >> 16;
            msg.data[7] = canid >> 24;

            msg.can_dlc = 8;

            if (can.SendMsg(msg, true, false, err))
            {
                messagesSent++;
                canid += 2;
            }
            else
            {
                usleep(100); // buffer full, sleep
            }
        }

        usleep(100000);
        msg.can_id = canid;
        msg.data[0] = canid;
        msg.data[1] = canid >> 8;
        msg.data[2] = canid >> 16;
        msg.data[3] = canid >> 24;
        msg.data[4] = canid;
        msg.data[5] = canid >> 8;
        msg.data[6] = canid >> 16;
        msg.data[7] = canid >> 24;
        can.SendMsg(msg, true, false, err);
        canid += 2;
        usleep(100000);
    }
}

int cansend(int argc, char **argv)
{
    CanWrapper can;
    struct can_frame msg;
    int messagesSent = 0;
    int err;
    int canid = 0;

    if (argc != 4)
    {
        printf("usage: %s send [canbus] [channel]\r\n", argv[0]);

        return 0;
    }

    if (!can.Init(argv[2], err))
    {
        printf("can init failed with code %d (%s)\r\n", err, argv[2]);

        return 0;
    }

    can.DisableEcho();

    if (!strcmp(argv[3], "1"))
    {
        canid = 1;
    }

    while (messagesSent < TX_MESSAGES)
    {
        msg.can_id = canid;
        msg.data[0] = canid;
        msg.data[1] = canid >> 8;
        msg.data[2] = canid >> 16;
        msg.data[3] = canid >> 24;
        msg.data[4] = canid;
        msg.data[5] = canid >> 8;
        msg.data[6] = canid >> 16;
        msg.data[7] = canid >> 24;

        msg.can_dlc = 8;

        if (can.SendMsg(msg, true, false, err))
        {
            messagesSent++;
            canid += 2;
        }
        else
        {
            usleep(100); // buffer full, sleep
        }
    }

    usleep(1000000);

    can.Close();

    return 0;
}
