

#include "parsing.h"
#include "subsystem.h"

extern Input* parseArgs(int argc, char* argv[]);
extern void encode(Input* in);
extern void decode(Input* in);

int main(int argc, char* argv[])
{
	if (argc < 4 || argc > 7)
	{
		printf("Wrong number of parameters");
		return 1;
	}
	Input* input = parseArgs(argc, argv);

	if (input)
	{
		switch (input->mode)
		{
		case ENCODE:
			encode(input);
			break;
		case DECODE:
			decode(input);
		}
	}
	else {
		printf("Wrong format of parametrs");
		return 2;
	}
	return 0;
}
