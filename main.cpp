// main.cpp

#include <iostream>
#include "serveurtcp.h"
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include "mysql_driver.h"
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main(){

		ServeurTcp serveur(1115);
		
		while(1)
		{

		cout<<"Le serveur est en attente de connexion..."<<endl;

		serveur.accepter();
		cout<<"Connexion établie avec le client."<<endl;
		sleep(2);
		
		unsigned char req[5] ={ 0xf2, 0x03, 0x0c, 0xca, 0x6c};
		unsigned char* requete = reinterpret_cast<unsigned char*>(req);
		serveur.envoyerTrame(requete);
		cout<<"Requête envoyée"<<endl;
		
		unsigned char reponse[255];
		serveur.recevoirTrame(reponse);
		char* rep = reinterpret_cast<char*>(reponse);
		cout<<endl<<endl<<"Réponse reçu: ";
		for(int i=0;i<70;i++){
			cout<<hex<< " 0x"<<(int)(rep[i]&0xff);
		}
		
		if((rep[1]&0xff)==0x1){

			cout<<endl<<"---------- Station 1 ----------";
			int station = reponse[1];
			cout<<endl<<endl<<dec<<"Station: "<<station;
			
			int heure = reponse[2];
			int minute = reponse[3];
			//cout<<endl<<endl<<dec<<"Heure: "<<heure;
			//cout<<endl<<dec<<"Minute: "<<minute;
			std::ostringstream h;
			h<<heure<<":"<<minute;
			std::string hreleve = h.str();
			cout<<endl<<"Heure: "<<hreleve;
			
			//char date = (reponse[7]<<16)+"/"+(reponse[8]<<8)+"/"+reponse[9];
			int jour = reponse[4];
			int mois = reponse[5];
			int annee = reponse[6];
			//cout<<endl<<dec<<"Jour: "<<jour;
			//cout<<endl<<dec<<"Mois: "<<mois;
			//cout<<endl<<dec<<"Annee: "<<annee;
			std::ostringstream d;
			d<<"20"<<annee<<"-"<<mois<<"-"<<jour;
			std::string dreleve = d.str();
			cout<<endl<<"Date: "<<dreleve;
			
			float temperature = (reponse[9]<<8)+reponse[10];
			temperature = temperature / 10;
			cout<<endl<<dec<<"Température: "<<temperature<<" degrés";
			
			int humidite = reponse[13];
			cout<<endl<<dec<<"Humidité: "<<humidite<<"%";
			
			int batterie = (int)reponse[16];
			//batterie = batterie / 10;
			cout<<endl<<dec<<"Batterie: "<<batterie<<"V";
			
			int humectometre = reponse[30];
			cout<<endl<<dec<<"Humectomètre: "<<humectometre<<"%";
			
			int girouette = (reponse[41]<<8)+reponse[42];
			cout<<endl<<dec<<"Girouette: "<<girouette<<"°";
			
			float pyranometre = (reponse[44]<<8)+reponse[45];
			cout<<endl<<dec<<"Pyranomètre: "<<pyranometre<<"W/m2";
			
			float anemometre = reponse[48];
			anemometre = anemometre / 10;
			cout<<endl<<dec<<"Anèmomètre: "<<anemometre<<"m/s";
			
			int pluviometre = reponse[51];
			pluviometre = pluviometre / 10;
			cout<<endl<<dec<<"Pluviomètre: "<<pluviometre<<"mm";
			
			try {
				sql::mysql::MySQL_Driver *driver;
				sql:: Connection *con;
				sql::Statement *stmt;
				sql::ResultSet *res;
				driver = sql::mysql::get_mysql_driver_instance();
				con = driver-> connect("localhost", "collecte", "collecte");
				stmt = con-> createStatement();
				stmt->execute("USE collecte");
				char query[1200];
				sprintf(query, "INSERT INTO station1champs(date, heure, Anemometre, Girouette, Pyranometre, Humectometre, Pluviometre, Humidite, Batterie, Temperature) VALUES('%s', '%02d:%02d', %f, %d, %f, %d, %d, %d, %d, %f)", dreleve.c_str(), heure, minute, anemometre, girouette, pyranometre, humectometre, pluviometre, humidite, batterie, temperature);
				res = stmt-> executeQuery("SELECT * FROM station1champs ORDER BY date ASC");
				
				delete res;
				delete stmt;
				delete con;
				}
			catch (sql::SQLException &e)
				{
				std::cout<<endl<<endl<< "# ERR: " << e.what();
				std::cout<<" (MySOL error code: " << e.getErrorCode ();
				std::cout<<", SQLState: " << e.getSQLState() << " )" << std::endl;
				//return EXIT_SUCCESS;
				}
		}
		
		if((rep[1]&0xff)==0x2){
			cout<<endl<<"---------- Station 2 ----------";
			int station = rep[1];
			cout<<endl<<endl<<hex<<"Station: "<<station;
			
			int heure = rep[2];
			int minute = rep[3];
			//cout<<endl<<endl<<dec<<"Heure: "<<heure;
			//cout<<endl<<dec<<"Minute: "<<minute;
			std::ostringstream h;
			h<<heure<<":"<<minute;
			std::string hreleve = h.str();
			cout<<endl<<"Heure: "<<hreleve;
			
			int jour = rep[4];
			int mois = rep[5];
			int annee = rep[6];
			//cout<<endl<<endl<<dec<<"Date: "<<jour;
			//cout<<endl<<endl<<dec<<"Date: "<<mois;
			//cout<<endl<<endl<<dec<<"Date: "<<annee;
			std::ostringstream d;
			d<<"20"<<annee<<"-"<<mois<<"-"<<jour;
			std::string dreleve = d.str();
			cout<<endl<<"Date: "<<dreleve;
			
			float temperature = (rep[9]<<8)+rep[10];
			temperature = temperature / 10;
			cout<<endl<<endl<<dec<<"Température: "<<temperature;
			
			int humidite = rep[13];
			cout<<endl<<endl<<dec<<"Humidité: "<<humidite;
			
			float tensiometre1 = rep[15];
			tensiometre1 = tensiometre1 / 10;
			cout<<endl<<endl<<dec<<"Tensiomètre 1: "<<tensiometre1;
			
			float tensiometre2 = rep[17];
			tensiometre2 = tensiometre2 / 10;
			cout<<endl<<endl<<dec<<"Tensiomètre 2: "<<tensiometre2;
			
			int humisol1 = rep[30];
			humisol1 = humisol1 / 10;
			cout<<endl<<endl<<dec<<"Humidité Sol 1: "<<humisol1;
			
			int humisol2 = rep[33];
			humisol2 = humisol2 / 10;
			cout<<endl<<endl<<dec<<"Humidité Sol 2: "<<humisol2;
			
		}
		
		serveur.fermer();
		cout<<endl<<endl<<"Connexion fermée."<<endl<<endl;
		
	}

		return 0;
}


