/**************************************************************************
*
* Progetto   :    Nodo01 Eth - Gateway - Arduino Mega 2560 + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Gennaio 2015    -  Inizio - esito OK
*                 21 Marzo 2015   -  Introdotto SpeakEasy - esito OK
* 
****************************************************************************/

#include "bconf/inSketch.h"	            // Using the inSketch mode. FUNZIONANTE
#define  QC_BOARDTYPE  		0x04        // Arduino + Ethernet W5100
#define  QC_GATEWAYTYPE  	0x01        // Difinito come GateWay
#define  QC_INTERFACE           0x00

#include "Souliss.h"
#include "SpeakEasy.h"	
#include <SPI.h>

#define  MAC_INSKETCH const uint8_t MAC_ADDRESS[] = {  0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBA};    //Funzionante inSketch

// network addresses

#define myvNet_address_1	          0x000B 	// 0x000B IP 11    ***Questo nodo***  **Gateway Souliss app**
#define myvNet_address_2	          0x000C        // 0x000C IP 12    
#define myvNet_address_3	          0x000D        // 0x000C IP 13
#define myvNet_subnet	                  0xFF00
#define myvNet_supern	                  0x0000

#define Nodo02_address_bridge01	          0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485               
#define Nodo04_address_peer01br01	  0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_address_peer02br01         0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_address_bridge02	          0xCE04	// Indirizzo Nodo03 bridge02 della Seriale RS485    
#define Nodo06_address_peer01br02	  0xCE05	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_address_peer02br02         0xCE06	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_address_peer03br02         0xCE07	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// END network addresses

#define LEDLOGICNO_N1			0	// This is the memory slot for the logic that handle the light
#define LEDLOGICN1_N1			1
#define LEDLOGICN2_N1			2
#define LEDLOGICN3_N1			3
#define LEDLOGICN4_N1			4
#define LEDLOGICN5_N1			5
/*
#define LIGHT1_N2            0
#define LIGHT2_N2	     1
#define LIGHT3_N2            2
#define LIGHT4_N2	     3
#define LIGHT5_N2            4
#define LIGHT6_N2	     5
*/
 

void setup()
{	
  // Setup the network configuration
  

  Souliss_SetAddress(myvNet_address_1, myvNet_subnet, myvNet_supern);	// Questo nodo sarà --> 192.168.2.11	
  SetAsGateway(myvNet_address_1);                        // Questo nodo è Gateway Souliss app
  SetAsPeerNode(myvNet_address_2, 1);                    // definisco Nodo02 --- Eth + brige01 RS485
  SetAsPeerNode(Nodo04_address_peer01br01, 2);           // definisco Nodo04 peer01 su bridge01
  SetAsPeerNode(Nodo05_address_peer02br01, 3);           // definisco Nodo05 peer02 su bridge01



  Set_SimpleLight(LEDLOGICNO_N1);      // Set the typical
  Set_SimpleLight(LEDLOGICN1_N1);
  Set_SimpleLight(LEDLOGICN2_N1);
  Set_SimpleLight(LEDLOGICN3_N1);
  
  Set_AutoLight(LEDLOGICN4_N1);
  Set_AutoLight(LEDLOGICN5_N1);



  // Define inputs, outputs pins
  pinMode(2, INPUT);					// Hardware pulldown required
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  pinMode(8, OUTPUT);					// Power the LED
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{ 
  // The Souliss methods are scheduled in phases, this allow load
  // balance and proper timing.

  EXECUTEFAST() {						
	UPDATEFAST();	
		
	FAST_30ms() {
      // Use Pin2 as ON command, Pin3 as OFF command
      ssDigIn(2, Souliss_T1n_ToogleCmd, LEDLOGICNO_N1);		
      ssDigIn(3, Souliss_T1n_ToogleCmd, LEDLOGICN1_N1);		
      ssDigIn(4, Souliss_T1n_ToogleCmd, LEDLOGICN2_N1);		
      ssDigIn(5, Souliss_T1n_ToogleCmd, LEDLOGICN3_N1);		
      ssDigIn(6, Souliss_T1n_ToogleCmd, LEDLOGICN4_N1);		
      ssDigIn(7, Souliss_T1n_ToogleCmd, LEDLOGICN5_N1);		

      // Execute the logic that handle the LED
      Logic_SimpleLight( LEDLOGICNO_N1);
      Logic_SimpleLight( LEDLOGICN1_N1);
      Logic_SimpleLight( LEDLOGICN2_N1);
      Logic_SimpleLight( LEDLOGICN3_N1);
      Logic_AutoLight( LEDLOGICN4_N1);
      Logic_AutoLight( LEDLOGICN5_N1);

      // Use Pin9 as output to power the LED
      Souliss_DigOut(8, Souliss_T1n_Coil, memory_map, LEDLOGICNO_N1);	
      Souliss_DigOut(9, Souliss_T1n_Coil, memory_map, LEDLOGICN1_N1);	
      Souliss_DigOut(10, Souliss_T1n_Coil, memory_map, LEDLOGICN2_N1);	
      Souliss_DigOut(11, Souliss_T1n_Coil, memory_map, LEDLOGICN3_N1);	
      Souliss_DigOut(12, Souliss_T1n_Coil, memory_map, LEDLOGICN4_N1);	
      Souliss_DigOut(13, Souliss_T1n_Coil, memory_map, LEDLOGICN5_N1);	
    } 

    FAST_70ms() {  
		ProcessCommunication();										
		}

    FAST_510ms(){
                ssCommunicationChannels();
                }

    FAST_710ms(){
                ssGetTypicals();	
                }

        // Execute the code every 51 time_base_fast		  
    			
  }
  EXECUTESLOW() {
		UPDATESLOW();

		SLOW_10s() {		// We handle the light timer with a 10 seconds base time
			// Timer_SimpleLight(MYLIGHT);					
		} 	  	
	}	
} 


