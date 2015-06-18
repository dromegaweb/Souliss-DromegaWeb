/**************************************************************************
*
* Progetto   :    Nodo01 Eth - Gateway - Arduino Mega 2560 + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Gennaio 2015     -  Inizio - esito OK
*                 21 Marzo 2015    -  Introdotto SpeakEasy - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*
****************************************************************************/

// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/Gateway.h"             // uso il nodo come Gateway Souliss

#include <SPI.h>
#include "Souliss.h"

// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------
uint8_t ip_address_Gw[4]  = {192, 168, 2, 11};      // indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, 12};      // indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, 13};      // indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, 10};    // indirizzo Gateway router

#define	Gateway_address		11				// indirizzo ip Gateway Souliss   
#define E2_address			12				// indirizzo ip su Ethernet Nodo02
#define	E3_address			13				// indirizzo ip su Ethernet Nodo03
#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		0xFF00
#define myvNet_supern		0x0000
#define	myvNet_supern_E2	Nodo02_address_Gateway_RS485_01
#define	myvNet_supern_E3	Nodo03_address_Gateway_RS485_02

#define Nodo02_address_Gateway_RS485_01		0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_address_peer01_Gw_01			0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_address_peer02_Gw_01			0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_address_Gateway_RS485_02		0xCE11	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_address_peer01_Gw_02			0xCE12	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_address_peer02_Gw_02			0xCE13	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_address_peer03_Gw_02			0xCE14	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------

// Indirizzo MAC per Scheda Ethernet Nodo01
#define MAC_INSKETCH
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBA};
#define AUTO_MAC         0
#define MAC_DEBUG        0


#define LED_O_N1			0	// This is the memory slot for the logic that handle the light
#define LED_1_N1			1
#define LED_2_N1			2
#define LED_3_N1			3
#define ALLARME  			4
#define WATCHDOG                        5


void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_Gw, subnet_mask, ip_gateway_Router);    // Questo nodo sarà --> 192.168.2.11
  SetAsGateway(myvNet_address);                                           // Questo nodo è Gateway Souliss app
  SetAsPeerNode(E2_address, 1);                       // definisco Nodo02 Eth + brige01 RS485
  SetAsPeerNode(Nodo04_address_peer01_Gw_01, 2);      // definisco Nodo04 peer01 su bridge01
  SetAsPeerNode(Nodo05_address_peer02_Gw_01, 3);      // definisco Nodo05 peer02 su bridge01


  // Set tipici e I/O LED
  Set_T11(LED_O_N1);
  Set_T11(LED_1_N1);
  Set_T11(LED_2_N1);
  Set_T11(LED_3_N1);
  
  
  pinMode(2, INPUT);	    // Ingresso pulldown 
 
  pinMode(8, OUTPUT);	    // Power the LED
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  
 // Set tipici e I/O ALLARME  
  Set_T41(ALLARME);
  pinMode(7, INPUT);    // Ingresso pulldown
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
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
        LowDigIn(7, Souliss_T4n_Alarm, ALLARME);       // Pin7 ingresso sensore allarme pull-up
        Logic_T41(ALLARME);                            //esegui logica allarme
        nDigOut(12, Souliss_T4n_Antitheft, ALLARME);    // Pin12 se allarme attivo              
        LowDigOut(13, Souliss_T4n_InAlarm, ALLARME);    // Pin13 se allarme spento
    }
    
    FAST_2110ms()   {    // watchdog di controllo presenza dei nodi    
            mInput(ALLARME) = Watchdog(Nodo04_address_peer01_Gw_01, WATCHDOG, Souliss_T4n_Alarm);
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


