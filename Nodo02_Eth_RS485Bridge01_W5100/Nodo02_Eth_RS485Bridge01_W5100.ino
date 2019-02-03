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

#include <CapacitiveSensor.h>

int led_01=8;
int led_02=9;
int led_03=2;

CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);        // 10M resistor between pins 4 & 5, pin 5 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,7);        // 10M resistor between pins 4 & 7, pin 7 is sensor pin, add a wire and or foil



#define LED_01_N2      0
#define LED_02_N2      1
#define LED_03_N2      2



void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_E2, subnet_mask, ip_gateway_Router);        // set IP a 12
  Souliss_SetAddress(Nodo02_Bridge01_RS485, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485

  Set_T11(LED_01_N2);
  Set_T11(LED_02_N2);
  Set_T11(LED_03_N2);


  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);      //Riservato a seriale RS 485 pin Enable
  
  pinMode(led_01, OUTPUT);
  pinMode(led_02, OUTPUT);
  pinMode(led_03, OUTPUT);
  cs_4_5.set_CS_AutocaL_Millis(0xFFFFFFFF);  
  
}


void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    
    long total1 =  cs_4_5.capacitiveSensor(50);
    long total2 =  cs_4_6.capacitiveSensor(50);
    long total3 =  cs_4_8.capacitiveSensor(50);

    FAST_50ms()  {

      if (total1>300)   {
         SetInput(LED_01_N2,Souliss_T1n_ToggleCmd);
         Send(myvNet_address, 0 , Souliss_T1n_ToggleCmd);
         }
         
      if (total2>300) 
         SetInput(LED_02_N2,Souliss_T1n_ToggleCmd);
 
      if (total3>300) 
         SetInput(LED_03_N2,Souliss_T1n_ToggleCmd);

      Logic_T11(LED_01_N2);
      Logic_T11(LED_02_N2);
      Logic_T11(LED_03_N2);
      
      DigOut(led_01, Souliss_T1n_Coil, LED_01_N2);
      DigOut(led_02, Souliss_T1n_Coil, LED_02_N2);
      DigOut(led_03, Souliss_T1n_Coil, LED_03_N2);
     
    }
    
    FAST_PeerComms();
        
    START_PeerJoin();
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}




