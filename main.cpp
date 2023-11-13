#include <string.h>


extern int cansend(int argc, char**argv);
extern int canrec(int argc, char**argv);

int main(int argc, char**argv)
{

	if(!strcmp(argv[1], "rec"))
	{
		canrec(argc, argv);
	}
	
	if(!strcmp(argv[1], "send"))
	{
		cansend(argc, argv);
	}

	return 0;
}
