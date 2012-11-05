#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define TAILLE_LIGNE 100
#define TAILLE_COLONNE 4

int Pgcd(int a, int b, int tableau[TAILLE_LIGNE][TAILLE_COLONNE], int *premier);
int remonteeEuclide (int tableau[TAILLE_LIGNE][TAILLE_COLONNE], int i, int tableauClef[]);
int generateurClef(int tableauClef[]);
int convertisseur(char motDePasse[]);

#endif // MAIN_H_INCLUDED
