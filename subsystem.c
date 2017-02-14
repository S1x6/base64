#include "subsystem.h"

void copyBit(char a, char* b, int i, int j);
void decode(Input* in);
void breakStr(int* n, int devider, FILE* out);
int readBytes(char* dest, int count, int ign, FILE* file);
char getNumFromB64(char c);

void encode(Input* in)
{
	FILE* fin = fopen(in->input, "rb");
	FILE* fout = fopen(in->output, "wb");
	const char* alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char* buf = calloc(3, sizeof(char));
	int readBytes = 0;
	int n = 0;
	int f = in->string_break;
	while (!feof(fin))
	{
		readBytes = fread_s(buf, sizeof(buf), sizeof(char), 3, fin);
		if (readBytes == 3)
		{
			fprintf(fout, "%c", alph[buf[0] >> 2]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[(((buf[0] << 6) >> 2) | (buf[1] >> 4) ) & 0x3f ]); 
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[(((buf[1] << 4) >> 2) | (buf[2] >> 6) ) & 0x3F ]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[buf[2] & 0x3F]); 
			breakStr(&n, f, fout);
		} 
		else if (readBytes == 2)
		{
			fprintf(fout, "%c", alph[buf[0] >> 2]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[(((buf[0] << 6) >> 2) | (buf[1] >> 4)) & 0x3f]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[((buf[1] << 4) >> 2) & 0x3F]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", '=');
			breakStr(&n, f, fout);
		}
		else if (readBytes == 1)
		{
			fprintf(fout, "%c", alph[buf[0] >> 2]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", alph[((buf[0] << 6) >> 2) & 0x3f]);
			breakStr(&n, f, fout);
			fprintf(fout, "%c", '=');
			breakStr(&n, f, fout);
			fprintf(fout, "%c", '=');
			breakStr(&n, f, fout);
		}
	}
	fclose(fin);
	fclose(fout);
}


void decode(Input* in)
{
	FILE* fin = fopen(in->input, "rb");
	FILE* fout = fopen(in->output, "wb");
	int n = 0, equals = 0;
	char* buf = calloc(4, sizeof(char));
	char toWrite = 0;
	while (!feof(fin))
	{
		equals = readBytes(buf, 4, in->ignore, fin);
		if (buf[0] == -1)
			break;
		switch (equals)
		{
		case 0:
			toWrite = (buf[0] << 2) | (buf[1] >> 4);
			fwrite(&toWrite, sizeof(char), 1, fout);
			toWrite = (buf[1] << 4) | (buf[2] >> 2);
			fwrite(&toWrite, sizeof(char), 1, fout);
			toWrite = ((buf[2] << 6) | buf[3]);
			fwrite(&toWrite, sizeof(char), 1, fout);
			break;
		case 1:
			toWrite = (buf[0] << 2) | (buf[1] >> 4);
			fwrite(&toWrite, sizeof(char), 1, fout);
			toWrite = (buf[1] << 4) | (buf[2] >> 2);
			fwrite(&toWrite, sizeof(char), 1, fout);
			break;
		case 2:
			toWrite = (buf[0] << 2) | (buf[1] >> 4);
			fwrite(&toWrite, sizeof(char), 1, fout);
			break;
		}

	}
}



void breakStr(int* n, int divider, FILE* out)
{
	*n = *n + 1;
	if (divider != 0 && (*n % divider) == 0) {
		fprintf(out, "\n");
		*n = 0;
	}
}

int readBytes(char* dest, int count, int ign, FILE* file)
{
	char tmp = 0;
	int i = 0, e = 0, readBytes = 0;;
	for (; i < count; i++) {
		do {
			readBytes = 0;
			readBytes = fread_s(&tmp, sizeof(tmp), sizeof(char), 1, file);
			if (!readBytes)
				break;
			if (tmp == '=') {
				char* buf = calloc(2,sizeof(char));
				int a = fread(buf, sizeof(char), 2, file);
				if (a == 0) {
					e++;
					break;
				}
				if (a == 1) {
					e++;
					fseek(file, -1, SEEK_CUR);
					break;
				}
				else if (!ign){
					printf("Invalid character found");
					exit(0);
				} else 
					fseek(file, -2, SEEK_CUR);
			}
			if (!ign && getNumFromB64(tmp) == -1 && tmp != '=' && tmp != '\n')
			{
				printf("Invalid character found");
				exit(0);
			}
		} while (getNumFromB64(tmp) == -1);
		dest[i] = getNumFromB64(tmp);
	}
	return e;
}


void copyBit(char a, char* b, int i, int j) //копирует i-ый бит числа а в j-ый бит числа b (справа)
{
	char tmp;
	tmp = a & (1 << (i - 1));
	if (tmp)
		*b |= 1 << (j-1);
	else
		*b &= ~(1 << (j-1));
}

char getNumFromB64(char c)
{
	const char* alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char i = 0;
	for (; i < 64; i++)
	{
		if (alph[i] == c)
			return i;
	}
	return -1;
}