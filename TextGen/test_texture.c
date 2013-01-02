//Texture Test
//Auteur : Julien De Loor
//Déscription : un fichier pour tester rapidement un algorithme de génération de 
//texture avec la winapi
//Compilation migwin32: gcc -o test_texture test_texture.c -mwindows -lopengl32 -lglu32 -std=c99 -W -Wall -Werror

#include <gl/gl.h> //Opengl 3.0
#include <gl/glu.h> //Opengl 3.0 a ne pas confondre avec GLUT qui n'est (je crois) pas open-source (d'aillerus je ne m'en sert pas ici)
#include <windows.h> //WinAPI
#include <stdlib.h> //LibC

#define TEXTURE_SIZE 512 //Texture carrée (doit être une puissance de 2)

GLuint texture_id; //Variable globale en c'est pas bien ^^

GLuint GenerateTexture(const unsigned int size) // la fonction que vous devez coder
{
	//Nos pixels codées en RGBA !
	unsigned char* pixels = (unsigned char*) malloc(size * size * 4);
	unsigned int x=0,y=0;
	for (unsigned int i = 0; i < size * size * 4; i += 4) //Boucle optimisé
	{
	
		//Allez à vous de generer maintenant !
		pixels[i] = 255;
		pixels[i+1] = 0;
		pixels[i+2] = 0;
		pixels[i+3] = 255;
		
		x++;
		if (x > size) 
		{
			y++;
			x=0;
		}
	}
	
	GLuint id;
	glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0,  GL_RGBA,GL_UNSIGNED_BYTE, pixels);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	free(pixels);
	
	return id;
}

void OpenglCode(HDC hDC) //Affichage 
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glPushMatrix();
		glPushMatrix();
			glBegin(GL_QUADS);
				glColor4ub(255,255,255,255);
				glTexCoord2d(0,0);glVertex2f(-1,-1);
				glTexCoord2d(1,0);glVertex2f(1,-1);
				glTexCoord2d(1,1);glVertex2f(1,1);
				glTexCoord2d(0,1);glVertex2f(-1,1);
			glEnd();
		glPopMatrix();
	glPopMatrix();
	SwapBuffers(hDC);//Raffraichissement de la fenetre...
}

LRESULT CALLBACK WinProc(
HWND hwnd,	//Handle de la fenêtre
UINT uMsg,	//Message
WPARAM wParam,	//Paramètre word
LPARAM lParam )	//Paramètre long
 
{	
	switch(uMsg)
	{	
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_CREATE:
			break;
		
		case WM_SIZE: //Changement de taille
			glViewport(0,0,LOWORD(lParam),HIWORD(lParam));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-1.0,1.0, -1.0, 1.0, -1, 1);
			break;

		default:
			return DefWindowProc(
			hwnd,
			uMsg,
			wParam,
			lParam);
	}	
	return 0;
}


//fonction "main" version Windows avec quelques arguments qui ne servent jamais (comme le 2ieme par exemple)
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR argv, INT drawing)
{
	(void) argv; (void) drawing; (void) previous; //3 arguments inutiles merci windows !
	instance = GetModuleHandle( 0 );
    int         done=0;   //Condition d'arrêt de la boucle infinie
    WNDCLASSEX		wc; //Classe de fenetre (Structure à remplir pour générer une fenetre)
	
	memset( &wc, 0, sizeof(WNDCLASSEX) ); //Mise à zero de la structure
	//On complete ensuite les varaibles importantes
	wc.cbSize        = sizeof(WNDCLASSEX); //taille de la structure
    wc.style         = CS_OWNDC;//Très important c'est ce qui permet de mettre opengl dans notre fenetre !
    wc.lpfnWndProc   = WinProc; //Callback de la fenetre : que doit faire la fenetre si on appuis sur une Touche par exemple
    wc.hInstance     = instance; //Instance de la fenetre (inutile mais bon c'est plus propre)
    wc.lpszClassName = "FenetreOpenGL"; //Nom du "type" de fenetre
	
    if( !RegisterClassEx(&wc) ) //On essaye "d'enregistrer" le type de fenetre que l'on souhaite créer
	//En fait cela permet de créer ensuite une "infinité" de fenetre a partir de la même base ce que l'on ne fais pas ici
	//Mais windows considère cela comme obligatoire... donc
	{
		MessageBox(NULL,"Impossible de m'enregistrer auprès de votre systeme d'exploitation !","Echec !",MB_OK | MB_ICONERROR);
        return -1; 
	}
	
	HWND myWindows; //Pointeur vers ma fenetre
	
	//Création de la fenetre
	myWindows = CreateWindowEx( WS_EX_APPWINDOW, "FenetreOpenGL", "Test de texture", WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU,  
                                 15, 15,TEXTURE_SIZE, TEXTURE_SIZE, 0, 0, instance, 0 );
								 
	if (!myWindows) //N'arrive jamais
	{
		MessageBox(NULL,"Impossible de créer une fenetre votre ordinateur est trop pourri !","Echec !",MB_OK | MB_ICONERROR);
		return -2;
	}
	
	HDC hDC; //pointeur de Device Context la clée d'opengl ! (j'ai galéré pour comprendre comment sa marche...)
	hDC = GetDC(myWindows); //On accède au pointeur des pixels de la fenetre (en gros)
	if (!hDC)
	{
		MessageBox(NULL,"Windows ne veut pas me filer mes pixels...","Echec !",MB_OK | MB_ICONERROR);
        return -3; 
	}
	
	unsigned int PixelFormat; //Obligatoire dit comment sont codées les pixels dans l'écran (je crois)
	const PIXELFORMATDESCRIPTOR pfd = //Obligatoire (infos pour la carte graphique)
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		32,             // zbuffer
		0,              // stencil!
		0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};
	
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
	{
		MessageBox(NULL,"Le pixel format ne marche pas !","Echec !",MB_OK | MB_ICONERROR);
        return -4; 
	}
	if (!(SetPixelFormat(hDC,PixelFormat,&pfd)))
	{
		MessageBox(NULL,"Le pixel format ne marche pas !","Echec !",MB_OK | MB_ICONERROR);
        return -5; 
	}
	
	HGLRC hRC; //poiteur de context d'opengl... (pour changer) en gros pour si il y a plusieurs 
	//applis qui utilise opengl en même temps on "reserve" une partie d'opengl pour nous
	if (!(hRC=wglCreateContext(hDC)))
	{
		MessageBox(NULL,"OpenGL y merde !","Echec !",MB_OK | MB_ICONERROR);
        return -6; 
	}
	if(!(wglMakeCurrent(hDC,hRC))) //On indique à opengl que l'on veut utiliser ce contexte pour travailler désormais avec ce device (assez technique)
	{
		MessageBox(NULL,"OpenGL y merde !","Echec !",MB_OK | MB_ICONERROR);
        return -7; 
	}
	
	texture_id = GenerateTexture(TEXTURE_SIZE); //On genere la texture avec votre fonction ^^
	
	MSG msg; //Un "message"	winAPI (une information sous forme de structure que windows envoie au programme)						 
    while( !done ) //Boucle infinie de notre programme
    {
        while( PeekMessage(&msg,0,0,0,PM_REMOVE) ) //Capture des message de l'instance du programme
        {
            if( msg.message == WM_QUIT || (msg.message == WM_KEYDOWN && msg.wParam==VK_ESCAPE)) {
				done=1; break; 
			}
            DispatchMessage( &msg );// inutile ici on a pas de "Callbacks"
        }
        OpenglCode(hDC);
		Sleep(10); //Liberation du CPU
    }
	glDeleteTextures(1,&texture_id);

	wglMakeCurrent( 0, 0 ); //On indique que l'on utilise plus rien (pour pas faire foirer la commande suivante !)
	wglDeleteContext(hRC ); //On libere OpenGL
	ReleaseDC(myWindows, hDC ); //On libere le contexte 
	
    return 0xdead; //Signature de Julien De Loor ;)
}