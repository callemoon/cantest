#include "canwrapper.h"
#include <stdio.h>

#define TX_MESSAGES 5000000


int canecho(int argc, char **argv)
{
    CanWrapper can;
    struct can_frame msg;
    int messagesSent = 0;
    int canid;
    bool extended, rtr;
    int errCode;
    bool err2;
    struct timeval timeout;
    unsigned int expectedId;
    bool sendRequest;
    int failures = 0;
    int pass = 0;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    if (argc != 4)
    {
        printf("usage: %s echo [canbus] [channel]\r\n", argv[0]);

        return 0;
    }

    if (!can.Init(argv[2], errCode))
    {
        printf("can init failed with code %d (%s)\r\n", errCode, argv[2]);

        return 0;
    }

    can.DisableEcho();

    if (!strcmp(argv[3], "0"))
    {
        canid = 0;
        sendRequest = 1;
        expectedId = 0;
    }
    else
    {
        canid = 1;  
        sendRequest = 0; 
        expectedId = 0; 
    }

    while(1)
    {
        // send a request frame
        if(sendRequest)
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

            expectedId = (canid + 1);

            if (can.SendMsg(msg, true, false, errCode))
            {
                messagesSent++;
                canid += 2;
            }
            else
            {
                printf("could not send message\r\n");
            }
        }

        // wait for answer
        if (can.GetMsg(msg, extended, rtr, err2, errCode, timeout))
        {
            if(err2)
            {
                printf("error frame received\r\n");
            }
            else
            {
                if ((msg.can_id == expectedId) &&
                    (msg.can_dlc == 8) &&
                    (msg.data[0] == (expectedId & 0xFF)) &&
                    (msg.data[1] == ((expectedId >> 8) & 0xFF)) &&
                    (msg.data[2] == ((expectedId >> 16) & 0xFF)) &&
                    (msg.data[3] == ((expectedId >> 24) & 0xFF)) &&
                    (msg.data[4] == (expectedId & 0xFF)) &&
                    (msg.data[5] == ((expectedId >> 8) & 0xFF)) &&
                    (msg.data[6] == ((expectedId >> 16) & 0xFF)) &&
                    (msg.data[7] == ((expectedId >> 24) & 0xFF)))
                {
                    pass++;
                }
                else
                {
                    failures++;
                    printf("unexpected frame\n");
                }

                printf("%d %d\n", pass, failures);
            }
            sendRequest = 1;
        }
    }    
}

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
