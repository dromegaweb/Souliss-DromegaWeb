
/**************************************************************************
*
* Progetto   :    Nodo01 Eth - Gateway - Arduino Mega 2560 + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Gennaio 2015     -  Inizio - esito OK
*                 21 Marzo 2015    -  Introdotto SpeakEasy - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*                 28 Dic 2015      -  Aggiornato parametri MAC
*                 29/03/2016       -  Attivazione interfaccia HTML/XML
*                 08 Aprile 2017   -  Aggiornamento sistema di compilazione
*
****************************************************************************/
// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Indirizzo MAC per Scheda Ethernet Nodo01 Gateway
#define MAC_INSKETCH                                              // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0x01};     // l'ultimo indirizzo "0x01" identifica il Nodo01
#define AUTO_MAC         0
#define MAC_DEBUG        0

// Configure the framework
#include "bconf/StandardArduino.h"        // Uso un Arduino standard
#include "conf/ethENC28J60.h"              // Uso connessione Ethernet con ENC28J60
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
//#include "conf/Gateway_wPersistence.h"    // uso il nodo come Gateway Souliss con persistenza
//#include "conf/XMLinterface.h"            // Abilito l'interfaccia HTML/XML

#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"


// This is the memory slot for the logic that handle the light

#define ALLARME  			0
//#define WATCHDOG      2

void setup()
{
  Initialize();
  
  Souliss_SetIPAddress(ip_address_Gw, subnet_mask, ip_gateway_Router);    // Questo nodo sarà --> 192.168.2.51
  SetAsGateway(myvNet_address);                                           // Questo nodo è Gateway Souliss app
  
  SetAsPeerNode(ip_Eth_N2, 1);                   // definisco Nodo02 Eth + brige01 RS485
  SetAsPeerNode(ip_Eth_N3, 2);                   // definisco Nodo03 Eth + brige02 RS485
  SetAsPeerNode(ip_address_Esp01,3);             // definisco NodoEsp01

  SetAsPeerNode(Nodo04_Bridge01_Peer01, 4);      // definisco Nodo04 peer01 su bridge01
  SetAsPeerNode(Nodo05_Bridge01_Peer02, 5);      // definisco Nodo05 peer02 su bridge01
  SetAsPeerNode(Nodo06_Bridge02_Peer01, 6);      // definisco Nodo06 peer01 su bridge02
  SetAsPeerNode(Nodo07_Bridge02_Peer02, 7);      // definisco Nodo07 peer02 su bridge02
    
  //      Set tipici e I/O ALLARME  
 
  Set_T41(ALLARME);
  
  pinMode(6, INPUT);         // tasto attiva allarme - PullDown
  pinMode(7, INPUT);         // tasto dosattiva allarme - PullDown
  
  pinMode(8, OUTPUT);        // acceso con allarme attivo
  pinMode(9, OUTPUT);        // acceso con allarme spento

  //Init_XMLServer();  // inizializza server XML
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_30ms() {
  
      DigIn(9, Souliss_T4n_Armed, ALLARME);      // tasto attiva allarme - PullDown
      DigIn(11, Souliss_T4n_NotArmed, ALLARME);   // tasto dosattiva allarme - PullDown
      Logic_T41(ALLARME);                         //esegui logica allarme
    /* 
      DigIn(2, Souliss_T1n_ToggleCmd, LED_O_N1);  // Usa Pin2 per cambiare lo stato pull-down
      Logic_T11( LED_O_N1);                       // Esegue la logica che gestisce i LED
      Logic_T11( LED_1_N1);
      Logic_T11( LED_2_N1);
      Logic_T11( LED_3_N1);
      DigOut(8,  Souliss_T1n_Coil, LED_O_N1);     // Usa PinX per alimentare e accendere i LED
      DigOut(9,  Souliss_T1n_Coil, LED_1_N1);
      DigOut(10, Souliss_T1n_Coil, LED_2_N1);
      DigOut(11, Souliss_T1n_Coil, LED_3_N1);
     */
    }
    
    FAST_510ms()  {      
        
        Logic_T41(ALLARME);                                //esegui logica allarme
        nDigOut(12,   Souliss_T4n_Antitheft,  ALLARME);    // Pin12 alto se allarme attivo              
        LowDigOut(13, Souliss_T4n_InAlarm,    ALLARME);    // Pin13 alto se allarme spento
    }
    
    FAST_1110ms() {
     //   Run_XMLServer();              //aggiornamento dati per XMLServer
    }
/*    elimino sezione watchdog
    FAST_2110ms() {    // watchdog di controllo presenza dei nodi    
    mInput(ALLARME) = Watchdog(Nodo04_Bridge01_Peer01, WATCHDOG, Souliss_T4n_Alarm);
    }
*/    
    FAST_GatewayComms();   //Qui elaboriamo tutte le comunicazioni con altri nodi la Souliss App


  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {		//
      
    }
  }
}


