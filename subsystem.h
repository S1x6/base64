
#ifndef _SUBSYSTEM_H
#define _SUBSYSTEM_H
#include "parsing.h"
#include <string.h>

void decode(Input* in);
void breakStr(int* n, int devider, FILE* out);
int readBytes(char* dest, int count, int ign, char * table, FILE* file);
unsigned char * fillTable(const unsigned char * filler);


#endif