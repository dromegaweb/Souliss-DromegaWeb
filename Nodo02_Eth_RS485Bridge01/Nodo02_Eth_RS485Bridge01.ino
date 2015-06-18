/**************************************************************************
*
* Progetto   :    Nodo02 Eth - Bridge01 RS485 - Arduino uno + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Febbraio 2015     - Inizio  - esito OK
*                 21 Marzo 2015     - introdotto SpeakEasy - esito OK
*                 25 Marzo 2015     - aggiunto gateway RS485 - esito OK
*                 29 Marzo 2015    -  Aggiunto gestione Usart InSketch - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*
****************************************************************************/

#define USARTDRIVER_INSKETCH
#define	USARTDRIVER		Serial	//Dico al driver vNet di usare la seriale 0 dell'UNO
#define USART_TXENABLE		1
#define USART_TXENPIN		3
#define USART_DEBUG  		0



// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/usart.h"	              // uso la seriale
#include "conf/SuperNode.h"           // The main node is the SuperNode



#include <SPI.h>
#include "Souliss.h"

// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------
uint8_t ip_address_Gw[4] = {192, 168, 2, 11};      // indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4] = {192, 168, 2, 12};      // indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4] = {192, 168, 2, 13};      // indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]   = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, 10};    // indirizzo Gateway router

#define	Gateway_address   11     // indirizzo ip Gateway Souliss   
#define E2_address        12     // indirizzo ip su Ethernet Nodo02
#define	E3_address        13     // indirizzo ip su Ethernet Nodo03
#define myvNet_address          ip_address_Gw[3]	  // ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		0xFF00
#define myvNet_supern	        0x0000
#define	myvNet_supern_E2	Nodo02_address_Gateway_RS485_01
#define	myvNet_supern_E3	Nodo03_address_Gateway_RS485_02

#define Nodo02_address_Gateway_RS485_01    0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485              
#define Nodo04_address_peer01_Gw_01	   0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_address_peer02_Gw_01        0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_address_Gateway_RS485_02    0xCE11	// Indirizzo Nodo03 bridge02 della Seriale RS485    
#define Nodo06_address_peer01_Gw_02        0xCE12	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_address_peer02_Gw_02        0xCE13	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_address_peer03_Gw_02        0xCE14	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------

// Indirizzo MAC per Scheda Ethernet Nodo02
#define MAC_INSKETCH
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBB};
#define AUTO_MAC         0
#define MAC_DEBUG        0


#define LIGHT1_N2     0
#define LIGHT2_N2	    1
#define LIGHT3_N2     2
#define LIGHT4_N2	    3
#define LIGHT5_N2     4
#define LIGHT6_N2	    5


void setup()
{
  Souliss_SetIPAddress(ip_address_E2, subnet_mask, ip_gateway_Router);                   // set IP a 12
  Souliss_SetAddress(Nodo02_address_Gateway_RS485_01, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485

  Set_T11(LIGHT1_N2);
  Set_T11(LIGHT2_N2);
  Set_T12(LIGHT3_N2);
  Set_T12(LIGHT4_N2);
  Set_T11(LIGHT5_N2);
  Set_T11(LIGHT6_N2);

  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);      // riservato RS485
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}


void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_30ms()  {

      // DigIn(2, Souliss_T1n_ToggleCmd, LIGHT1_N2);	// riservato RS485
      // DigIn(3, Souliss_T1n_ToggleCmd, LIGHT2_N2);  // riservato RS485
      DigIn(4, Souliss_T1n_ToggleCmd, LIGHT3_N2);
      DigIn(5, Souliss_T1n_ToggleCmd, LIGHT4_N2);
      DigIn(6, Souliss_T1n_ToggleCmd, LIGHT5_N2);
      DigIn(7, Souliss_T1n_ToggleCmd, LIGHT6_N2);

      Logic_T11(LIGHT1_N2);
      Logic_T11(LIGHT2_N2);
      Logic_T12(LIGHT3_N2);
      Logic_T12(LIGHT4_N2);
      Logic_T11(LIGHT5_N2);
      Logic_T11(LIGHT6_N2);

      DigOut(8, Souliss_T1n_Coil, LIGHT1_N2);
      DigOut(9, Souliss_T1n_Coil, LIGHT2_N2);
      DigOut(10, Souliss_T1n_Coil, LIGHT3_N2);
      DigOut(11, Souliss_T1n_Coil, LIGHT4_N2);
      DigOut(12, Souliss_T1n_Coil, LIGHT5_N2);
      DigOut(13, Souliss_T1n_Coil, LIGHT6_N2);
    }
    
    FAST_PeerComms();
        
    START_PeerJoin();
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
      Timer_T12(LIGHT3_N2);
      Timer_T12(LIGHT4_N2);
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}




