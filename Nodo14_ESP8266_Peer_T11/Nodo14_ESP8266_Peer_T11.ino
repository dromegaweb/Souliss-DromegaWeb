/**************************************************************************
* Utilizzo   :    Nodo T11
* Progetto   :    Nodo14 - Esp01
* Autore     :    DromegaWeb
* Scopo      :    Test T11
* 
* Data       :    12 Mar 2019   -  Approntamento
*
* NOTE        :   Pin riservati - .....
*                   
*      
***************************************************************************/

#define DW_DEBUG           //Messaggi visibili via seriale

#define HOSTNAME "Nodo14"


// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Include framework code and libraries
#include "bconf/MCU_ESP8266.h"          

// **** Definisco il nome e la password della rete WiFi a cui mi connetto ****     
#define WIFICONF_INSKETCH        //dichiarato in  #include "DromegaWebWifi.h"
#define WiFi_SSID 		"DrHouse"           //scrivi tra virgolette il nome della tua rete
#define WiFi_Password 	"bf14473bf"     //scrivi tra virgolette la password della tua rete 


#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include "Souliss.h"

#include "DromegaWebConf.h"
//#include "DromegaWebWifi.h"

// Typical
#define TEST_T11    0               

// **** Definiti i pin del modulo ESP01 **** 
#define INPUTPIN   0

void setup()
{   
	#ifdef DW_DEBUG
		Serial.begin(115200);
		Serial.println(" ");
		Serial.println("Nodo Attivo");
	#endif
	
    Initialize();
	
	#ifdef DW_DEBUG
		Serial.println("Inizializzazione Nodo14");
	#endif
	
    Souliss_SetIPAddress(ip_Nodo14, subnet_mask, ip_Router);
    //SetAddress(0xAB02, 0xFF00, 0xAB01);
 	  //INIT_Nodo14();  	//Mappa rete nodo vedi DromegaWebConf.h
	  pinMode(INPUTPIN, INPUT);  // ingersso allarme
	  Set_T11(TEST_T11);
	  
	  //Enable OTA
	ArduinoOTA.setHostname(HOSTNAME);
	ArduinoOTA.begin();
	#ifdef DW_DEBUG
		Serial.print("MAC: ");
		Serial.println(WiFi.macAddress());
		Serial.print("IP:  ");
		Serial.println(WiFi.localIP());
		Serial.print("Subnet: ");
		Serial.println(WiFi.subnetMask());
		Serial.print("Gateway: ");
		Serial.println("Node Initialized");
	#endif
	  
}

void loop()
{ 
    // Inizio ciclo
    EXECUTEFAST() {                     
        UPDATEFAST();   
        FAST_510ms() {   
              DigIn(INPUTPIN, Souliss_T1n_ToggleCmd, TEST_T11);    
              Logic_T11(TEST_T11);                                      
        }
		FAST_PeerComms();
		ArduinoOTA.handle();
		//START_PeerJoin();
    }
} 

