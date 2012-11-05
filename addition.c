#include <stdio.h>
#include <stdlib.h>

char *stringAddition (char*, int, char*, int);
char *addition (char *, int , char *, int);

int main (void)
{
	char stringP[]="-2345670987654309234569876543", stringQ[]="-11234567098765433456098765432", *stringSum = NULL;
	
	stringSum = addition(stringP,(int)sizeof(stringP)-1, stringQ, (int)sizeof(stringQ)-1);
	printf ("%s\n", stringSum);

	return 0;
}

char *addition (char *stringP, int tailleStringP, char *stringQ, int tailleStringQ)
{
	char *stringSum = NULL;
	
	int sI = 0;
	if (*stringP == '-')
	{
		if (*stringQ == '-')
		{
			stringSum = stringAddition (stringP+1, tailleStringP-1, stringQ+1, tailleStringQ-1);
			for (sI = 0; *(stringSum+sI) != '\0'; sI++);
			stringSum = (char *)realloc(stringSum, sI*sizeof(char)+sizeof(char));
			sI++;
			while (sI > 0)
			{
				stringSum[sI-1] = stringSum[sI-2];
				sI--;
			}
			*stringSum = '-';
			return stringSum;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

char *stringAddition (char* stringP, int tailleStringP, char*stringQ, int tailleStringQ)
{
	char * bString = (tailleStringP>tailleStringQ)? stringP : stringQ, *lString = (tailleStringP>tailleStringQ)? stringQ : stringP, tempString[2]="0", *stringSum = (char*)malloc(sizeof(char));
	*stringSum='\0';
	int bI =(tailleStringP>tailleStringQ)?tailleStringP : tailleStringQ, lI =(tailleStringP<tailleStringQ)?tailleStringP : tailleStringQ, b = 0, l = 0, sum = 0, tailleStringSum = 1, sI = tailleStringSum, retenue = 0;
	while ((bI > 0) | (bI==0 && (retenue!=0)))
	{
		bI--; lI--;
		
		if (bI < 0) 
		{
			b = 0;
		}
		else
		{
			tempString[0] = bString[bI];
			b = atoi(tempString); 
		}

		if (lI < 0)
		{
			l = 0;
		}
		else
		{
			tempString[0] = lString[lI];
			l = atoi(tempString);
		}

		sum = b + l + retenue;
		stringSum = (char *)realloc (stringSum, tailleStringSum *sizeof(char)+sizeof(char));
		if (stringSum == NULL)
		{
			printf ("error");
			return 0;
		}

		tailleStringSum++; sI++;
		
		while (sI > 1)
		{
			stringSum [sI-1] = stringSum[sI-2];
			sI--;
		}
		sprintf (tempString, "%d", sum%10);
		stringSum[0]=tempString[0];

		retenue = sum / 10;
		
		sI = tailleStringSum;
	}
	return stringSum;
}
