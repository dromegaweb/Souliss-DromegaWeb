/**************************************************************************
*
* Progetto   :    Nodo01 Eth - Gateway - Arduino Mega 2560 + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Gennaio 2015     -  Inizio - esito OK
*                 21 Marzo 2015    -  Introdotto SpeakEasy - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*                 28 Dic 2015      -  Aggiornato parametri MAC
*                 29/03/2016       -  Attivazione interfaccia HTML/XML
*
****************************************************************************/
// Indirizzo MAC per Scheda Ethernet Nodo01
#define MAC_INSKETCH                                              // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0x01};     // l'ultimo indirizzo "0x01" identifica il Nodo01
#define AUTO_MAC         0
#define MAC_DEBUG        0


// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/Gateway.h"             // uso il nodo come Gateway Souliss
#include "conf/XMLinterface.h"        // Abilito l'interfaccia HTML/XML


#include <SPI.h>
#include "Souliss.h"

// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------

#define ip_Router		  10      // indirizzo ip Ethernet Router
#define	ip_Gateway		11			// indirizzo ip Ethernet Gateway Souliss Nodo01  
#define ip_Eth_N2		  12			// indirizzo ip Ethernet Bridge01 Nodo02
#define	ip_Eth_N3		  13			// indirizzo ip Ethernet Bridge02 Nodo03

uint8_t ip_address_Gw[4]  = {192, 168, 2, ip_Gateway};		// indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, ip_Eth_N2};		// indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, ip_Eth_N3};		// indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, ip_Router};	// indirizzo Gateway router

#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		  0xFF00
#define myvNet_supern		  0x0000
#define	myvNet_supern_E2	Nodo02_Bridge01_RS485
#define	myvNet_supern_E3	Nodo03_Bridge02_RS485

#define Nodo02_Bridge01_RS485		    0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_Bridge01_Peer01			0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_Bridge01_Peer02			0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_Bridge02_RS485		    0xCF01	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_Bridge02_Peer01			0xCF02	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_Bridge02_Peer02			0xCF03	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_Bridge02_Peer03			0xCF04	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------




#define LED_O_N1			0	// This is the memory slot for the logic that handle the light
#define LED_1_N1			1
#define LED_2_N1			2
#define LED_3_N1			3
#define ALLARME  			4
#define WATCHDOG      5


void setup()
{
  Initialize();
  
  Souliss_SetIPAddress(ip_address_Gw, subnet_mask, ip_gateway_Router);    // Questo nodo sarà --> 192.168.2.11
  SetAsGateway(myvNet_address);                                           // Questo nodo è Gateway Souliss app
  
  SetAsPeerNode(ip_Eth_N2, 1);                       // definisco Nodo02 Eth + brige01 RS485
  SetAsPeerNode(ip_Eth_N3, 2);                       // definisco Nodo03 Eth + brige02 RS485

  SetAsPeerNode(Nodo04_Bridge01_Peer01, 3);      // definisco Nodo04 peer01 su bridge01
  SetAsPeerNode(Nodo05_Bridge01_Peer02, 4);      // definisco Nodo05 peer02 su bridge01
  SetAsPeerNode(Nodo06_Bridge02_Peer01, 5);      // definisco Nodo06 peer01 su bridge02


  //         Set tipici e I/O LED
  
  Set_T11(LED_O_N1);
  Set_T11(LED_1_N1);
  Set_T11(LED_2_N1);
  Set_T11(LED_3_N1);
  
  pinMode(2, INPUT);	    // Ingresso pulldown 
 
  pinMode(8, OUTPUT);	    // Power the LED
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  
  //      Set tipici e I/O ALLARME  
 
  Set_T41(ALLARME);
  //pinMode(7, INPUT);      // Ingresso pulldown
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Init_XMLServer();  // inizializza server XML
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_30ms() {      
      DigIn(2, Souliss_T1n_ToggleCmd, LED_O_N1);  // Usa Pin2 per cambiare lo stato pull-down
      Logic_T11( LED_O_N1);                       // Esegue la logica che gestisce i LED
      Logic_T11( LED_1_N1);
      Logic_T11( LED_2_N1);
      Logic_T11( LED_3_N1);
      DigOut(8,  Souliss_T1n_Coil, LED_O_N1);     // Usa PinX per alimentare e accendere i LED
      DigOut(9,  Souliss_T1n_Coil, LED_1_N1);
      DigOut(10, Souliss_T1n_Coil, LED_2_N1);
      DigOut(11, Souliss_T1n_Coil, LED_3_N1);
     
    }
    
    FAST_510ms() {      
        //DigIn(7, Souliss_T4n_Alarm, ALLARME);           // Pin7 ingresso sensore allarme pull-down
        Logic_T41(ALLARME);                             //esegui logica allarme
        nDigOut(12, Souliss_T4n_Antitheft, ALLARME);    // Pin12 alto se allarme attivo              
        LowDigOut(13, Souliss_T4n_InAlarm, ALLARME);    // Pin13 alto se allarme spento
    }
    
    FAST_1110ms() {
        Run_XMLServer();              //aggiornamento dati per XMLServer
    }
    
    FAST_2110ms()   {    // watchdog di controllo presenza dei nodi    
            mInput(ALLARME) = Watchdog(Nodo04_Bridge01_Peer01, WATCHDOG, Souliss_T4n_Alarm);
    }
    
    FAST_GatewayComms();   //Qui elaboriamo tutte le comunicazioni con altri nodi la Souliss App


  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {		// Gestiamo il timer luce con un tempo di base di 10 secondi
      Timer_T11( LED_O_N1);
      Timer_T11( LED_1_N1);
      Timer_T11( LED_2_N1);
      Timer_T11( LED_3_N1);
    }
  }
}


