#include "canwrapper.h"
#include <stdio.h>

#define RX_MESSAGES    5000000

int main(int argc, char** argv)
{
	struct can_frame msg;

	int i = 0;
	CanWrapper can;
	bool extended, rtr, err;
	int errCode;
	struct timeval time;
	int pass = 0;
	int fail = 0;

    if(argc != 2)
    {
        printf("usage: canrec canbus\r\n");

        return 0;
    }

	can.Init(argv[1], errCode);

	while(i < (RX_MESSAGES * 2))
	{
		if(can.GetMsg(msg, extended, rtr, err, errCode, time))
		{
			if((msg.can_id == i) &&
            (msg.len == 8) &&
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

				// resync i
				i = msg.data[0] + (msg.data[1] << 8) + (msg.data[2] << 16) + (msg.data[3] << 24);
				i+=2;
			}
			printf("%d %d\r\n", pass, fail);
		}
	}
	
	can.Close();

	return 0;
}