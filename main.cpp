// main.cpp

#include <iostream>
#include "serveurtcp.h"
#include <unistd.h>
#include <vector>

using namespace std;

int main()
{
		ServeurTcp serveur(1115);
		cout<<"Le serveur est en attente de connexion..."<<endl;

		serveur.accepter();
		cout<<"Connexion etablie avec le client."<<endl;
		
		sleep(2);
		
		unsigned char req[5] ={ 0xf2, 0x03, 0x0c, 0xca, 0x6c};
		unsigned char* requete = reinterpret_cast<unsigned char*>(req);
		serveur.envoyerTrame(requete);
		cout<<"Requête envoyé"<<endl;
		
		unsigned char reponse[255];
		serveur.recevoirTrame(reponse);
		char* rep = reinterpret_cast<char*>(reponse);
		cout<<endl<<endl<<"Réponse recu: ";
		for(int i=0;i<70;i++){
			cout<<hex<< " 0x"<<(int)(rep[i]&0xff);
		}
		
		if(rep[2]&0xff)==Ox1){
		
		int station = rep[2];
		cout<<endl<<endl<<hex<<"Station: "<<station;
		
		//char heure = (rep[5]<<8)+":"+rep[6];
		int heure = rep[3];
		int minute = rep[4];
		cout<<endl<<endl<<hex<<"Heure: "<<heure;
		cout<<endl<<endl<<hex<<"Minute: "<<minute;
		
		//char date = (rep[7]<<16)+"/"+(rep[8]<<8)+"/"+rep[9];
		int jour = rep[5];
		int mois = rep[6];
		int annee = rep[7];
		cout<<endl<<endl<<hex<<"Date: "<<jour;
		cout<<endl<<endl<<hex<<"Date: "<<mois;
		cout<<endl<<endl<<hex<<"Date: "<<annee;
		
		int temperature = (rep[10]<<8)+rep[11];
		temperature = temperature / 10;
		cout<<endl<<endl<<hex<<"Température: "<<temperature;
		
		int humidite = rep[14];
		cout<<endl<<endl<<hex<<"Humidité: "<<humidite;
		
		int batterie = rep[17];
		batterie = batterie / 10;
		cout<<endl<<endl<<hex<<"Batterie: "<<batterie;
		
		int humectometre = rep[31];
		cout<<endl<<endl<<hex<<"Humectomètre: "<<humectometre;
		
		int girouette = (rep[42]<<8)+rep[43];
		cout<<endl<<endl<<hex<<"Girouette: "<<girouette;
		
		int pyranometre = (rep[45]<<8)+rep[46];
		cout<<endl<<endl<<hex<<"Pyranomètre: "<<pyranometre;
		
		int anemometre = rep[49];
		cout<<endl<<endl<<hex<<"Anèmomètre: "<<anemometre;
		
		int pluviometre = rep[52];
		pluviometre = pluviometre / 10;
		cout<<endl<<endl<<hex<<"Pluviomètre: "<<pluviometre;
		
		} else if((rep[2]&0xff)==Ox2){
		
		int station = rep[2];
		cout<<endl<<endl<<hex<<"Station: "<<station;
		
		//char heure = (rep[5]<<8)+":"+rep[6];
		int heure = rep[3];
		int minute = rep[4];
		cout<<endl<<endl<<hex<<"Heure: "<<heure;
		cout<<endl<<endl<<hex<<"Minute: "<<minute;
		
		//char date = (rep[7]<<16)+"/"+(rep[8]<<8)+"/"+rep[9];
		int jour = rep[5];
		int mois = rep[6];
		int annee = rep[7];
		cout<<endl<<endl<<hex<<"Date: "<<jour;
		cout<<endl<<endl<<hex<<"Date: "<<mois;
		cout<<endl<<endl<<hex<<"Date: "<<annee;
		
		int temperature = (rep[10]<<8)+rep[11];
		temperature = temperature / 10;
		cout<<endl<<endl<<hex<<"Température: "<<temperature;
		
		int humidite = rep[14];
		cout<<endl<<endl<<hex<<"Humidité: "<<humidite;
		
		int tensiometre1 = rep[16];
		tensiometre1 = tensiometre1 / 10;
		cout<<endl<<endl<<hex<<"Tensiomètre 1: "<<tensiometre1;
		
		int tensiometre2 = rep[18];
		tensiometre2 = tensiometre2 / 10;
		cout<<endl<<endl<<hex<<"Tensiomètre 2: "<<tensiometre2;
		
		int humisol1 = rep[31];
		humisol1 = humisol1 / 10;
		cout<<endl<<endl<<hex<<"Humidité Sol 1: "<<humisol1;
		
		int humisol2 = rep[34];
		humisol2 = humisol2 / 10;
		cout<<endl<<endl<<hex<<"Humidité Sol 2: "<<humisol2;
		
		}
		
		serveur.fermer();
		cout<<endl<<endl<<"Connexion fermee."<<endl<<endl;

		return 0;
}


