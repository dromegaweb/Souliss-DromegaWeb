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
#define USART_TXENABLE	1
#define USART_TXENPIN		3
#define USART_DEBUG  		0

// Indirizzo MAC per Scheda Ethernet Nodo02
#define MAC_INSKETCH                                              // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0x02};     // l'ultimo indirizzo "0x02" identifica il Nodo02
#define AUTO_MAC         0
#define MAC_DEBUG        0

// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/usart.h"	              // uso la seriale
#include "conf/SuperNode.h"           // The main node is the SuperNode



#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"


#define LIGHT1_N2      0
#define LIGHT2_N2      1
#define LIGHT3_N2      2
#define LIGHT4_N2      3
#define LIGHT5_N2      4
#define LIGHT6_N2      5


void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_E2, subnet_mask, ip_gateway_Router);        // set IP a 12
  Souliss_SetAddress(Nodo02_Bridge01_RS485, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485

  Set_T11(LIGHT1_N2);
  Set_T11(LIGHT2_N2);
  Set_T12(LIGHT3_N2);
  Set_T12(LIGHT4_N2);
  Set_T11(LIGHT5_N2);
  Set_T11(LIGHT6_N2);

  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);      //Riservato a seriale RS 485 pin Enable
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




