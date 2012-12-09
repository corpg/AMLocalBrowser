/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#include "serverinfo.h"
#include "server.h"

/* Variable globale qui stocke le nombre de serveurs disponibles*/
int numServeur = 0;

void traiteInfo(const char* infoServeur, int id, AMServer* server){
	int part = 0; /*partie dans infoserveur -< voir fichier header*/
	int nul = 0; /*nombre de '\0' pour determiner les parties*/

	server->id = id; /*initialisation de l'id*/
	infoServeur += (HEADER + strlen("hostname") + 1); /*on saute le header avec le splitnum et hostname pour se trouver directement a la valeur de hostname*/
	
	for(part=1;part<=MAX_PART;infoServeur++){
		if(*infoServeur == '\0'){
			nul++;
		}
		else if(*(infoServeur-1)=='\0' && nul%2==0){
			switch(part++){
				case HOSTNAME: { /*nom du serveur*/
					sprintf(server->nom, "%s", infoServeur);
					break;
				}
				case MAPNAME: { /* nom de la map*/
					sprintf(server->map, "%s", infoServeur);
					break;
				}
				case HOSTPORT: { /*port BF2 du serveur*/
					server->port = atoi(infoServeur);
					break;
				}
				case GAMETYPE: { /*mode de jeu*/
					sprintf(server->gamemode, "%s", infoServeur);
					break;
				}
				case GAMEVARIANT: { /*mod joué (par defaut: bf2)*/
					sprintf(server->modname, "%s", infoServeur);
					break;
				}
				case NUMPLAYERS: { /*nombre de joueur sur le serveur*/
					server->numplayer = (short int)atoi(infoServeur);
					break;
				}
				case MAXPLAYERS: { /*nombre maximum de joueurs sur le serveur*/
					server->maxplayer = (short int)atoi(infoServeur);
					break;
				}
				case PASSWORD: { /*server protegé par mot de passe ? */
					server->password = *infoServeur;
					break;
				}
				case TIMELIMIT: { /*temps limit par round*/
					server->timelimit = atoi(infoServeur);
					break;
				}
				case ROUNDTIME: { /*nombre de round par map*/
					server->nbround = (short int)atoi(infoServeur);
					break;
				}
				case BF2_DEDICATED: { /*serveur dédié ? */
					server->isdedicated = *infoServeur;
					break;
				}
				case BF2_ANTICHEAT: { /*punkbuster ?*/
					server->punkbuster = *infoServeur;
					break;
				}
				case BF2_VOIP: { /* voip ? */
					server->voip = *infoServeur;
					break;
				}
				case BF2_AUTOBALANCE: { /*autobalance ? */
					server->autobalance = *infoServeur;
					break;
				}
				case BF2_FRIENDLYFIRE: { /*friendly fire ?*/
					server->friendlyfire = *infoServeur;
					break;
				}
				case BF2_SPAWNTIME: { /*temps de reaparition*/
					server->spawntime = (short int)atoi(infoServeur);
					break;
				}
				case BF2_SCORELIMIT: { /*score limite par round*/
					server->scorelimit = (unsigned short int)atoi(infoServeur);
					break;
				}
				case BF2_TICKETRATIO: { /*pourcentage de ticket par equipe*/
					server->ticketratio = (unsigned short int)atoi(infoServeur);
					break;
				}
				case BF2_TEAMRATIO: { /*pourcentage de joueur dans la team 1*/
					server->teamratio = (unsigned short int)atoi(infoServeur);
					break;
				}
				case BF2_TEAM1: { /*nom de la team1*/
					sprintf(server->team1, "%s", infoServeur);
					break;
				}
				case BF2_TEAM2: { /*nom de la team2*/
					sprintf(server->team2, "%s", infoServeur);
					break;
				}
				case BF2_BOTS: { /*bots sur la map ?*/
					server->bot = *infoServeur;
					break;
				}
				case BF2_MAPSIZE: { /*taille de la carte (16, 32, ou 64*/
					server->mapsize = (short int)atoi(infoServeur);
					break;
				}
				case BF2_RESERVEDSLOTS: { /*nombre de slots reservés sur le serveur*/
					server->reservedsolts = (short int)atoi(infoServeur);
					break;
				}
				case BF2_COOPBOTRATIO: { /*pourcentage de bots dans la team 1*/
					server->botratio = (short int)atoi(infoServeur);
					break;
				}
				case BF2_COOPBOTCOUNT: { /*nombre de bots sur le serveur*/
					server->nbot = (short int)atoi(infoServeur);
					break;
				}
				case BF2_COOPBOTDIFF: { /*niveau de difficultés des bots */
					server->botdif = (short int)atoi(infoServeur);
					break;
				}
				case BF2_NOVEHICLES: { /*infantry only ?*/
					server->novehicles = *infoServeur;
					break;
				}
			}
		}
	}	
}

AMServer* Listserver(void){
	SOCKET socketBF2; /*socket de la connexion*/
	WSADATA WSAData;
	int* serveurs;
	int i=0; /*indice de parcours*/
	char* infoServer; /* pointeur vers les informations du serveur*/
	IN_ADDR ipserveur;
	AMServer* lstserveurs = NULL;

	if(WSAStartup(MAKEWORD(2,0), &WSAData) != 0) {
		printf("Le socket n'a pu s'initialiser");
		/*erreur d'initialisation*/
		return NULL;
	}
	
	socketBF2 = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP); /*protocole UDP: envoie en mode non connecte.*/
	
	if(socketBF2==INVALID_SOCKET) {
		printf("Impossible d'ouvrir le socket");
		/*le socket n'a pu se créer correctement*/
		return NULL;
	}
	
	/* Socket OK :) */
	printf("Recherche des serveurs en cours...");
	serveurs = getBattlefield2Server(socketBF2);
	numServeur = *serveurs++; /* on recupere le nombre de serveur pour traiter le pointeur plus facilement */
	if(numServeur!=0){
		lstserveurs = calloc(numServeur, sizeof(AMServer)); /* on initialise le tableau qui contiendra tout les serveurs. ==> Possibilité de mise à jour*/
		for(i=0;i<numServeur;i++,serveurs+=6){
			ipserveur.s_addr = *serveurs;
			infoServer = getInfoServer(socketBF2, serveurs);
			
			/*on stocke l'IP et le port gamespy du serveur pour pouvoir rafraichir sans avoir a refaire de brodcast*/
			lstserveurs[i].portgs = *(serveurs+4);
			sprintf(lstserveurs[i].ip, "%s", inet_ntoa(ipserveur));
			
			if(infoServer==NULL){
				lstserveurs[i].id = -1; /*permet de dire que le serveur ne possede pas d'infos*/
			}
			else {
				traiteInfo(infoServer, i, (lstserveurs + i)); /*equivalent traiteInfo(infoServer, i, &lstserveurs[i]);*/
				/*car += sprintf((texte+car), " %s -- IP: %s:%d, Mod: %s, Joueurs: %d/%d\n",lstserveurs[i].nom,lstserveurs[i].ip,lstserveurs[i].port,lstserveurs[i].modname,lstserveurs[i].numplayer,lstserveurs[i].maxplayer);*/
			}
		}
	}
	
	/*on vide le cavhe contenant les serveurs et leur infos*/
	free(serveurs);
	free(infoServer);
	closesocket(socketBF2);

	return lstserveurs;
}
