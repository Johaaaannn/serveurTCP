/*	ServeurTcp.cpp    */

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>  /* Declaration des structures associees aux Sockets*/
#include <sys/socket.h>  /* Sockaddr, fonctions associees aux Sockets	*/

#include "serveurtcp.h"

ServeurTcp::ServeurTcp(int port)
{
if( (canal_demande=socket( AF_INET, SOCK_STREAM, 0) )<0)
	{
	perror("Erreur socket() : ");
	//exit(2);
	}

/* Definition de l'adresse   */
nom_producteur.sin_family = AF_INET;
nom_producteur.sin_addr.s_addr=htonl(INADDR_ANY);
nom_producteur.sin_port= htons(port);

/* Association de l'adresse au socket */
if ( bind(canal_demande, (struct sockaddr *)&nom_producteur, sizeof(nom_producteur)))
	{
	perror("Erreur de bind() : ");
	//exit(3);
	}

//printf ("num port reseau : %d\n", ntohs(nom_producteur.sin_port));
//printf ("num port serveur : %d\n", nom_producteur.sin_port);

/* Fixer la longueur de la file des appels a 5  */
listen(canal_demande, 5);

}

ServeurTcp::~ServeurTcp()
{
close (canal_demande);
}

int ServeurTcp::accepter()
{
canal_dedie = accept(canal_demande, 0, 0);
return (canal_dedie);
}

int ServeurTcp::fermer()
{
return(close (canal_dedie));
}

int ServeurTcp::lire(char* chaine)
{
int tailleTrame;

	tailleTrame = recv(canal_dedie, chaine, sizeof(chaine),0) ;
	return(tailleTrame);
}

int ServeurTcp::ecrire(char* chaine, int tailleTrame)
{
	return(send(canal_dedie, chaine, tailleTrame,0));
}

int ServeurTcp::envoyerTrame(unsigned char* trame)
{
	//unsigned char req[5] ={ 0xf2, 0x03, 0x0c, 0xca, 0x6c};
	char* requete = reinterpret_cast<char*>(trame);
	// Fin Section Critique
	return ecrire(requete, requete[1]+2);

}
int ServeurTcp::recevoirTrame( unsigned char* trameR){
	int compte = 0;
	int tailleTrame = 0;
	char  reponse[255];
	//char trameR[255];
	int taille_reponse = this->lire(reponse);

	// 1er octet
	printf (" ----------- \n");
	compte = taille_reponse - 2;
	//printf("taille reponse  : %d\n", taille_reponse);
	if(reponse[0] != 0xfffffff2){		//----------------- comparaison???
			return 0;
			printf(" pas f2!!! \n");
	}
	tailleTrame = reponse[1];
	printf("Entête F2 taille trame: %d type %x\n", tailleTrame,reponse[2]);
	for (int j=0; j<taille_reponse - 2 ; j++){
		trameR[j] = (unsigned int)reponse[j+2];
		//printf ("{%x}", trameR[j]);
	}
	//printf (" fin 1er morceau ----- \n");
	if (compte < tailleTrame){				// il faut lire la suite
		do {
			//printf (" nouveau morceau ----- \n");
			taille_reponse = this->lire(reponse);
			//printf("compte : %d\n", compte);
			for (int j=0; j<taille_reponse ; j++){
				trameR[compte+j] = (unsigned int)reponse[j];
				//printf ("{%x}", trameR[compte+j]);
			}
			compte += taille_reponse;
		} while (compte < tailleTrame);
		//printf (" fin des morceaux ----- \n");
	}
	//printf (" fin de la trame ----- \n");

	for (int j=0; j<tailleTrame; j++){
		//printf ("{%x}", trameR[j]);
	}
	// trame  = reinterpret_cast<unsigned char*>(trameR);
	//trame =(unsigned char*)trameR;
	//trame =trameR;
	printf (" fin méthode recevoir ----- \n");
	return tailleTrame;
}
