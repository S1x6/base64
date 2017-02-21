
#include "parsing.h"
#include "subsystem.h"

extern Input* parseArgs(int argc, char* argv[]);
extern void encode(Input* in);
extern void decode(Input* in);

enum RC { OK_RC, WRONG_NUM_OF_PARAMS_RC, WRONG_PARAMS_FORMAT_RC};

int main(int argc, char* argv[])
{
	if (argc < 4 || argc > 7)
	{
		printf("Wrong number of parameters");
		return WRONG_NUM_OF_PARAMS_RC;
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
		return WRONG_PARAMS_FORMAT_RC;
	}
	return OK_RC;
}
