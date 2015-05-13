/**************************************************************************
 * 
 * Progetto   :    Nodo05 Peer02 RS485 su Bridge01 (Nodo02) - Arduino Uno standalone
 * Autore     :    DromegaWeb
 * Data       :    25 Marzo 2015    -  Inizio - esito OK
 *                 29 Marzo 2015    -  Aggiunto gestione Usart InSketch - esito OK
 *                 03 aprile 2015   -  Aggiunto 2 PIR in ingresso
 * 
 ***************************************************************************/

#define USARTDRIVER_INSKETCH
#define	USARTDRIVER		Serial	//Dico al driver vNet di usare la seriale 0 dell'UNO
#define USART_TXENABLE		1
#define USART_TXENPIN		3
#define USART_DEBUG  		0

#include "bconf/inSketch.h"		    // Uso la modalità inSketch 
#define  QC_BOARDTYPE  		0x43	    // Arduino with USART
#define  QC_GATEWAYTYPE  	0x00        // definisco la schena "non gateway eth"     
#define  QC_INTERFACE           0x00

#include "Souliss.h"
#include "SpeakEasy.h"	
#include <SPI.h>

// network addresses

#define myvNet_address_1	0x000B 		// 0x000B IP 11
#define myvNet_address_2	0x000C          // 0x000C IP 12    ***Questo nodo***
#define myvNet_address_3	0x000D          // 0x000C IP 13
#define myvNet_subnet	        0xFF00
#define myvNet_supern	        0x0000

#define Nodo02_address_bridge01	          0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485               ***Questo nodo***
#define Nodo04_address_peer01br01	  0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_address_peer02br01         0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_address_bridge02	          0xCE04	// Indirizzo Nodo03 bridge02 della Seriale RS485    
#define Nodo06_address_peer01br02	  0xCE05	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_address_peer02br02         0xCE06	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_address_peer03br02         0xCE07	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// END network addresses

#define LIGHT1_N5            0
#define LIGHT2_N5	     1
#define LIGHT3_N5	     2
#define LIGHT4_N5	     3
#define LIGHT5_N5	     4

//#define PIR_01  5
//#define PIR_02  6
//#define PIR_03  7

void setup()
{
  Souliss_SetAddress(Nodo05_address_peer02br01, myvNet_subnet, myvNet_supern);

  Set_AutoLight(LIGHT1_N5);
  Set_AutoLight(LIGHT2_N5);
              //Set_SimpleLight(LIGHT3_N5);
              //Set_SimpleLight(LIGHT4_N5);
              //Set_SimpleLight(LIGHT5_N5);
  
//  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);

  pinMode(9, OUTPUT);					
  pinMode(10, OUTPUT);
                //pinMode(11, OUTPUT);					
                //pinMode(12, OUTPUT);
                //pinMode(13, OUTPUT);
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_50ms() {
      
           //   ssDigIn(6,10,PIR_01);	
      ssDigIn(7,Souliss_T1n_ToogleCmd,LIGHT1_N5);	
      ssDigIn(8,Souliss_T1n_ToogleCmd,LIGHT2_N5);	
      
      Logic_AutoLight(LIGHT1_N5);
      Logic_AutoLight(LIGHT2_N5);
              //  Logic_SimpleLight(LIGHT3_N5);
              //  Logic_SimpleLight(LIGHT4_N5);
               // Logic_SimpleLight(LIGHT5_N5);
      
      ssDigOut(9, Souliss_T1n_Coil, LIGHT1_N5);      	
      ssDigOut(10, Souliss_T1n_Coil, LIGHT2_N5);
           //   ssDigOut(11, Souliss_T1n_Coil, LIGHT3_N5);      	
           //   ssDigOut(12, Souliss_T1n_Coil, LIGHT4_N5);
           //   ssDigOut(13, Souliss_T1n_Coil, LIGHT5_N5);      	

    }

    FAST_70ms() {                  
      ProcessCommunication();	
    }

    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		// Gestiamo il timer con un tempo base di 10 secondi
        Timer_AutoLight(LIGHT1_N5);
        Timer_AutoLight(LIGHT2_N5);
                //Timer_SimpleLight(LIGHT3_N5);
                //Timer_SimpleLight(LIGHT4_N5);
                //Timer_SimpleLight(LIGHT5_N5);
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }

}






