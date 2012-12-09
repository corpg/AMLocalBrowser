/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

/* Ce fichier contient les prototypes des fonctions et les definition de valeur permettant l'affichage des informations renvoyées par le ou les serveurs */

#ifndef SERVERINFO_H
#define SERVERINFO_H

#define HEADER 16
#define MAX_PART 45
#define HOSTNAME 1
#define MAPNAME 4
#define GAMETYPE 5
#define GAMEVARIANT 6
#define NUMPLAYERS 7
#define MAXPLAYERS 8
#define PASSWORD 10
#define TIMELIMIT 11
#define ROUNDTIME 12
#define HOSTPORT 13
#define BF2_DEDICATED 14
#define BF2_ANTICHEAT 16
#define BF2_VOIP 21
#define BF2_AUTOBALANCE 22
#define BF2_FRIENDLYFIRE 23
#define BF2_SPAWNTIME 27
#define BF2_SCORELIMIT 30
#define BF2_TICKETRATIO 31
#define BF2_TEAMRATIO 32
#define BF2_TEAM1 33
#define BF2_TEAM2 34
#define BF2_BOTS 35
#define BF2_MAPSIZE 37
#define BF2_RESERVEDSLOTS 41
#define BF2_COOPBOTRATIO 42
#define BF2_COOPBOTCOUNT 43
#define BF2_COOPBOTDIFF 44
#define BF2_NOVEHICLES 45

/*Structure AMServer qui stocke les informations d'un serveur pour faciliter leur utilisation*/
typedef struct {
	int id; /*id du serveur pour faciliter l'indexation*/
	char ip[16];
	int port;
	int portgs;
	char nom[21];
	char map[21];
	char gamemode[15];
	short int numplayer;
	short int maxplayer;
	char password;
	int timelimit;
	short int nbround;
	char modname[20];
	char isdedicated;
	char punkbuster;
	char voip;
	char autobalance;
	char friendlyfire;
	short int spawntime;
	short int scorelimit;
	short int ticketratio;
	short int teamratio;
	char team1[5];
	char team2[5];
	char bot;
	short int mapsize;
	short int reservedsolts;
	short int nbot;
	short int botratio;
	short int botdif;
	char novehicles;
} AMServer;

extern int numServeur;

/*--------------------------------------------------------------
	Nom : 		traiteInfo
	Prototype : */
void traiteInfo(const char* infoServeur, int id, AMServer* server);
/*	But :			Pour parcourir les informations renvoyées par un serveur et remplir la structure AMServer.
	Valeur rendue :	None.
	Paramètres :	La chaine de caractères envoyée par le serveur et contenant ses infos, l'id du serveur, un pointeur ers la structure AMServer a remplir.
	Précondition :	infoServeur != NULL
	Postcondition :	None
--------------------------------------------------------------*/

AMServer* Listserver(void);

#endif
