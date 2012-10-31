#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE; //8 bit = 1 octet
typedef unsigned short WORD; //16 bit = 2 octet
typedef unsigned long DWORD; //32 bit = 4 octet

int main(int argc, char **argv)
{
	char *fichierWav=NULL, octet[8] = {0};
	FILE * la=NULL, *file=NULL;
	int i;

	if (argc == 2)
	{
		fichierWav = argv[1];
	}

	la = fopen(fichierWav, "rb");

	fread(octet, 1, 4, la);
	fread(&i,4, 1, la);

	printf ("%d\n", i);


	for (int j=0; j<28; j++)
	{
		fread(octet, 1, 1, la);
	}

	file = malloc((i-35)*sizeof(unsigned short));

	fread (octet, 1, 4, la);
	fread (&i, 4, 1, la);
	printf ("%d\n", i/4);

	printf ("%d\n", *file);
	fread(file, 4, 2, la);
	
	printf ("%d\n", *file);
	
//	for (int j=0; j<27563; j++)
//	{
//		printf ("%d\n", file[j]);
//	}

	free(file);
	fclose (la);

	return 0;

}

