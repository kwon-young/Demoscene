#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE; //8 bit = 1 octet
typedef unsigned short WORD; //16 bit = 2 octet
typedef unsigned long DWORD; //32 bit = 4 octet

int main(int argc, char **argv)
{
	if (argc >= 2)
	{
		FILE* pFile=NULL; //Pointeur du fichier
		pFile = fopen(argv[1], "rb");
		
		if (!pFile) {
			printf("\nUnable to open : %s\nPress a key to continue\n",argv[1]);
			getc(stdin);
			return 0xdead;
		}
		
		//On va maintenant récuperer différentes valeurs contenues dans le header du wav
		//Header "RIFF"
		DWORD riff_magic; //Le nombre "magique" qui permet d'identifier le fichier comme étant un fichier wav
		DWORD riff_chunk_size; //Je ne sait pas a quoi sa sert...
		DWORD riff_format; //Permet ddentifier l'encodage (ici on veut du wav)
		
		//Sous Header "fmt"
		DWORD fmt_magic; //Identifiant du header (inutile)
		DWORD fmt_chunk_size; // pariel je vois pas l'utilité indique juste la taille du header
		WORD fmt_audio_format; //Format de stockage ?? 
		WORD fmt_num_channels; //Le nombre de cannaux (mono = 1, stéréo = 2)
		DWORD fmt_sample_rate; //Taux d'échantillonage (nombre de valeurs par seconde par cannaux)
		DWORD fmt_byte_rate; //Nombre d'octets à lire par secondes 
		WORD fmt_block_align; //Nombre d'octets lus par position de haut-parleur
		WORD fmt_bits_per_sample; //Précision en nombre de bits de la position du haut-parleur (typiquement 16 bits)
		
		//Sous Header "data"
		DWORD data_magic; //Identifiant du header data (inutile aussi !)
		DWORD data_chunk_size; //Normalement le nombre d'octets des données (la taille qu'il faut allouer)
		
		BYTE* wav_buffer; //Notre tableau qui contiendra les valeurs de position du haut-parleur
		
		//Lisont le fichier
		
		fread (&riff_magic, 4, 1,pFile);
		fread (&riff_chunk_size, 4, 1,pFile);
		fread (&riff_format, 4, 1,pFile);
		
		fread (&fmt_magic, 4, 1,pFile);
		fread (&fmt_chunk_size, 4, 1,pFile);
		fread (&fmt_audio_format, 2, 1,pFile);
		fread (&fmt_num_channels, 2, 1,pFile);
		fread (&fmt_sample_rate, 4, 1,pFile);
		fread (&fmt_byte_rate, 4, 1,pFile);
		fread (&fmt_block_align, 2, 1,pFile);
		fread (&fmt_bits_per_sample, 2, 1,pFile);
		
		fread (&data_magic, 4, 1,pFile);
		fread (&data_chunk_size, 4, 1,pFile);
		
		//Maintenant vérifications des magics, si il sont faux alors on affirme que le fichier n'est pas un fichier wav
		if (riff_magic != 0x52494646 || riff_format != 0x57415645 || fmt_magic != 0x666D7420 || data_magic != 0x64617461)
			printf("Error : this file is not a wav file ! wrong magic\n");
		
		//Une vérification suplémentaire
		else if (fmt_chunk_size != 0x10)
			printf("Warning : fmt header corrupted or not supported by this program ! size : 0x%X\n",fmt_chunk_size);
		
		//On va maintenant afficher les infos sur le header avec des gros printf
		printf("RIFF Header Infos\n");
		printf("Magic : 0x%X (must be 0x52944646)\n",riff_magic);
		printf("Chunck Size : %u (must be size of file - 8)\n",riff_chunk_size);
		printf("File Format ID : 0x%X (must be 0x57415645 for wav format)\n\n",riff_format);
		
		printf("FMT Sub-Header Infos\n");
		printf("Magic : 0x%X (must be 0x666D7420)\n",fmt_magic);
		printf("Chunck Size : 0x%X (must be 0x10)\n",fmt_chunk_size);
		printf("Audio Format : %u\n",fmt_audio_format);
		printf("Num Channels : %u\n",fmt_num_channels);
		printf("Sample Rate : %u\n",fmt_sample_rate);
		printf("Bytes/Sec : %u\n",fmt_byte_rate);
		printf("Byte/Position : %u\n",fmt_block_align);
		printf("Bits/Position/Channels : %u\n\n",fmt_bits_per_sample);
		
		printf("Data Sub-Header Infos\n");
		printf("Magic : 0x%X (must be 0x64617461)\n",data_magic);
		printf("Chunck Size : 0x%X (must be size of file - 4)\n",data_chunk_size);
		
		fclose (pFile);
		printf("\nPress a key to continue\n");
		getc(stdin);
	}
	else
	{
		printf("Usage : wavtest.exe filename\nPress a key to continue\n");
		getc(stdin);
	}
	
	return 0;
}

