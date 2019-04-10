/**************************************************************************
* Utilizzo   :    Nodo unico allarme (Gateway)
* Progetto   :    Nodo10 - Esp01
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo è un sensore PIR
* 
* Data       :    22 Aprile 2017   -  Approntamento come PIR
*
* NOTE : Pin riservati 
*               Ingresso PIR     ->  0
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

/*
// Indirizzo MAC per Scheda Ethernet Nodo03
#define MAC_INSKETCH                                           // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x04};  // l'ultimo indirizzo "0x04" identifica il Nodo04
#define AUTO_MAC         0
#define MAC_DEBUG        0
*/

// Configure the framework
#include "bconf/MCU_ESP8266.h"          
#include "conf/Gateway.h"
#include "conf/IPBroadcast.h"

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "Souliss.h"





//#include "C:\Users\DROMEGA2\Documents\Arduino\libraries\DromegaWebConf\DromegaWebConf.h"         // **** Define Network  ****   
//#include "C:\Users\DROMEGA2\Documents\Arduino\libraries\DromegaWebConf\DromegaWebWifi.h"         // **** Define private WiFi ****   


// This identify the number of the I/O logic
#define ALLARME    0               

// **** Definiti i pin del modulo ESP **** 
#define INPUTPIN   0


void setup()
{   
    Initialize();
    SetIPAddress(ip_NodoTest, subnet_mask, ip_Router);
    SetAsGateway(myvNet_address);   
   
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

        FAST_GatewayComms();  // Usato solo quando questo nodo è anche Gateway                                      
        //FAST_PeerComms();     // Usato solo quando questo nodo è peer                                       
        
    }
} 
