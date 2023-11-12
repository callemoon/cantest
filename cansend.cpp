#include "canwrapper.h"
#include <stdio.h>

#define TX_MESSAGES    5000000

int main(int argc, char**argv)
{
	struct can_frame msg;

	int bus;
	int i = 0;
	int err;
	CanWrapper can;

    if(argc != 2)
    {
        printf("usage: cansend canbus channel\r\n");

        return 0;
    }

	can.Init(argv[1], err);

	if(!strcmp(argv[2], "1"))
	{
		i = 1;
	}

	while(i < (TX_MESSAGES * 2))
	{
		msg.can_id = i;
		msg.data[0] = i;
		msg.data[1] = i >> 8;
		msg.data[2] = i >> 16;
		msg.data[3] = i >> 24;
		msg.data[4] = i;
		msg.data[5] = i >> 8;
		msg.data[6] = i >> 16;
		msg.data[7] = i >> 24;

		msg.len = 8;

		if(can.SendMsg(msg, true, false, err))
		{
			i+=2;
		}
		else
		{
			usleep(100);	// buffer full, sleep
		}
	}

	usleep(1000000);

	can.Close();

	return 0;
}