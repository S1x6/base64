#include "parsing.h"

Input* parseArgs(int argc, char* argv[])
{
	Input* res = calloc(1, sizeof(Input));
	int i = 1;
	for (i; i < argc; i++)
	{
		if (!strcmp(argv[i], "-f")) {
			res->string_break = atoi(argv[i + 1]);
			i++;
			continue;
		}
		if (!strcmp(argv[i], "-d") && res->mode == NONE) {
			res->mode = DECODE;
			if (argc >= i + 2) {
				res->input = argv[i + 1];
				res->output = argv[i + 2];
				i += 2;
				continue;
			} else return NULL;
		}
		if (!strcmp(argv[i], "-i")) {
			res->ignore = 1;
			continue;
		}
		if (!strcmp(argv[i], "-e") && res->mode == NONE) {
			res->mode = ENCODE;
			if (argc >= i + 2) {
				res->input = argv[i + 1];
				res->output = argv[i + 2];
				i += 2;
				continue;
			} else return NULL;
		}
		else return NULL;
	}
	return res;
}
