#include "subsystem.h"

unsigned char * fillTable(const unsigned char * filler)
{
	unsigned char * table = malloc(sizeof(char) * 256);
	int i = 0;
	int length = strlen(filler);
	for (; i < 255; i++)
		table[i] = 65;
	for (i = 0; i < length; i++) {
		table[filler[i]] = i;
	}
	return table;
}

void encode(Input* in)
{
	FILE* fin = fopen(in->input, "rb");
	FILE* fout = fopen(in->output, "wb");
	const unsigned char* alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char* buf = calloc(3, sizeof(char));
	int readBytes = 0; // ������� ���� ���� �������
	int n = 0; // ������� �������� �����
	int f = in->string_break;
	char toWrite = 0; //��������� ������� ��� �����������

	while (!feof(fin))
	{
		readBytes = fread(buf, sizeof(char), 3, fin);
		if (readBytes == 3)
		{
			fwrite(&alph[(buf[0] >> 2) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout); //�������� ������������� �������� ������ 
			fwrite(&alph[((((buf[0] << 6) >> 2) & 0x30) | ((buf[1] >> 4) & 0x0f)) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&alph[((((buf[1] << 4) >> 2) & 0x3c) | ((buf[2] >> 6) & 0x03)) & 0x3F], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&alph[buf[2] & 0x3F], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
		}
		else if (readBytes == 2)
		{
			fwrite(&alph[(buf[0] >> 2) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&alph[((((buf[0] << 6) >> 2) & 0x30) | ((buf[1] >> 4) & 0x0f)) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&alph[((((buf[1] << 4) >> 2) & 0x3c) | ((buf[2] >> 6) & 0x03)) & 0x3F], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			toWrite = '=';
			fwrite(&toWrite, sizeof(char), 1, fout);
			breakStr(&n, f, fout);
		}
		else if (readBytes == 1)
		{
			fwrite(&alph[(buf[0] >> 2) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&alph[((((buf[0] << 6) >> 2) & 0x30) | ((buf[1] >> 4) & 0x0f)) & 0x3f], sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			toWrite = '=';
			fwrite(&toWrite, sizeof(char), 1, fout);
			breakStr(&n, f, fout);
			fwrite(&toWrite, sizeof(char), 1, fout);
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
	unsigned char* buf = calloc(4, sizeof(char));
	char toWrite = 0;
	unsigned char * table = fillTable("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	while (!feof(fin))
	{
		equals = readBytes(buf, 4, in->ignore, table, fin);
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
	fclose(fin);
	fclose(fout);
}



void breakStr(int* n, int divider, FILE* out) // ����������� ������� �, ���� �� ������ ��������� f, �� ����������� ������� ������
{
	*n = *n + 1;
	if (divider != 0 && (*n % divider) == 0) {
		char a = '\n';
		fwrite(&a, sizeof(char), 1, out);
		*n = 0;
	}
}

int readBytes(char* dest, int count, int ign, char * table, FILE* file)
{
	unsigned char tmp = 0;
	int i = 0, e = 0, readBytes = 0;;
	for (; i < count; i++) {
		do {
			readBytes = 0;
			readBytes = fread_s(&tmp, sizeof(tmp), sizeof(char), 1, file);
			if (!readBytes) //�������� ����� �����
				break;
			if (tmp == '=') { // ���� "=" �� ����������� ��������� �������, ���� �� "=" � �� ����� ����� �� ������
				char* buf = calloc(2, sizeof(char));
				int a = fread(buf, sizeof(char), 2, file); // a - ���������� ��������� ��������
				if (a == 0) {
					e++;
					break;
				}
				if (a == 1) {
					e++;
					fseek(file, -1, SEEK_CUR);
					break;
				}
				else if (!ign) {
					printf("Invalid character found");
					exit(0);
				}
				else
					fseek(file, -2, SEEK_CUR);
			}
			if (!ign && table[tmp] == 65 && tmp != '=')
			{
				printf("Invalid character found");
				exit(0);
			}
		} while (table[tmp] == 65);
		dest[i] = table[tmp];
	}
	return e;
}