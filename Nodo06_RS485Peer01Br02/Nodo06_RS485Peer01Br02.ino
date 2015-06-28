/**************************************************************************
 * 
 * Progetto   :    Nodo06 Peer01 RS485 su Bridge02 (Nodo03) - Arduino Pro Mini
 * Autore     :    DromegaWeb
 * Data       :    27 Giugno 2015    -  Prima bozza 
 * 
 * 
 ***************************************************************************/
  
  #define USARTDRIVER_INSKETCH
  #define USARTDRIVER	    Serial	//Dico al driver vNet di usare la seriale 0 dell'Arduino UNO
  #define USART_TXENABLE    1
  #define USART_TXENPIN		3
  #define USART_DEBUG  		0
  
  
  // Configure the framework
  #include "bconf/StandardArduino.h"			// Uso un Arduino standard
  #include "conf/usart.h"	                        // uso la seriale
  
  
  #include <SPI.h>
  #include "Souliss.h"
  
  
 // -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------
uint8_t ip_address_Gw[4]  = {192, 168, 2, 11};      // indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, 12};      // indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, 13};      // indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, 10};    // indirizzo Gateway router

#define	Gateway_address   11     // indirizzo ip Gateway Souliss   
#define E2_address        12     // indirizzo ip su Ethernet Nodo02
#define	E3_address        13     // indirizzo ip su Ethernet Nodo03
#define myvNet_address          ip_address_Gw[3]	  // ultimo byte dell' IP address (11) � anche l'indirizzo vNet
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

#define LIGHT1_N6        0
#define LIGHT2_N6	     1


void setup()
{
  Initialize();
  Souliss_SetAddress(Nodo06_address_peer01_Gw_02, myvNet_subnet, Nodo03_address_Gateway_RS485_02);

  Set_T11(LIGHT1_N6);
  Set_T11(LIGHT2_N6);


         //  pinMode(7,INPUT);
         //  pinMode(8,INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);					
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);					
  pinMode(10,OUTPUT);
  
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_50ms() {
                             //   DigIn(7,Souliss_T1n_ToggleCmd,LIGHT1_N6);	
                             //   DigIn(8,Souliss_T1n_ToggleCmd,LIGHT2_N6);	
      Logic_T11(LIGHT1_N6);
      Logic_T11(LIGHT2_N6);
    
      DigOut(6, Souliss_T1n_Coil, LIGHT1_N6);      
      DigOut(7, Souliss_T1n_Coil, LIGHT2_N6);      	
   
    }

    FAST_PeerComms()

    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		// Gestiamo il timer con un tempo base di 10 secondi
        Timer_T11(LIGHT1_N6);
        Timer_T11(LIGHT2_N6);
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }

}






