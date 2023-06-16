// ServeurTcp.h

#ifndef _SERVEURTCP_H
#define _SERVEURTCP_H
#ifndef CRC16KERMIT_H
#define CRC16KERMIT_H
#include "serveurtcp.h"
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class ServeurTcp
    {
    private:
    	int canal_demande;
    	int canal_dedie;
		struct sockaddr_in  nom_producteur;
    public:
    	ServeurTcp(int port);
    	int accepter();
		int fermer();
    	int lire(char* chaine);
    	int ecrire(char* chaine, int taille);
	int envoyerTrame(unsigned char* trame);
	int recevoirTrame(unsigned char* trame);
    	~ServeurTcp();
    	unsigned int calculerCRC(unsigned char* reponse, int tailleTrame);
    };

typedef struct
	{
	union
		{
		unsigned long l;
		struct
			{
			unsigned char b1;
			unsigned char b2;
			unsigned char b3;
			unsigned char b4;
			} b;
      		} u;
	} t_addr_in;



#endif	//_SERVEURTCP_H
#endif	//CRC16KERMIT_H
