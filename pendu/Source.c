#include <stdio.h>
#include <stdlib.h>
#include "source.h"
#define DICO "dico.txt"
#include <string.h>
#include <time.h>
#include <ctype.h>
int main()
{
	 printf("\t bienvenue sur le jeu du pendu\n");
	 // choisir un mot
	
	 char MotSecret[100] ;
	 strcpy (MotSecret,PIC_WORD());
	 int Vie = 10;
	 char* Reponse = NULL;
	 int Gagner = 0;
	 int victoire = taille_chaine(MotSecret)-1;
	 Reponse = (char*)malloc((taille_chaine(MotSecret))*sizeof(char));
	 init(Reponse,(taille_chaine(MotSecret)));

	 while(victoire != 0  && Vie!= 0)
	 {printf("il vous reste %d vie \n",Vie);
	  printf("Quel est le mot secret ? : %s \n", Reponse);
	  printf("proposition : ");
	  char caractere = LireCaractere();
	  Gagner = verification(caractere,MotSecret,(taille_chaine(MotSecret)),Reponse);
	  if (Gagner > 0)
	  {victoire -= Gagner;
	  }
	  else if  (Gagner == 0)
		  {Vie -= 1;
	      }
	  printf("\n\n"); 
	 }
	 
	 
	 if (Vie == 0)
	 { printf ("c'est un echec monstrueux ");
	  
	 }
	 else printf("c'est gagner mec ");



	 
	 free (Reponse);
     return 0;

}
 

char* PIC_WORD()
{  
   FILE* fichier = NULL;
   fichier = fopen(DICO, "r");
   srand ( time(NULL) );
   int Nb_Word = 0;
   if (fichier != NULL)
   {
     char chaine[100]= "";
     while (fgets(chaine, 100, fichier) != NULL)
	  { Nb_Word +=1;
	  }

   }
   fclose(fichier);
   
   
   int aleatoire = rand() % Nb_Word +1;  
   int ligne = 0;
   char chaine2[100]= "";
   fichier = fopen(DICO, "r");
   while (ligne != aleatoire )
      { fgets(chaine2, 100, fichier);
        ligne +=1;
	
	  }
   char * MonMot = NULL;
   int Taille =(taille_chaine(chaine2));
   MonMot= (char*)malloc(Taille*sizeof(char));
 
   for(int i = 0;chaine2[i] !='\0';i++)
   { MonMot[i]= chaine2[i];
   }
   MonMot[Taille] = '\0';

   fclose(fichier);
   return MonMot;

}

int verification(char caractere,char * mot,int taille,char * Reponse)
{	int j = 0;
	
for( int i = 0; i< taille ; i++)
    
	{ if ( Reponse[i] == caractere )
	  { j = -1;
	   break;
	  }
		
	  else if (mot[i]== caractere)
	   {
		 Reponse[i] = caractere;
		 j +=1;
       }
	    
		 
	 }
	return j;
		 
}



	

char LireCaractere()
{ char caractere = 0 ;
  caractere = toupper(getchar());
  
  while (getchar() != '\n') ;
  return caractere;
  
}

void init(char * t,int Taille)
{ for( int i =0; i < Taille-1 ; i++)
	 { t[i]= '*';
	 }
  t[Taille-1] = '\0';
}


int taille_chaine(const char* t)
{    int somme = 0;
	 for( unsigned i =0; t[i] !='\0' ; i++)
	 { somme ++;
	 }
	 return somme;
}