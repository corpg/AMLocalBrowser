/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

#include <windows.h> /* API windows */
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <commctrl.h>

#include "serverInfo.h"
#include "browser.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASS fenetre; /*pour creer une classe de fenetre a afficher (http://msdn.microsoft.com/fr-fr/library/ms633576(en-us,VS.85).aspx)*/
	HWND handleFenetre; /* handle de notre fenetre principale*/
	MSG messageAction; /*sert a stocker le message recu par la fenetre. Structure (http://msdn.microsoft.com/en-us/library/ms644958(VS.85).aspx)*/
	
	handlePrincipal = hinstance;

					/****** Creation de la fenetre ******/
	fenetre.style = 0;
	fenetre.lpfnWndProc = activiteFenetre; /*pointeur de fonction pour definir le comportament de la fenetre*/
	fenetre.hInstance = hinstance; /*fenetre principale*/
	fenetre.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(ICO)); /*on charge l'icone de la fenetre*/
	fenetre.hCursor = LoadCursor(NULL, IDC_ARROW); /*pour le curseur*/
	fenetre.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE); /*couleur de la fenetre par defaut*/
	fenetre.lpszMenuName	= "MENUPRINCIPAL";
	fenetre.lpszClassName = "Principale"; /*Nom de notre classe*/
	/* Allocation suplémentaires d'octets*/
	fenetre.cbClsExtra = 0; /*a la cette structure */
	fenetre.cbWndExtra = 0; /* al'instance attribue a la fenetre*/
	
	RegisterClass(&fenetre); /*pour enregistrer notre nouvelle classe de fenetre*/
	handleFenetre = CreateWindow("Principale", "AMLocalBrowser", WS_OVERLAPPEDWINDOW, GetSystemMetrics (SM_CXSCREEN) / 4, GetSystemMetrics (SM_CYSCREEN) / 3, 500, 200, NULL, NULL, hinstance, NULL); /*On cree notre fenetre (http://msdn.microsoft.com/en-us/library/ms632679(VS.85).aspx)*/
	
					/***** Affichage de la fenetre ******/
	ShowWindow(handleFenetre, nCmdShow); /*affiche*/
	UpdateWindow(handleFenetre); /*rafraichit*/
	
		/***** Fenetre Active en attente d'une action sur celle-ci (bouton, ect....) ******/
	while(GetMessage(&messageAction, NULL, 0, 0)){ /*Attend un message pour que la fenetre se "reveille" . Boucle infi sauf si messageAction = WM_QUIT*/
		TranslateMessage(&messageAction); /*traduction*/
        DispatchMessage(&messageAction); /*envoie a la procedure de fenetre*/
	}
	
	return messageAction.wParam; /*Fin de l'application */
	
}

LRESULT CALLBACK activiteFenetre(HWND handle, UINT message, WPARAM wParam, LPARAM lParam){
	static HWND tableau;
	
	switch(message){
		case WM_CREATE:
			{
			tableau = createTableauServeurs(handle);
			/*
			CreateWindow("static", "coucou", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_SUNKEN, 0, 0, 0, 0, handle, NULL, handlePrincipal, NULL); * http://msdn.microsoft.com/en-us/library/bb760773(VS.85).aspx *
			SendMessage(tabServeur, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT), 0);*/
			}
			return 0;
			
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case MENU_CONNECT_IP:
					system("start bf2.exe +modPath mods/am");
					break;
				case MENU_ABOUT:
					MessageBox(handle, "Un browser permettant la connection aux serveur LAN Airmaps.", "...", MB_OK | MB_APPLMODAL);
					break;
				case MENU_SERVER_UPDATE: {
					LVITEM lvi;
					ListView_DeleteAllItems(tableau);
					ZeroMemory(&lvi,sizeof(lvi));
					lvi.mask=LVIF_TEXT;
					lvi.cchTextMax = 256;
					lvi.iItem=0; 
					lvi.iSubItem=0;
					lvi.pszText="Prout";
					ListView_InsertItem(tableau, &lvi);
					Listserver();
				}
					break;
			}
			return 0;
			
		case WM_SIZE:
            MoveWindow(tableau, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return 0;
			
		case WM_DESTROY: /*lorsque l'on detruit la fenetre*/
			PostQuitMessage(0); /*on quitte aussi l'application (envoie WM_QUIT a l'application)*/
			return 0;
			
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}
}


HWND createTableauServeurs(HWND handleFenetre){
	HWND tableau;
	LVITEM lvi;
	LVCOLUMN LvCol;
	char colonne[5][200];
	AMServer* serveurs = Listserver();
	char buffer[256];
	int i;
	
	/*Initialisation du tableau des noms des colonnes*/
	strcpy(colonne[0], NOM);
	strcpy(colonne[1], MAP);
	strcpy(colonne[2], JOUEUR);
	strcpy(colonne[3], GAMEMODE);
	strcpy(colonne[4], MODNAME);
	
	InitCommonControls();
	
	tableau = CreateWindowEx(WS_EX_CLIENTEDGE , WC_LISTVIEW, "",
                  WS_CHILD | WS_VISIBLE | LVS_REPORT, 0, 0, 0, 0, handleFenetre, NULL, handlePrincipal, NULL); /*creation du tableau d'affichage des resultats*/
	
	/*colonnes*/
	ZeroMemory(&LvCol,sizeof(LvCol));
	LvCol.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
	
	for(i=0;i<5;i++){		  
		LvCol.cx=0x60;
		LvCol.pszText=colonne[i];
		ListView_InsertColumn(tableau, i, &LvCol);
	}
	
	
	/*lignes*/
	ZeroMemory(&lvi,sizeof(lvi));
	lvi.mask=LVIF_TEXT;
	lvi.cchTextMax = 256;
	
	for(i=0;i<numServeur;i++){
		/*nom du serveur*/
		lvi.iItem=i; 
		lvi.iSubItem=0;
		lvi.pszText=serveurs[i].nom;
		ListView_InsertItem(tableau, &lvi);

		/*map*/
		lvi.iSubItem=1;
		lvi.pszText=serveurs[i].map;
		ListView_SetItem(tableau, &lvi);
		
		/*nombre de joueurs*/		
		lvi.iSubItem=2;
		sprintf(buffer,"%d/%d",serveurs[i].numplayer,serveurs[i].maxplayer);
		lvi.pszText = buffer;
		ListView_SetItem(tableau, &lvi);
		
		/*mode de jeu*/		
		lvi.iSubItem=3;
		lvi.pszText = serveurs[i].gamemode;
		ListView_SetItem(tableau, &lvi);
		
		/*mod ?*/		
		lvi.iSubItem=4;
		lvi.pszText = serveurs[i].modname;
		ListView_SetItem(tableau, &lvi);
	}
	
	
	
	return tableau;
}
