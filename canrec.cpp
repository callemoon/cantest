#include "canwrapper.h"
#include <stdio.h>

#define RX_MESSAGES 5000000

int canrec(int argc, char **argv)
{
    CanWrapper can;
    struct can_frame msg;
    unsigned int expectedId = 0;
    bool extended, rtr, err;
    int errCode;
    struct timeval timeout;
    int pass = 0;
    int fail = 0;
    int printFrame = 100;
    int receivedMessages = 0;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (argc != 3)
    {
        printf("usage: %s rec [canbus]\r\n", argv[0]);

        return 0;
    }

    if (!can.Init(argv[2], errCode))
    {
        printf("can init failed with code %d (%s)\r\n", err, argv[2]);

        return 0;
    }

    printf("printing every %d th frame\r\n", printFrame);

    while (receivedMessages < RX_MESSAGES)
    {
        if (can.GetMsg(msg, extended, rtr, err, errCode, timeout))
        {
            if(err)
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
                    receivedMessages++;
                    pass++;
                    expectedId += 2;
                }
                else
                {
                    fail++;

                    printf("unexpected frame received id:%d len:%d data:%d %d %d %d %d %d %d %d\r\n",
                        msg.can_id, msg.can_dlc,
                        msg.data[0], msg.data[1], msg.data[2], msg.data[3],
                        msg.data[4], msg.data[5], msg.data[6], msg.data[7]);

                    // resync expectedId
                    expectedId = msg.data[0] + (msg.data[1] << 8) + (msg.data[2] << 16) + (msg.data[3] << 24);
                    expectedId += 2;
                }

                if ((receivedMessages % printFrame) == 0)
                {
                    printf("%d %d\r\n", pass, fail);
                }
            }
        }
    }

    can.Close();

    return 0;
}
