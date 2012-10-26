#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE; //8 bit = 1 octet
typedef unsigned short WORD; //16 bit = 2 octet
typedef unsigned long DWORD; //32 bit = 4 octet

int main(int argc, char **argv)
{
	char *fichierWav=NULL, octet[8]={};
	FILE * la=NULL;
	int i;

	if (argc == 2)
	{
		fichierWav = argv[1];
	}

	la = fopen(fichierWav, "rb");

	fread(octet,1,4,la);//RIFF

	printf ("%s\n", octet);

	fread (&i, 4, 1, la);//taille du fichier

	printf ("%d\n", i);

	fread(octet,1,4,la);//WAVE

	printf ("%s\n", octet);

	fread(octet,1,4,la);//fmt

	printf ("%s\n", octet);

	fread (&i, 4, 1, la);//PCM

	printf ("%d\n", i);

	fread (&i, 2, 1, la);//audio format : 1

	printf ("%d\n", i);

	fread (&i, 2, 1, la);//canal

	printf ("%d\n", i);

	fread (&i, sizeof(DWORD), 1, la);//sample rate

	printf ("%d\n", i);

	fread (&i, sizeof(DWORD), 1, la);//byte rate

	printf ("%d\n", i);

	fread (&i, sizeof(WORD), 1, la);//byte rate

	printf ("%d\n", i);

	fread (&i, sizeof(WORD), 1, la);//byte per sample

	printf ("%d\n", i);

	fread(octet,1,4,la);//fmt

	printf ("%s\n", octet);

	fread (&i, sizeof(DWORD), 1, la);//byte per sample

	printf ("%d\n", i);

	return 0;

}
