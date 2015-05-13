/*

 Progetto   :    Nodo 2 collegamento Ethernet(W5100) + RS485 
 Autore     :     
 Data       :    Inizio Febbraio 2015
 
 */

#include "bconf/inSketch.h"		            // Using the inSketch mode. FUNZIONANTE
#define  QC_BOARDTYPE  		0x04        // Arduino + Ethernet W5100
#define  QC_GATEWAYTYPE  	0x00        // Define board as gateway      
#define  QC_INTERFACE           0x00

// End of configuration block

#include "Souliss.h"
#include "Typicals.h"
#include <SPI.h>

#define  MAC_INSKETCH const uint8_t MAC_ADDRESS[] = {  0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBB};

// network addresses
#define network_address_1	0x000B
#define network_address_2	0x000C
#define network_my_subnet	0xFF00
#define network_my_supern	0x0000

#define LEDLOGICNO_N1		0				// This is the memory slot for the logic that handle the light
#define LEDLOGICN1_N1		1
#define LEDLOGICN2_N1		2
#define LEDLOGICN3_N1		3
#define LEDLOGICN4_N1		4
#define LEDLOGICN5_N1		5
#define LIGHT1_N2            0
#define LIGHT2_N2	     1
#define LIGHT3_N2            2
#define LIGHT4_N2	     3
#define LIGHT5_N2            4
#define LIGHT6_N2	     5


// define the shared memory map
U8 memory_map[MaCaco_MEMMAP];

// flag 
U8 data_changed = 0;

#define time_base_fast		10				// Time cycle in milliseconds
#define time_base_slow		10000			// Time cycle in milliseconds
#define num_phases		255				// Number of phases

U8 phase_speedy=0, phase_fast=0, phase_slow=0;
unsigned long tmr_fast=0, tmr_slow=0;  

void setup()
{	
  // Setup the network configuration
  //  for the board will be 192.168.1.12

  Souliss_SetAddress(network_address_2, network_my_subnet, network_my_supern);	
  Souliss_SetT11(memory_map, LIGHT1_N2);        // Set the typical to use in slot 0 and 1
  Souliss_SetT11(memory_map, LIGHT2_N2);
  Souliss_SetT12(memory_map, LIGHT3_N2);
  Souliss_SetT12(memory_map, LIGHT4_N2);
  Souliss_SetT11(memory_map, LIGHT5_N2);
  Souliss_SetT11(memory_map, LIGHT6_N2);

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

  if(abs(millis()-tmr_fast) > time_base_fast)
  {	
    tmr_fast = millis();
    phase_fast = (phase_fast + 1) % num_phases;

    // Execute the code every 3 time_base_fast		
    if (!(phase_fast % 3))
    {
      Souliss_DigIn(2, Souliss_T1n_ToogleCmd, memory_map, LIGHT1_N2);		
      Souliss_DigIn(3, Souliss_T1n_ToogleCmd, memory_map, LIGHT2_N2);
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

    // Execute the code every 5 time_base_fast		  
    if (!(phase_fast % 5))
    {   
      // Retreive data from the communication channel
      Souliss_CommunicationData(memory_map, &data_changed);		
    }	
  }	
} 

