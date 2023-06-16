// main.cpp

#include <iostream>
#include <cstring>
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
		
		while(1){

		cout<<endl<<"Le serveur est en attente de connexion..."<<endl;
		
		serveur.accepter();
		cout<<endl<<"Connexion établie avec le client."<<endl;
		
		sleep(2);
		
		unsigned char req[5] ={ 0xf2, 0x03, 0x0c, 0xca, 0x6c};
		unsigned char* requete = reinterpret_cast<unsigned char*>(req);
		serveur.envoyerTrame(requete);
		cout<<endl<<"Requête envoyée"<<endl;
		
		int tailleTrame = 61 ;
		unsigned char reponse[tailleTrame];
		tailleTrame = serveur.recevoirTrame(reponse);
		//cout<<tailleTrame;
		//char* rep = reinterpret_cast<char*>(reponse);
		cout<<endl<<endl<<"Réponse reçue: ";
		for(int i=0;i<tailleTrame;i++){
			cout<<hex<<" 0x"<<(int)(reponse[i]&0xff);
		}
		
		if((reponse[1]&0xff)==0x1){

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
			
			int HumAir = reponse[13];
			cout<<endl<<dec<<"Humidité de l'air: "<<HumAir<<"%";
			
			int batterie = (int)reponse[16];
			batterie = batterie / 10;
			cout<<endl<<dec<<"Batterie: "<<batterie<<"V";
			
			int humectometre = reponse[30];
			cout<<endl<<dec<<"Humectomètre: "<<humectometre<<"%";
			
			int girouette = (reponse[41]<<8)+reponse[42];
			cout<<endl<<dec<<"Girouette: "<<girouette<<"°";
			
			float pyranometre = (reponse[44]<<8)+reponse[45];
			cout<<endl<<dec<<"Pyranomètre: "<<pyranometre<<"W/m2";
			
			float anemometre = reponse[48];
			anemometre = 0.447 * anemometre;
			cout<<endl<<dec<<"Anèmomètre: "<<anemometre<<"m/s";
			
			int pluviometre = reponse[51];
			pluviometre = pluviometre / 10;
			cout<<endl<<dec<<"Pluviomètre: "<<pluviometre<<"mm";
			
			short int crcTrame = ((reponse[tailleTrame-2]<<8) + reponse[tailleTrame-1]);
			cout<<endl<<endl<<"Crc Trame: "<<hex<<crcTrame;
			
			unsigned int crc = serveur.calculerCRC(reponse, tailleTrame);
			short int crcTest = reinterpret_cast<unsigned int>(crc);
			cout<<endl<<hex<<"Le crc est: "<<(int)crcTest;
			
			if(crcTrame == crcTest){
				
				cout<<endl<<endl<<"Les Trames correspondent";
			
			try {
				sql::mysql::MySQL_Driver *driver;
				sql:: Connection *con;
				sql::Statement *stmt;
				//sql::ResultSet *res;
				bool res;
				driver = sql::mysql::get_mysql_driver_instance();
				con = driver-> connect("localhost", "comcollecteTCP", "collecte");
				stmt = con-> createStatement();
				stmt->execute("USE collecte");
				char query[1200];
				sprintf(query, "INSERT INTO station1champs(date, heure, Anemometre, Girouette, Pyranometre, Humectometre, Pluviometre, HumAir, Batterie, Temperature) VALUES('%s', '%02d:%02d', %f, %d, %f, %d, %d, %d, %d, %f)", dreleve.c_str(), heure, minute, anemometre, girouette, pyranometre, humectometre, pluviometre, HumAir, batterie, temperature);
				//cout<<endl<<query;
				res = stmt-> execute(query);
				cout<<endl<<endl<<"Nombre de ligne insérée: "<<res+1;
				//delete res;
				delete stmt;
				delete con;
			}
					
			catch (sql::SQLException &e){
				std::cout<<endl<<endl<< "# ERR: " << e.what();
				std::cout<<" (MySOL error code: " << e.getErrorCode ();
				std::cout<<", SQLState: " << e.getSQLState() << " )" << std::endl;
				//return EXIT_SUCCESS;
			}
			
			}else{
				
				cout<<endl<<"Les Trames ne correspondent pas";
				
			}
		}
		
		if((reponse[1]&0xff)==0x2){
			cout<<endl<<"---------- Station 2 ----------";
			int station = reponse[1];
			cout<<endl<<endl<<hex<<"Station: "<<station;
			
			int heure = reponse[2];
			int minute = reponse[3];
			//cout<<endl<<endl<<dec<<"Heure: "<<heure;
			//cout<<endl<<dec<<"Minute: "<<minute;
			std::ostringstream h;
			h<<heure<<":"<<minute;
			std::string hreleve = h.str();
			cout<<endl<<"Heure: "<<hreleve;
			
			int jour = reponse[4];
			int mois = reponse[5];
			int annee = reponse[6];
			//cout<<endl<<endl<<dec<<"Date: "<<jour;
			//cout<<endl<<endl<<dec<<"Date: "<<mois;
			//cout<<endl<<endl<<dec<<"Date: "<<annee;
			std::ostringstream d;
			d<<"20"<<annee<<"-"<<mois<<"-"<<jour;
			std::string dreleve = d.str();
			cout<<endl<<"Date: "<<dreleve;
			
			float temperature = (reponse[9]<<8)+reponse[10];
			temperature = temperature / 10;
			cout<<endl<<dec<<"Température: "<<temperature<<" degrés";
			
			int HumAir = reponse[13];
			cout<<endl<<dec<<"Humidité de l'air: "<<HumAir<<"%";
			
			int tensiometre1 = reponse[15];
			if(tensiometre1>239){
				tensiometre1 = 239;
			}else if(tensiometre1<0){
				tensiometre1 = 0;
			}
			cout<<endl<<dec<<"Tensiomètre 1: "<<tensiometre1;
			
			int tensiometre2 = reponse[21];
			if(tensiometre2>239){
				tensiometre2 = 239;
			}else if(tensiometre2<0){
				tensiometre2 = 0;
			}
			cout<<endl<<dec<<"Tensiomètre 2: "<<tensiometre2;
			
			float HumSol1 = (reponse[29]<<8)+reponse[30];
			HumSol1 = HumSol1 / 10;
			if (HumSol1 > 100){
				HumSol1 = 100;
			}
			cout<<endl<<dec<<"Humidité Sol 1: "<<HumSol1;
			
			float HumSol2 = (reponse[32]<<8)+reponse[33];
			HumSol2 = HumSol2 / 10;
			if (HumSol2 > 100){
				HumSol2 = 100;
			}
			cout<<endl<<dec<<"Humidité Sol 2: "<<HumSol2;
			
			short int crcTrame = ((reponse[tailleTrame-2]<<8) + reponse[tailleTrame-1]);
			cout<<endl<<endl<<"Crc Trame: "<<hex<<crcTrame;
			
			unsigned int crc = serveur.calculerCRC(reponse, tailleTrame);
			short int crcTest = reinterpret_cast<unsigned int>(crc);
			cout<<endl<<hex<<"Le crc est: "<<(int)crcTest;
			
			if(crcTrame == crcTest){
				
				cout<<endl<<endl<<"Les Trames correspondent";
			
			try {
				sql::mysql::MySQL_Driver *driver;
				sql:: Connection *con;
				sql::Statement *stmt;
				//sql::ResultSet *res;
				bool res;
				driver = sql::mysql::get_mysql_driver_instance();
				con = driver-> connect("localhost", "comcollecteTCP", "collecte");
				stmt = con-> createStatement();
				stmt->execute("USE collecte");
				char query[1200];
				sprintf(query, "INSERT INTO station2serre(date, heure, Tensiometre1, Tensiometre2, HumSol1, HumSol2, HumAir, Temperature) VALUES('%s', '%02d:%02d', %d, %d, %f, %f, %d, %f)", dreleve.c_str(), heure, minute, tensiometre1, tensiometre2, HumSol1, HumSol2, HumAir, temperature);
				//cout<<en[Cdl<<query;
				res = stmt-> execute(query);
				cout<<endl<<endl<<"Nombre de ligne insérée: "<<res+1;
				//delete res;
				delete stmt;
				delete con;
			}
					
			catch (sql::SQLException &e){
				std::cout<<endl<<endl<< "# ERR: "<<e.what();
				std::cout<<endl<<" (MySOL error code: "<<e.getErrorCode ();
				std::cout<<endl<<", SQLState: "<<e.getSQLState()<<")"<< std::endl;
				//return EXIT_SUCCESS;
			}
			
			}else{
				
				cout<<endl<<"Les Trames ne correspondent pas";
				
			}
		}
		
	serveur.fermer();
	cout<<endl<<endl<<"Connexion fermée."<<endl<<endl;
	
	}
	
}
