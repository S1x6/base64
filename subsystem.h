#ifndef _SUBSYSTEM_H
#define _SUBSYSTEM_H
#include "parsing.h"

void decode(Input* in);
void breakStr(int* n, int devider, FILE* out);
int readBytes(char* dest, int count, int ign, FILE* file);
char getNumFromB64(char c);
void fillTable(char * table, const char * filler);


#endif
