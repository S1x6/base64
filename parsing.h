#ifndef _PARSING_H
#define _PARSING_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef enum com { NONE, ENCODE, DECODE } Mode;
typedef struct _Input {
	Mode mode;
	char ignore;
	int string_break;
	char* input;
	char* output;
} Input;


#endif