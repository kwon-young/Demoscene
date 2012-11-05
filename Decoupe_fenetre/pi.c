//=============================================================================//
//																			   //
//					CAlCULE DE PI              V 1.00                          //
//																			   //
//     Le projet c'est quoi ?												   //
//     le but de ce projet est de tester les performance de ton pc			   //
//																			   //
//                                                       auteur : !3l4ckbird   //
//=============================================================================//

#include <windows.h>
#define ID_B_PARLER 0
#define ID_B_QUITTER 1

HRGN BmpToRgn (HBITMAP hBmp, COLORREF cTransparentColor);

//HBITMAP LoadAnImage(char* FileName); 

HINSTANCE instance;

LRESULT CALLBACK procedureFenetrePrincipale(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE cetteInstance, HINSTANCE precedenteInstance,LPSTR lignesDeCommande, int modeDAffichage)
{
    HWND fenetrePrincipale;
    MSG message;
    WNDCLASS classeFenetre;

    instance = cetteInstance;

    classeFenetre.style = 0;
    classeFenetre.lpfnWndProc = procedureFenetrePrincipale;
    classeFenetre.cbClsExtra = 0;
    classeFenetre.cbWndExtra = 0;
    classeFenetre.hInstance = NULL;
    classeFenetre.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    classeFenetre.hCursor = LoadCursor(NULL, IDC_ARROW);
    classeFenetre.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
    classeFenetre.lpszMenuName =  NULL;
    classeFenetre.lpszClassName = "classeF";

    // On prévoit quand même le cas où ça échoue
    if(!RegisterClass(&classeFenetre)) return FALSE;

    fenetrePrincipale = CreateWindow("classeF", "Ma premiere fenetre winAPI !", WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 900, 900,
                                                   NULL, NULL, cetteInstance, NULL);
    if (!fenetrePrincipale) return FALSE;
	
	//CoInitialize(NULL);
	//char * bitmap = "pi.bmp";
	HBITMAP hvar =(HBITMAP) LoadImage(NULL,"pi.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
    SetWindowRgn(fenetrePrincipale,BmpToRgn(hvar, RGB(255,255,255)),TRUE);
	
    ShowWindow(fenetrePrincipale, modeDAffichage);
    UpdateWindow(fenetrePrincipale);


    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
	
	
	return message.wParam;
}

LRESULT CALLBACK procedureFenetrePrincipale(HWND fenetrePrincipale, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND boutons[2] = {NULL};

    switch (message)
    {
        case WM_CREATE:
                boutons[0] = CreateWindow("BUTTON", "Parler", WS_CHILD | WS_VISIBLE,
        5, 5, 383, 30, fenetrePrincipale, (HMENU)ID_B_PARLER, instance, NULL);
                boutons[1] = CreateWindow("BUTTON", "Quitter", WS_CHILD | WS_VISIBLE,
        5, 45, 383, 30, fenetrePrincipale, (HMENU)ID_B_QUITTER, instance, NULL);
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_B_PARLER:
                    MessageBox(fenetrePrincipale, "Clic", "Bonjour.", MB_ICONINFORMATION);
                    break;

                case ID_B_QUITTER:
                    SendMessage(fenetrePrincipale, WM_DESTROY, 0, 0);
                    break;
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(fenetrePrincipale, message, wParam, lParam);
    }
}





HRGN BmpToRgn (HBITMAP hBmp, COLORREF cTransparentColor)
{
 #define ALLOC_UNIT 100

    int y, x;
    HRGN hRgn = NULL;

    if (!hBmp) return 0; // si bitmap invalide retourne

    BITMAP bm;
    GetObject(hBmp, sizeof(bm), &bm); // met les infos d'en tete du bitmap dans bm
    UINT siz=bm.bmWidth*bm.bmHeight*4; // enregistre la taille des donnes de l'image

    char *lpBmpBits=(char*)LocalAlloc(LMEM_FIXED,siz); // fait de la place pour les bits du bitmap
    GetBitmapBits(hBmp,siz,lpBmpBits); // obtient les bits de l'image dans l'espace qu'on a reservé
    bm.bmBits=lpBmpBits; // complete la strucutre bm avec les bits
    
    while (bm.bmWidthBytes % 4) bm.bmWidthBytes++; // bmWidthBytes doit être divisible par 4

    DWORD maxRects = ALLOC_UNIT;
    HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
    RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
    pData->rdh.iType = RDH_RECTANGLES;
    pData->rdh.nCount = pData->rdh.nRgnSize = 0;
    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

    BYTE *p32 = (BYTE *)bm.bmBits;
    for (y = 0; y < bm.bmHeight; y++) // parcourt toutes les lignes de l'image, de haut en bas
    {
        for (x = 0; x < bm.bmWidth; x++) // parcourt tous les pixels de la ligne, de gauche à droite
        {
            // Recherche une suite continue de pixels non transparents
            int x0 = x;
            DWORD *p = (DWORD *)(p32 + 4*x);
            while (x < bm.bmWidth)
             {
                if ((unsigned)*p==cTransparentColor)
                  break; // ce pixel est transparent
                p++;
                x++;
            }
            if (x > x0)
            {
                // ajoute les pixels (de (x0, y) à (x, y+1)) à la region en tant que rectangle
                if (pData->rdh.nCount >= maxRects)
                {
                   GlobalUnlock(hData);
                   maxRects += ALLOC_UNIT;
                   hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) +
                         (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
                   pData = (RGNDATA *)GlobalLock(hData);
                }
                RECT *pr = (RECT *)&pData->Buffer;
                SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
                if (x0 < pData->rdh.rcBound.left)
                   pData->rdh.rcBound.left = x0;
                if (y < pData->rdh.rcBound.top)
                   pData->rdh.rcBound.top = y;
                if (x > pData->rdh.rcBound.right)
                   pData->rdh.rcBound.right = x;
                if (y+1 > pData->rdh.rcBound.bottom)
                   pData->rdh.rcBound.bottom = y+1;
                pData->rdh.nCount++;

                // Il parait que sous Windows 98, ExtCreateRegion() ne marche pas s'il y a trop de rectangles
                // Pas de panique: on construit la region en deux fois
                if (pData->rdh.nCount == 2000)
                {
                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
                    if (hRgn)
                    {
                       CombineRgn(hRgn, hRgn, h, RGN_OR);
                       DeleteObject(h);
                    }
                    else
                        hRgn = h;
                    pData->rdh.nCount = 0;
                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
                }
            }
         }
        // On passe à la ligne suivante
        p32 += bm.bmWidthBytes;
    }
    // On cree la region
    // (et, s'il y avait plus de 2000 rectangles, on la combine avec celle créee precedemment)
    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
    if (hRgn)
    {
        CombineRgn(hRgn, hRgn, h, RGN_OR);
        DeleteObject(h);
    }
    else hRgn = h;
    LocalFree((HLOCAL)lpBmpBits);
    return hRgn;
}


/*HBITMAP LoadAnImage(char* FileName)
{
	WCHAR wpath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, wpath, MAX_PATH);
	
	IPicture* pPic;
	OleLoadPicturePath(wpath, NULL, NULL, NULL, IID_IPicture,(LPVOID*)&pPic);
	
HBITMAP hPic = NULL;
	pPic->get_Handle((UINT*)&hPic);
	
	HBITMAP hPicRet = (HBITMAP)CopyImage(hPic, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
	
	pPic->Release();
	return hPicRet;
}*/

