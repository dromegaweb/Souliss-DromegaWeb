/**************************************************************************
*
* Progetto   :    Nodo02 Eth - Bridge01 RS485 - Arduino uno + Ethernet W5100 
* Autore     :    DromegaWeb 
* Data       :    Febbraio 2015     - Inizio  - esito OK
*                 21 Marzo 2015     - introdotto SpeakEasy - esito OK
*                 25 Marzo 2015     - aggiunto gateway RS485 - esito OK
*                 29 Marzo 2015    -  Aggiunto gestione Usart InSketch - esito OK

* 
****************************************************************************/

#define USARTDRIVER_INSKETCH
#define	USARTDRIVER		Serial	//Dico al driver vNet di usare la seriale 0 dell'UNO
#define USART_TXENABLE		1
#define USART_TXENPIN		3
#define USART_DEBUG  		0

#include "bconf/inSketch.h"		    // uso la modalità inSketch 
#define  QC_BOARDTYPE  		0x41	    // Arduino Ethernet (or Ethernet Shield) with USART 
#define  QC_GATEWAYTYPE  	0x00        // definisco la schena "non gateway eth"     
#define  QC_INTERFACE           0x00
                                        
#include "Souliss.h"
#include "SpeakEasy.h"
#include <SPI.h>

#define  MAC_INSKETCH const uint8_t MAC_ADDRESS[] = {  0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBB};

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

/*
#define LEDLOGICNO_N1		0				// This is the memory slot for the logic that handle the light
#define LEDLOGICN1_N1		1
#define LEDLOGICN2_N1		2  
#define LEDLOGICN3_N1		3
#define LEDLOGICN4_N1		4
#define LEDLOGICN5_N1		5*/

#define LIGHT1_N2            0
#define LIGHT2_N2	     1
#define LIGHT3_N2            2
#define LIGHT4_N2	     3
#define LIGHT5_N2            4
#define LIGHT6_N2	     5
    

void setup()
{	
  Souliss_SetAddress(myvNet_address_2, myvNet_subnet, myvNet_supern);         // set IP a 12
  Souliss_SetAddress(Nodo02_address_bridge01, myvNet_subnet, myvNet_supern);  //definisce l'indirizzo del gateway RS485

  Souliss_SetT11(memory_map, LIGHT1_N2);        
  Souliss_SetT11(memory_map, LIGHT2_N2);
  Souliss_SetT12(memory_map, LIGHT3_N2);
  Souliss_SetT12(memory_map, LIGHT4_N2);
  Souliss_SetT11(memory_map, LIGHT5_N2);
  Souliss_SetT11(memory_map, LIGHT6_N2);

  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);
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

      // Souliss_DigIn(2, Souliss_T1n_ToogleCmd, memory_map, LIGHT1_N2);	// riservato enable RS485	
      // Souliss_DigIn(3, Souliss_T1n_ToogleCmd, memory_map, LIGHT2_N2);
      Souliss_DigIn(4, Souliss_T1n_ToogleCmd, memory_map, LIGHT3_N2);		
      Souliss_DigIn(5, Souliss_T1n_ToogleCmd, memory_map, LIGHT4_N2);		
      Souliss_DigIn(6, Souliss_T1n_ToogleCmd, memory_map, LIGHT5_N2);		
      Souliss_DigIn(7, Souliss_T1n_ToogleCmd, memory_map, LIGHT6_N2);		

      Souliss_Logic_T11(memory_map, LIGHT1_N2, &data_changed);
      Souliss_Logic_T11(memory_map, LIGHT2_N2, &data_changed);
      Souliss_Logic_T12(memory_map, LIGHT3_N2, &data_changed);
      Souliss_Logic_T12(memory_map, LIGHT4_N2, &data_changed);
      Souliss_Logic_T11(memory_map, LIGHT5_N2, &data_changed);
      Souliss_Logic_T11(memory_map, LIGHT6_N2, &data_changed);

      Souliss_DigOut(8, Souliss_T1n_Coil, memory_map, LIGHT1_N2);	
      Souliss_DigOut(9, Souliss_T1n_Coil, memory_map, LIGHT2_N2);
      Souliss_DigOut(10, Souliss_T1n_Coil, memory_map, LIGHT3_N2);
      Souliss_DigOut(11, Souliss_T1n_Coil, memory_map, LIGHT4_N2);
      Souliss_DigOut(12, Souliss_T1n_Coil, memory_map, LIGHT5_N2);
      Souliss_DigOut(13, Souliss_T1n_Coil, memory_map, LIGHT6_N2);
    } 

    FAST_70ms() {
                ProcessCommunication();	
                }

    /*    FAST_70ms() {
     		}
     
     FAST_90ms() {
     		}
     
     FAST_110ms() {
     		}
     
     FAST_510ms() {
     		}
     
     FAST_2110ms() {
     		}		  
     }
     EXECUTESLOW() {
     	UPDATESLOW();
     	SLOW_10s() {	
     }
     
     SLOW_510s() {
     		} 
     
     */
  }




} 




