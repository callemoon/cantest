#include "canwrapper.h"
#include <stdio.h>

#define RX_MESSAGES    5000000

int canrec(int argc, char**argv)
{
	struct can_frame msg;

	unsigned int i = 0;
	CanWrapper can;
	bool extended, rtr, err;
	int errCode;
	struct timeval time;
	int pass = 0;
	int fail = 0;

	time.tv_sec = 1;
	time.tv_usec = 0;

	int printFrame = 10;
	int counter = 0;

    if(argc != 3)
    {
        printf("usage: canrec canbus\r\n");

        return 0;
    }

	if(can.Init(argv[2], errCode))
	{
		printf("can init ok %s\r\n", argv[2]);
	}

	printf("printing every %d th frame\r\n", printFrame);

	while(i < (RX_MESSAGES * 2))
	{
		if(can.GetMsg(msg, extended, rtr, err, errCode, time))
		{
			if((msg.can_id == i) &&
			(msg.can_dlc == 8) &&
			(msg.data[0] == (i & 0xFF)) &&
			(msg.data[1] == ((i >> 8) & 0xFF)) &&
			(msg.data[2] == ((i >> 16) & 0xFF)) &&
			(msg.data[3] == ((i >> 24) & 0xFF)) &&
			(msg.data[4] == (i & 0xFF)) &&
			(msg.data[5] == ((i >> 8) & 0xFF)) &&
			(msg.data[6] == ((i >> 16) & 0xFF)) &&
			(msg.data[7] == ((i >> 24) & 0xFF)))
			{
				pass++;
				i+=2;
			}
			else
			{
				fail++;

				printf("wrong frame received id:%d data:%d %d %d %d %d %d %d %d\r\n", msg.can_id,
					msg.data[0], msg.data[1], msg.data[2], msg.data[3],
					msg.data[4], msg.data[5], msg.data[6], msg.data[7]);

				// resync i
				i = msg.data[0] + (msg.data[1] << 8) + (msg.data[2] << 16) + (msg.data[3] << 24);
				i+=2;
			}

			if((counter % printFrame) == 0)
			{
				printf("%d %d\r\n", pass, fail);
			}

			counter++;
		}
	}
	
	can.Close();

	return 0;
}
