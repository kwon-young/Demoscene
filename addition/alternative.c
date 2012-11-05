#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys\time.h>
#include <string.h>

char* string_add_operator(const char* a, const char* b, const unsigned short base);

int main (void)
{
	printf("test julien 15 + 12 en base 10 = %s\n",string_add_operator("15", "12", 10));
	printf("test julien 15454517 + 12210 en base 10 = %s\n",string_add_operator("15454517", "12210", 10));
	return 0;
}

//Autre test addition by julien

char* string_add_operator(const char* a, const char* b, const unsigned short base)
{
	unsigned int len_a = strlen(a), len_b = strlen(b); //On prend une seule fois la taille
	if (!len_a || !len_b) //Les chaines ne contiennes rien
		return "Error no number";
	unsigned int len_sum = len_a;
	if (len_b > len_a)
		len_sum = len_b;
	if (b[0] + a[0] - 2*'0' + 1 > base - 1) //Possibilité d'un résultat du type nombre à n chiffres -> nombres à n+1 chiffres
		len_sum++;
		
	char* sum = malloc(len_sum+1); //+1 pour le '\0'
	short ret=0; //retenue
	short previous_ret=0; //retenue précédente
	const unsigned int offset_a = len_sum - len_a;
	const unsigned int offset_b = len_sum - len_b;
	unsigned int local_compute = 0;
	
	for (unsigned int i = len_sum + 1; i ;i--)
	{
		
		if (!((int)i+(int)offset_a-1) && !((int)i+(int)offset_b-1))
			local_compute = 0;
		else if (!((int)i+(int)offset_a-1))
			local_compute = b[i+offset_b-2] - 2*'0';
		else if (!((int)i+(int)offset_b-1))
			local_compute = a[i+offset_a-2] - 2*'0';
		else
		{
			if (a[i+offset_a-1] - '0' > base || b[i+offset_b-1] - '0' > base)
			{
				free(sum);
				return "Error base not respected";
			}
			ret = a[i+offset_a-2] + b[i+offset_b-2] - 2*'0' > base-1 ? 1 : 0; //Y a t'il une retenue ?
			if (ret) //si il y a une retenue on addition le "reste"
				local_compute = a[i+offset_a-2] + b[i+offset_b-2] - 2*'0' - base;
			else //si il n'y en à pas on addition tout simplement
				local_compute = a[i+offset_a-2] + b[i+offset_b-2] - 2*'0';
		}
		sum[i-2] = '0' + local_compute + previous_ret;//On addition le terme courant
		previous_ret = ret;
		ret = 0;
	}
	sum[len_sum] = '\0'; //très important !!!!
	return sum;
}