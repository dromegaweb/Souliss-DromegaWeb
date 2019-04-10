/**************************************************************************
* Utilizzo   :    Nodo Allarme01
* Progetto   :    Nodo13 - Esp12 + PIR
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo ha un sensore PIR
* 
* Data       :    22 Apr 2017   -   Approntamento come PIR
*                 09 Mar 2019   -   Finalizzato
*
* NOTE       :    Pin riservati 
*                 Ingresso PIR   ->  12
*      
***************************************************************************/

// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// **** Definisco il nome e la password della rete WiFi a cui mi connetto ****     
#define WIFICONF_INSKETCH                //dichiarato in  #include "DromegaWebWifi.h"
#define WiFi_SSID          "DrHouse"     //scrivi tra virgolette il nome della tua rete
#define WiFi_Password      "bf14473bf"   //scrivi tra virgolette la password della tua rete 

//#include "DromegaWebWifi.h"
#include "DromegaWebConf.h"

// Include framework code and libraries
#include "bconf/MCU_ESP8266.h"          
#include "conf/IPBroadcast.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "Souliss.h"

// Typical
#define ALLARME     0
#define T11_TEST    1

// **** Definiti i pin del modulo ESP12 **** 
#define INPUTPIN   12

void setup()
{   
    Initialize();
    INIT_Nodo13_TEST_stat();   //Mappa rete nodo vedi DromegaWebConf.h 	  
    Set_T42(ALLARME);
    Set_T11(T11_TEST);
    pinMode(INPUTPIN, INPUT);  // ingersso allarme
    
}

void loop()
{ 
    // Inizio ciclo
    EXECUTEFAST() {
        UPDATEFAST();   
        FAST_510ms() {   
              DigIn(INPUTPIN, Souliss_T4n_Alarm, ALLARME);
              Logic_T42(ALLARME,Add_Gateway);
        }
        FAST_PeerComms();
    }
}
