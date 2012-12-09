/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "server.h"
#include "serverInfo.h"

int main(void){
	SOCKET socketBF2; /*socket de la connexion*/
	WSADATA WSAData;
	int* serveurs;
	int numServeur = 0;
	int i=0; /*indice de parcours*/
	char* infoServer; /* pointeur vers les informations du serveur*/
	IN_ADDR ipserveur;
	AMServer* lstserveurs = NULL;

	if(WSAStartup(MAKEWORD(2,0), &WSAData) != 0) {
		printf("Le socket n'a pu s'initialiser");
		/*erreur d'initialisation*/
		return EXIT_FAILURE;
	}
	
	socketBF2 = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP); /*protocole UDP: envoie en mode non connecte.*/
	
	if(socketBF2==INVALID_SOCKET) {
		printf("Impossible d'ouvrir le socket");
		/*le socket n'a pu se créer correctement*/
		return EXIT_FAILURE;
	}
	
	/* Socket OK :) */
	
	printf("Recherche des serveurs en cours...");
	serveurs = getBattlefield2Server(socketBF2);
	printf("\n");
	numServeur = *serveurs++; /* on recupere le nombre de serveur pour traiter le pointeur plus facilement */
	if(numServeur==0){
		printf("Aucun serveur disponible !\n");
	}
	else{
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
				printf(" %s -- IP: %s:%d, Mod: %s, Joueurs: %d/%d\n",lstserveurs[i].nom,lstserveurs[i].ip,lstserveurs[i].port,lstserveurs[i].modname,lstserveurs[i].numplayer,lstserveurs[i].maxplayer);
			}
		}
	}
	
	/*on vide le cavhe contenant les serveurs et leur infos*/
	free(serveurs);
	free(infoServer);
	
	system("Pause");
	return EXIT_SUCCESS;
}
