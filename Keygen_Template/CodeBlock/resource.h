// Fichier : resource.h					Auteur : Clubber
// Contient les identifiants permettant d'accéder au resources
// du fichier resource.rc dans le code C

//On definis les identifiants selon le type de resource
//    ID_ : non spécifié
//    IDC_ : Control (élement...)
//    IDD_ : Dialog (fenetre)
//    IDI_ : Icon (la première icon est celle qui apparait dans l'explorateur) (.ico)
//    IDB_ : Button (boutons)
//////////////////////////////////////////////////////////////////////////////////////

//Notez que utiliser des nombres directements pose probleme avec la gestion des int/DWORD
//Il est donc important de definir des identifiants ici

#pragma once //protection Header (Vraiment utile ?)
#define  WIN32_LEAN_AND_MEAN //Diminue l'importation


// /!\ Chaque ID doit être unique (le nombre qui le suit ne doit pas apparaitre 2 fois)

#define ID_NUL       0   //Ne doit normalement pas être utilisé....

//Icones
#define IDI_RELEASE  1   //Icone du programme
#define IDI_BDE      2   //Icone du BDE... de la Team quoi ;)

//BITMAP
#define ID_LOGO      3  //Image de votre logo (format BMP)

//Dialogs (Fenetres)
#define IDD_RELEASE  4 //Fenetre Principale
#define IDD_BDE      5 //Fenetre du BDE...
//A vous d'ajouter ou de supprimmer des Dialogs au besoin

//Controls
#define IDC_STATIC  -1 //Controls d'affichage (on se fiche des messages recus)
#define IDC_LOGO    10 //pour que le logo agisse comme un bouton
#define IDC_ENTRE   11 //Champs de texte d'entré (si besoin)
#define IDC_SORTIE  12 //Champ de Texte de sortie (si besoin)
//A vous d'ajouter ou de supprimmer des controles au besoin

//Boutons
#define IDB_ACTION 100 //Bouton d'action (génération etc etc)
#define IDB_QUIT   101 //Bouton pour quiter
#define IDB_BDE    102 //Bouton affichant la Boite de dialogue du BDE
#define IDB_BDEOK  103 //Bouton de confirmation de la boite de dialogue du BDE...
//A vous d'ajouter ou de supprimmer des boutons au besoin

//Voila c'est fini pour ce fichier ! c'était pas si compliqué ^^
