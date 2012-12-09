/**
  * Part of AMLocalBrowser.
  * Source code distributed under the Simple Public License 2.0.
  *
  * Author: Rarenlys (etienne.corpg@gmail.com)
  * Juillet 2008
*/

#include <winsock2.h>
#include <stdio.h>

#include "server.h"

/*--------------------------------------------------------------
	Nom : 		sendPaquet
	Prototype : */
static int sendPaquet(SOCKET sock, u_long s_addrServer, u_short portgs, const char* data, int taille);
/*	But :			Envoyer un paquet de taille octet de donnee data au server d'IP s_addrServer, sur le port portgs.
	Valeur rendue :	1 si l'envoie s'est bien passé, 0 sinon.
	Paramètres :	sock du type SOCKET (ou int), l'adresse IP sous la forme d'unsigned long (utiliser inet_addr()), le port portgs sur lequel on envoie les donnees, les donnees data à envoyer et leur taille.
	Précondition :	Socket valide
	Postcondition :	valeur rendue != 0
--------------------------------------------------------------*/

int* getBattlefield2Server(SOCKET sock){
	SOCKADDR_IN Serveur; /* pour stocket les informations du serveur qui repond */
	int tailleSock = sizeof(Serveur); /*taille de la structure sockadrr de infServer*/
	
	int portgs = 29900; /* port gamespy par defaut */
	int broadcast = 1;
	int state = -1; /*Valeur rendue par la fonction select*/

	char firstRequest[SIZE_BROADCAST_QUERY] = "\xfe\xfd\x02\x00\x00\x00\x00\x00"; /* On initialise la requete: FE FD 02 00 00 00 00 00 */
	char dataRcv[10]; /*buffer de 10o qui contiendra les données recues du serveur*/
	int nserveur = 0; /*Nombre de serveur ayant repondu*/
	int* server; /*pointeur qui servira a parcourir la memoire pour y retrouver les infos des serveurs */
		
	fd_set readfds;
	
	/*Time out de la fonction select*/
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = TIMEOUT * 1000;
	
	/*initialisation de server à 0*/
	server = malloc(1 * sizeof(int));
	*server = nserveur;
	
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)); /* Permet d'autoriser le broadcast*/

	for(;portgs<=29950;portgs++){
		/* On envoie notre requete ... */
		if(sendPaquet(sock, INADDR_BROADCAST, portgs, firstRequest, sizeof(firstRequest))==0){
			/* Erreur lors de l'envoie de la requete ! */
			continue;
		}
		
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		
		while(state!=0){
			state = select((sock+1), &readfds, NULL, NULL, &tv);
		
			if (state == 0){ /*timeout */
				continue;
			}
			else if(state == -1){ /*erreur*/
				continue;
			}
			else { /*si select nous indique que le descripteur contient des infos */
				if(FD_ISSET(sock, &readfds)){ /* si c'est le socket qui nous interesse */
					/*on recupere les infos du paquet recu, on verifie que les donnes recue sont celles attendues et on reealloue la memoire*/
					if(SIZE_BROADCAST_RESPONSE == recvfrom(sock, dataRcv, sizeof(dataRcv), 0, (SOCKADDR *)&Serveur, &tailleSock) && strncmp(dataRcv, "\x05\x00\x00\x00\x00\x00", SIZE_BROADCAST_RESPONSE)==0){
						server=realloc(server, 6);/*on realloue de la memoire pour stocker les infos du nouveau serveur*/
						*(server + (nserveur * 6) +1) = Serveur.sin_addr.s_addr; /* IP du serveur ajoute */
						*(server + (nserveur * 6) + 4 +1) = portgs; /*port gamespy */
						*server = ++nserveur; /* nombre de serveur*/
					}
				}
			}
		}
		state = -1;
	}	
				
	return server;
}


char* getInfoServer(SOCKET sock, const int* stServer){
	char* info = NULL;
	char query[SIZE_INFO_QUERY];
	char data[2048]; /*buffer de 2Ko qui recuperera les infos du serveur */
	int state = -1;
	int retry = NUM_RETRY; /*nombre de possibilite de recommencer a envoyer les donnes*/
	fd_set readfds;
	/*Time out de la fonction select*/
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = TIMEOUT * 1000;
	
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	
	sprintf(query, "\xfe\xfd%c\xe3\x9f\x3d\x01\xff\xff\xff\x01", 0x00); /*requete pour demander les infos du serveur*/
	for(;retry!=0;retry--){
		if(sendPaquet(sock, *stServer, *(stServer + 4), query, sizeof(query))==0){
			/* Impossible d'envoyer la requete */
			continue; /*on recommence*/
		}
		
		state = select((sock+1), &readfds, NULL, NULL, &tv);
		
		if (state == 0){ /*timeout */
			continue; /*on recommence*/
		}
		else if(state == -1){ /*erreur*/
			continue; /*on recommence*/
		}
		else{
			if(FD_ISSET(sock, &readfds)){
				recvfrom(sock, data, sizeof(data), 0, NULL, 0);
				info = data;
				break; /*on s'arrete*/
			}
		}
	}
		
	return info;
}


static int sendPaquet(SOCKET sock, u_long s_addrServer, u_short portgs, const char* data, int taille){
	SOCKADDR_IN server;
	
	server.sin_addr.s_addr = s_addrServer; /* IP du serveur*/
	server.sin_family = AF_INET;
	server.sin_port = htons(portgs); /*port du serveur */
	
	if(taille == sendto(sock, data, taille, 0, (SOCKADDR *)&server, sizeof(server))) { /*Si on a reussit a envoyer toutes les donnes */
		return 1;
	}
	else {
		return 0;
	}
}
