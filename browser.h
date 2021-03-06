/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

#ifndef BROWSER_H
#define BROWSER_H

#define ICO 200

/* Constantes du menu */
#define MENU_SERVER_RAFRAICHIR 10
#define MENU_SERVER_UPDATE 11
#define MENU_CONNECT_IP 12
#define MENU_CONNECT_SERVER 13
#define MEN HELP 14
#define MENU_ABOUT 15

/* Pour l'affichage des resultats*/
#define NOM "Nom"
#define MAP "Map"
#define JOUEUR "Joueur"
#define GAMEMODE "Mode de jeu"
#define MODNAME "Mod"

/*variable globale pour connaitre le handle de l'application dans les sous-fonctions */
HINSTANCE handlePrincipal;

LRESULT CALLBACK activiteFenetre(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

HWND createTableauServeurs(HWND handleFenetre);
void update(HWND handle);

#endif
