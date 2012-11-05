//Fichier : main.c                      Auteur : Clubber
// Keygen de merde Template : si vous aussi vous vous voulez faire une release ^^

#define  WIN32_LEAN_AND_MEAN //Light importation
#include <windows.h> //import de win32 (tout le monde le poss�de sur son ordinateur)
#include <stdlib.h> //Pour malloc et free (la base ^^)
#include "resource.h" //pour les ID des �l�ments de la fen�tre...
#define CLUBBER_SIGNATURE 0xdead //^^ valeur de retour de la fenetre...

//Allez je fais sa (presque) propre on va d�clarer les fonction avant... normalement il faudrait faire un header sp�cial
//Mais moins il y a de fichiers mieux je me porte....

//Callback de la proc�dure pour la fenetre principale : en gros la fonction qu'appelle la boucle infinie de windows
//une fois la boucle lanc� cette fonction est appell� si un �venement intervient : ex : l'appuis d'un bouton
BOOL CALLBACK ReleaseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//je vous rappelle au passage que le booleen n'est pas d�fini en C avant la norme 99, d'ou les enum dans le fichier window.h

//Callback idem mais cette fois pour la fen�tre du BDE
BOOL CALLBACK BDEProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Fonction d'action je vais l'appeller quand on appuira sur le bouton d'action (ici copie)
void Action(HWND release);
//Les param�tres d�pende de ce que vous avez besoin en fait cette fonction depend de votre programme
//Sachez qu'a partir du HWND qui contient la fen�tre release il est possible de retrouver tout les
//�l�ments contenus dans la fen�tre (que vous cr�er dans ressource.h et resource.rc)


//La fonction main (int main(int argc, char** argv)) drole de t�te n'est-ce pas ? ^^ 
int WINAPI WinMain(HINSTANCE instance,HINSTANCE obselete,LPSTR argv,int drawMode);
//on remercira windows de donner des param�tres obsol�tes en entr�...
//En fait les 2 param�tres int�rr�ssants sont : instance et argv
//D'ailleurs on ne remercie pas windows de ne pas s�parer chaques mot que le faisait tr�s bien la fonction main --#


// Et c'est tout ! ...  toutes nos fonction sont d�clar�s ! on � plus qu'a les cr�er... comme je les d�clar�s je vais en
// profiter pour commencer par WinMain comme ca sa aura servi � qqchose ^^

//Allez hop c'est partie on commence par le commencement le point d'entr�
int WINAPI WinMain(HINSTANCE instance,HINSTANCE obselete,LPSTR argv,int drawMode)
{
	//D�mmarage de la boucle principale... avec notre fenetre principale IDD_RELEASE et 
	//la fonction que windows doit appeler : ReleaseProc
	//Parler de boucle est un peu inexacte puisqu'en v�rit� releaseProc ne sera appeler que si 
	//un �v�nement intervient (appuis sur un bouton etc etc)
	DialogBox(instance,MAKEINTRESOURCE(IDD_RELEASE),NULL,ReleaseProc);
	return CLUBBER_SIGNATURE; //^^ marque du template
} //Et c'est tout ! c'�tait dur ? bien sur que non !


//Bon maintenant bah je propose de continuer dans l'ordre... avec la fonction qui va �tre appeler tt de suite 
//apr�s WinMain -> releaseProc

BOOL CALLBACK ReleaseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon; //Icone de la fenetre ne sert que lors du premier appel
	
	unsigned int Id = LOWORD(wParam); //L'iD de l'envoyeur (qui vient d'envoyer le message ?)
	unsigned int Code = HIWORD(wParam); //Le Code de la WM_COMMAND voir plus bas
	
	//On regarde les messages litt�ralement
	switch (uMsg) {
		//Lors que l'on appelle DialogBox() celle ci appelle releaseProc avec ce message pour dire que l'on  initialise la fenetre
		case WM_INITDIALOG: 
			//Les fichiers resource.rc n'informe pas de la pr�sence d'une icone et du titre li�e � la fen�tre
			//l'initialisation et donc l'endroit id�al pour le faire
			hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_RELEASE)); //on r�cup�re l'icone depuis les resources  
			SendMessage(hWnd, WM_SETICON, ICON_SMALL,(LPARAM) hIcon); //on l'assigne � la fen�tre
			SendMessage(hWnd, WM_SETICON, ICON_BIG,(LPARAM) hIcon); //et � la barre en bas
			SetWindowText(hWnd, TEXT("Super Release")); //On donne un titre � la fen�tre
			break;

		//Message envoy� si l'utilisateur appuis sur la croix rouge
		case WM_CLOSE:
			PostQuitMessage(0); //On "dit" qu'on va quitter
			EndDialog(hWnd, TRUE); //on ferme la boite de dialogue
			break;
	  
		case WM_COMMAND: //Une commande utile  
			switch(Id) //On regarde qui envoie la commande parmis nos �l�ments
			{
				case IDC_LOGO: //Que l'on clique sur le logo ou que l'on clique sur le bouton du BDE
				case IDB_BDE: // on veut le m�me r�sultat : ouvrir la fen�tre du BDE
					if(Code==STN_CLICKED) //On regarde si l'action envoyer conrespond � un clic
						DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BDE),NULL,BDEProc); //si on demarre une nouvelle fenetre
						//Notez que le 3ieme param�tre est la fenetre parent ainsi pour qu'une seule fenetre ne puisse s'ouvrir � la fois on peut
						//utiliser la ligne de code suivante :
						//DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT),hWnd,BDEProc);
						break;
				case IDB_QUIT: //Si on clique sur le bouton quitter
					if(Code==STN_CLICKED)
						SendMessage(hWnd, WM_CLOSE, 0, 0); // c'est comme si on fermer la fenetre avec la croix...
					break;
				case IDB_ACTION: //Sa y est on y est !
					if(Code==STN_CLICKED)
						Action(hWnd); //C'est l� que vous g�n�rerait le code...
					break; 
			}
			break;
			
		default: //Procedure par defaut 
			return FALSE; //le message ne nous � pas servit on renvoie FALSE
	  }

	return TRUE; //le message nous � servit on renvoie TRUE
}

//Un peu plus complexe bon maintenant voyons ce que sa doit faire lorsque j'appuis sur le bouton d'action
void Action(HWND release)
{
	//Ici le but n'est pas de g�n�r� un code mais juste de voir comment obtenir ce qu'il y a marqu� dans le champs d'entr� 
	//puis de le copier dans le champ de sortie...
	//Commencons par r�cupp�rer la longueur de la cha�ne de caract�re 
	//comprise dans le champs d'entr�e
	int length = GetWindowTextLength(GetDlgItem(release,IDC_ENTRE)) + 1;
	if (length <= 2) //pas de texte (possible) ou erreur (impossible)
	{	//On previent l'utilisateur qu'il � oubli� d'entr� qqchose 
		MessageBox(release,"Entre une qqchose dans le champs d'entr� patate !","/!\\ Attention ! /!\\",MB_OK | MB_ICONWARNING | MB_SETFOREGROUND);
		return; //retour pr�matur�
	}
	char* str = malloc(length+1); //Allocation dynamiques Lamers ;) on va maintenant r�cup�r� la cha�ne de caract�res
	GetDlgItemText(release,IDC_ENTRE, str, length); //D'ailleurs c'est ici que sa ce passe
	str[length] = '\0'; //on ajoute le caract�re Nul 
	//Une fois le texte r�cup�r� il ne nous reste plus qu'a l'envoy� dans le champs de sortie
	SetDlgItemText(release,IDC_SORTIE,str);
	free(str); //puis de supprimmer l'allocation dynamique
}

//Bon il ne manque rien ? vous �tes sur 
//Mais si bien sur qu'il manque qqchose la fenetre du BDE !!! la plus importante !!!
//c'est le m�me principe que ReleaseProc donc je ne detail pas ou plutot je fais un petit copier coller

BOOL CALLBACK BDEProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon; //Icone de la fenetre ne sert que lors du premier appel
	
	unsigned int Id = LOWORD(wParam); //L'iD de l'envoyeur (qui vient d'envoyer le message ?)
	unsigned int Code = HIWORD(wParam); //Le Code de la WM_COMMAND voir plus bas
	
	//On regarde les messages litt�ralement
	switch (uMsg) {
		//Lors que l'on appelle DialogBox() celle ci appelle releaseProc avec ce message pour dire que l'on  initialise la fenetre
		case WM_INITDIALOG: 
			//Les fichiers resource.rc n'informe pas de la pr�sence d'une icone et du titre li�e � la fen�tre
			//l'initialisation et donc l'endroit id�al pour le faire
			hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_BDE)); //on r�cup�re l'icone depuis les resources Attention l'icone du BDE c'ete fois ci 
			SendMessage(hWnd, WM_SETICON, ICON_SMALL,(LPARAM) hIcon); //on l'assigne � la fen�tre
			SendMessage(hWnd, WM_SETICON, ICON_BIG,(LPARAM) hIcon); //et � la barre en bas
			SetWindowText(hWnd, TEXT("Super BDE")); //On donne un titre � la fen�tre
			break;

		//Message envoy� si l'utilisateur appuis sur la croix rouge
		case WM_CLOSE:
			EndDialog(hWnd, TRUE); //on ferme la boite de dialogue
			break;
	  
		case WM_COMMAND: //Une commande utile  
			switch(Id) //On regarde qui envoie la commande parmis nos �l�ments
			{
				//Le switch case est un peu inutile ici puisqu'on a que 1 bouton 
				case IDB_BDEOK: //Si on clique sur le bouton quitter
					if(Code==STN_CLICKED)
						SendMessage(hWnd, WM_CLOSE, 0, 0); // c'est comme si on fermer la fenetre avec la croix...
					break;
			}
			break;
			
		default: //Procedure par defaut 
			return FALSE; //le message ne nous � pas servit on renvoie FALSE
	  }
	return TRUE; //le message nous � servit on renvoie TRUE
}
//Et voil� vous pouvez maintenant faire des keygen de merde... pour opengl et la 3D va falloir que vous fouinez sur cette merveille d'internet
//Et oui on ne peut pas tt avoir ces 173 lignes c'est d�j� pas mal ;)