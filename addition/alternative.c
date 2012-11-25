#include <stdio.h>
#include "alternative.h"

int main (int argc, char** argv)
{
	//printf("sum de  1001011 + 10011 = %s\n", unreversed_string_operation(binary_reversed_string_operator_add,"1001011", "10011"));
	printf("sum de  678567 + 23 = %s\n", unreversed_string_operation(reversed_string_operator_add,"678567", "23",10));
	printf("sum de DEAD + AC1DE = %s\n", unreversed_string_operation(reversed_string_operator_add,"DEAD", "AC1DE",16));
	if (argc == 3)
	{
		printf("sum de %s + %s  (base 16)= %s\n",argv[1],argv[2], unreversed_string_operation(reversed_string_operator_add,argv[1], argv[2],16));
	}
	return 0;
}

char* binary_reversed_string_operator_add(const char* a,const char *b) //ici les chaines sont inversées de base !!!
{
	if (!a || !b)
		return NULL; //erreur 
	unsigned int len_a = strlen(a), len_b = strlen(b); //On prend une seule fois la taille
	if (!len_a || !len_b) //pas de nombres : erreur
		return NULL;
		
	unsigned int len_sum = len_b> len_a ? len_b+1 : len_a+1; //on cherche la chaine avce le nombres de chiffres le plus long + retenue eventuelles augmentant le nombre total de chiffres
	char* sum = malloc(len_sum+1); //+1 pour le '\0'
	memset(sum,'0',len_sum); //on remplie la chaine de 0
	
	for (unsigned int i = 0; i < len_sum; i++)
	{
		unsigned int temp_sum = sum[i] - '0'; //on fait la somme de la "colone" courante 
		if (i < len_a) temp_sum += a[i] - '0'; //si il y a un chiffre dans le a on l'ajoute 
		if (i < len_b) temp_sum += b[i] - '0'; //si il y a un chiffre dans le b on l'ajoute
		switch (temp_sum)
		{
				case 3: //si la somme des 3 vaut 3 alors on a 11
					sum[i+1] = '1';
					break;
				case 2: //si la somme des 3 vaut 2 alors on a 10
					sum[i+1] = '1';
					sum[i] = '0';
					break;
				case 1: //si la somme des 3 vaut 1 alors on a 01
					sum[i] = '1';
					break;
				case 0: //si la somme des 3 vaut 0 alors on a 00
					break;
				default:
					free(sum);
					return NULL;
		}
	}
	sum[len_sum] = '\0';
	if (sum[len_sum-1] == '0')
	{
		sum[len_sum-1] = '\0';
		sum = realloc(sum,(len_sum));
	}
	return sum;
}

char* get_reversed_string(const char* str)
{
	const unsigned int len = strlen(str);
	char* rstr = malloc(len+1);
	for (unsigned int i = 0 ; i < len; i++) rstr[i] = str[len-1-i];
	rstr[len] = '\0';
	return rstr;
}

void reverse_string(char* str)
{
	const unsigned int len = strlen(str);
	char* cpy = malloc(len+1);
	strcpy(cpy,str);
	for (unsigned int i = 0 ; i < len; i++) str[i] = cpy[len-1-i];
}

char* unreversed_string_operation(rstr_op op ,const char* a, const char *b, unsigned int base)
{
    char *a_r = get_reversed_string(a), *b_r = get_reversed_string(b);
	char *res = op(a_r,b_r,base);
	free(a_r);free(b_r);
	reverse_string(res);
	return res;
}


char* reversed_string_operator_add(const char* a,const char *b,const unsigned int base) //ici les chaines sont inversées de base !!!
{
	if (!a || !b)
		return NULL; //erreur 
	unsigned int len_a = strlen(a), len_b = strlen(b); //On prend une seule fois la taille
	if (!len_a || !len_b) //pas de nombres : erreur
		return NULL;
		
	unsigned int len_sum = len_b> len_a ? len_b+1 : len_a+1; //on cherche la chaine avec le nombres de chiffres le plus long + retenue eventuelles augmentant le nombre total de chiffres
	char* sum = malloc(len_sum+1); //+1 pour le '\0'
	memset(sum,'0',len_sum); //on remplie la chaine de '0'
	
	for (unsigned int i = 0; i < len_sum; i++)
	{
		unsigned int temp_sum = sum[i] - '0'; //on fait la somme de la "colone" courante 
		if (i < len_a) temp_sum += a[i] > '9' ? a[i] - 'A' + 10 : a[i] - '0'; //si il y a un chiffre dans le a on l'ajoute 
		if (i < len_b) temp_sum += b[i] > '9' ? b[i] - 'A' + 10 : b[i] - '0'; //si il y a un chiffre dans le b on l'ajoute
		if (temp_sum < base)
			sum[i] = temp_sum + '0';
		else if (temp_sum < base+base)
		{
			sum[i] = temp_sum - base + '0';
			sum[i+1] += 1;
		}
		else 
		{
			free(sum);
			return NULL;
		}
		if (sum[i] > '9')
			sum[i] += 'A' - '9' - 1 ;

	}
	sum[len_sum] = '\0';
	if (sum[len_sum-1] == '0')
	{
		sum[len_sum-1] = '\0';
		sum = realloc(sum,(len_sum));
	}
	return sum;
}