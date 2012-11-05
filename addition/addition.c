#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys\time.h>

struct timeval t0;
void startChronos(void){
  gettimeofday(&t0,NULL);
  
}

unsigned long stopChronos(void){ 
  struct timeval t1;
  unsigned long dt;
  gettimeofday(&t1,NULL);
  dt=(t1.tv_sec-t0.tv_sec)*1000000 + (t1.tv_usec-t0.tv_usec);
  return dt;
}

char *stringAddition (char*, int, char*, int);
char *addition (char *, int , char *, int);

int main (void)
{
	char stringP[]="-682941969615793013305004643260597999245126121596742131345503419983820808782479337897279487086105391708698075815841348586518237841983797762386820655458424818961583911653533213999815528023956817565465132694969605740597312314035924910789787145849340254172972562106764061122754436407736899335523348532478026765506795913528511130955927979529648657380551392858241260116607303046062556510579706529702796386904271941911924124344229628883562115202151348273296110017833453211608280156124786140743755912522577609272163469049312782840504808375526841498413904015426695619628558042905365113264164670430883218328841811785974044946768920897944742795982548163849450097710974623862070136381630306521491478582236663925643017946973762284290567645973025140333476814891", stringQ[]="-682941969615793013305004643260597999245126121596742131345503419983820808782479337897279487086105391708698075815841348586518237841983797762386820655458424818961583911653533213999815528023956817565465132694969605740597312314035924910789787145849340254172972562106764061122754436407736899335523348532478026765506795913528511130955927979529648657380551392858241260116607303046062556510579706529702796386904271941911924124344229628883562115202151348273296110017833453211608280156124786140743755912522577609272163469049312782840504808375526841498413904015426695619628558042905365113264164670430883218328841811785974044946768920897944742795982548163849450097710974623862070136381630306521491478582236663925643017946973762284290567645973025140333476814891", *stringSum = NULL;
	
	unsigned long tmps = 0;

	startChronos();
	stringSum = addition(stringP,(int)sizeof(stringP)-1, stringQ, (int)sizeof(stringQ)-1);
	tmps = stopChronos();
	printf ("%s\n%lu", stringSum, tmps);

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
			
			return ;
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
