/**************************************************************************
 * 
 * Progetto   :    Nodo06 Peer01 RS485 su Bridge02 (Nodo03) - Arduino Pro Mini
 * Autore     :    DromegaWeb
 * Data       :    27 Giugno 2015    -  Prima bozza 
 *                 05 Luglio 2015    -  Aggiunto RGB esito OK ma non su android 2.2
 * 
 ***************************************************************************/
  
#define USARTDRIVER_INSKETCH
#define USARTDRIVER	    Serial	//Dico al driver vNet di usare la seriale 0 dell'Arduino UNO
#define USART_TXENABLE    1
#define USART_TXENPIN	    3
#define USART_DEBUG  	    0


// Configure the framework
#include "bconf/StandardArduino.h"			// Uso un Arduino standard
#include "conf/usart.h"	                        // uso la seriale


#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"

#define LEDCONTROL          0               // This is the memory slot for the logic that handle the light
#define LEDRED              1               // This is the memory slot for the logic that handle the light
#define LEDGREEN            2               // This is the memory slot for the logic that handle the light
#define LEDBLUE             3               // This is the memory slot for the logic that handle the light



void setup()
{
  Initialize();
  Souliss_SetAddress(Nodo06_Bridge02_Peer01, myvNet_subnet, Nodo03_Bridge02_RS485);

  Set_LED_Strip(LEDCONTROL);                  // Set a logic to control a LED strip


     // Define inputs, outputs pins
    pinMode(7, INPUT);                  // Hardware pulldown required
    pinMode(4, OUTPUT);                 // Power the LED
    pinMode(5, OUTPUT);                 // Power the LED
    pinMode(6, OUTPUT);                 // Power the LED  
  
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_10ms() {
      
            DigIn(7, Souliss_T1n_ToggleCmd, LEDCONTROL);        
            
            // Execute the logic that handle the LED
            Logic_LED_Strip(LEDCONTROL);

            // Use the output values to control the PWM
            analogWrite(4, mOutput(LEDRED));
            analogWrite(5, mOutput(LEDGREEN));
            analogWrite(6, mOutput(LEDBLUE));   
            ProcessCommunication();   	
   
    }

    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		// Gestiamo il timer con un tempo base di 10 secondi
        
            // The timer handle timed-on states
            Timer_LED_Strip(LEDCONTROL); 
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }

}






