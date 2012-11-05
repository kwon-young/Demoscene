#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main()
{
    int tableauClef[6]={0}, i;
    /*tableau[0] = p
     *tableau[1] = q
     tableau[2] = n
     tableau[3]  = 
     tableau [4] = e
     tableau [5] = d*/

    tableauClef[0] = 45989;
    tableauClef[1] = 46021;

    generateurClef(tableauClef);

    for (i=0; i<6; i++)
    {
        printf("%d ", tableauClef[i]);
    }

    return 0;
}


