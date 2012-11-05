#include "main.h"
#include "math.h"

int generateurClef(int tableauClef[])
{
    int tableau[TAILLE_LIGNE][TAILLE_COLONNE]={0, 0, 0, 0}, premier=0, *pointeurSurPremier=&premier, i=2;

    tableauClef[2] = tableauClef[0] * tableauClef[1];
    tableauClef[3] = (tableauClef[0] - 1) * (tableauClef[1] - 1);

    while (tableauClef[5] <= 0)
    {
        Pgcd(tableauClef[3], i, tableau, pointeurSurPremier);
        i++;
        if (premier == 1)
        {
            i--;
            tableauClef[4] = i;

            tableauClef[5] = remonteeEuclide(tableau, Pgcd(tableauClef[4], tableauClef[3], tableau, pointeurSurPremier), tableauClef);
            i++;
        }
    }
    return 1;
}
int Pgcd(int a, int b, int tableau[TAILLE_LIGNE][TAILLE_COLONNE], int *pointeurSurPremier)
{
    int r = 2, q = 0, i=0;

    while (r > 1)
    {
        if (i == 0)
        {
            tableau[i][0]=a;
            tableau[i][1]=b;
        }
        else
        {
            a = b;
            b = r;
            tableau[i][0]=a;
            tableau[i][1]=b;
        }
        r = a%b;
        q = a/b;
        tableau[i][2]=q;
        tableau[i][3]=r;
        i++;

    }
    if (r == 1)
    {
        *pointeurSurPremier = 1;
    }
    return i;
}

int remonteeEuclide (int tableau[TAILLE_LIGNE][TAILLE_COLONNE], int i, int tableauClef[])
{
    int a = 0, b = 0, j = 0, pair = 0, k=0;


    for (j = i - 1;j > 0; j--)
    {
        if (j == i - 1)
        {
            a = tableau[j][2] * tableau[j-1][2] + 1;
            b = tableau[j][2];
            pair=1;
        }
        else if (pair == 1)
        {
            b = b + tableau[j-1][2] * a;
            pair=0;
        }
        else if (pair == 0)
        {
            a = a + tableau[j-1][2] * b;
            pair=1;
        }
    }
    a = fabs(a);
    b=fabs(b);

    if (a > b)
    {
        if (tableau[0][0] * a - tableau[0][1] * b == 1)
        {
            return a;
        }
        else if (tableau[0][0] * a - tableau[0][1] * (-b) == 1)
        {
            return a;
        }
        else
        {
            return -a;
        }
    }
    else
    {
        if (tableau[0][0] * b - tableau[0][1] * a == 1)
        {
            return b;
        }
        else if (tableau[0][0] * b - tableau[0][1] * (-a) == 1)
        {
            return b;
        }
        else
        {
            return -b;
        }
    }

}

