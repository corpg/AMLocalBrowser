/* 
 * Ce fichier contient les prototypes des fonctions qui permettent de communiquer avec les serveurs Battlefield 2
 * Auteur: Rarenlys  (bludiararenlys@yahoo.fr)
 * Touts droits reservés. Juillet 2008 
 * www.airmaps.fr 
 */
 
#ifndef SERVER_H
#define SERVER_H

#define SIZE_BROADCAST_QUERY 8
#define SIZE_BROADCAST_RESPONSE 6
#define SIZE_INFO_QUERY 11
#define NUM_RETRY 3
#define TIMEOUT 100 /*valeur en milliseconde*/

/*
typedef struct {
	u_long ip;
	u_short portgs;
} Localserver;
*/

/*--------------------------------------------------------------
	Nom : 		getBattlefield2Server
	Prototype : */
int* getBattlefield2Server(SOCKET sock);
/*	But :			Lister tous les serveurs Battlefield 2 en faisant
		une requête sur le socket sock en brodcast ("IP: 255.255.255.255") sur les ports
		29900 à 29950.
	Valeur rendue :	Un pointeur vers les informations des serveurs stockés sous forme d'entier:
	IP des serveurs et leur port gamespy sous la forme: [nombredeServeur][IP][IP][IP][IP][port][port][IP][IP][IP][IP][port][port]...
	Paramètres :	sock du type SOCKET (ou int) qu'lon utilise pour envoyer les donnees.
	Précondition :	Socket valide
	Postcondition :	None
--------------------------------------------------------------*/

/*--------------------------------------------------------------
	Nom : 		getInfoServer
	Prototype : */
char* getInfoServer(SOCKET sock, const int* stServer);
/*	But :			Recuperer les informations d'un serveur (map, gamemode, joueurs, ...).
	Valeur rendue :	Pointeur vers les informations.
	Paramètres :	sock du type SOCKET (ou int), et un pointeur vers l'etat du serveur (IP et port sous la forme [IP][IP][IP][IP][port][port])
	Précondition :	Socket valide
	Postcondition :	valeur rendue != NULL
--------------------------------------------------------------*/

#endif
