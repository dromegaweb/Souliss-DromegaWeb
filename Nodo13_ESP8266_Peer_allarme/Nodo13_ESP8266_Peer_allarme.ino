/**************************************************************************
* Utilizzo   :    Nodo Allarme01
* Progetto   :    Nodo13 - Esp01 + PIR
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo ha un sensore PIR
* 
* Data       :    22 Apr 2017   -  Approntamento come PIR
*				          09 Mar 2019   -  Finalizzato
*
* NOTE        :   Pin riservati 
*                   Ingresso PIR     ->  0
*      
***************************************************************************/

//#define DEBUG
//#define DEBUG_SOULISS


// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Include framework code and libraries
#include "bconf/MCU_ESP8266.h"          
#include "conf/IPBroadcast.h"

// **** Definisco il nome e la password della rete WiFi a cui mi connetto ****     
#define WIFICONF_INSKETCH        //dichiarato in  #include "DromegaWebWifi.h"
#define WiFi_SSID 		"DrHouse"           //scrivi tra virgolette il nome della tua rete
#define WiFi_Password 	"bf14473bf"     //scrivi tra virgolette la password della tua rete 

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "Souliss.h"
#include "DromegaWebConf.h"
//#include "DromegaWebWifi.h"

// Typical
#define ALLARME    0               

// **** Definiti i pin del modulo ESP01 **** 
#define INPUTPIN   0

void setup()
{   
    Initialize();
    INIT_Nodo13_TEST();   //Mappa rete nodo vedi DromegaWebConf.h 	  
 	  Set_T41(ALLARME);
 	  pinMode(INPUTPIN, INPUT);  // ingersso allarme
    
}

void loop()
{ 
    // Inizio ciclo
    EXECUTEFAST() {                     
        UPDATEFAST();   
        FAST_510ms() {   
              DigIn(INPUTPIN, Souliss_T4n_Alarm, ALLARME);    
              Logic_T41(ALLARME);                                      
        }
		FAST_PeerComms();	
		//START_PeerJoin();
   //FAST_GatewayComms();
    }
} 




	/*
#ifdef DEBUG
	Serial.println("------------");
#endif
*/
