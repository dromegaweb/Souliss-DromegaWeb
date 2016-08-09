/**************************************************************************
 * 
 * Progetto   :    Nodo05 Peer02 RS485 su Bridge01 (Nodo02) - Arduino Uno standalone
 * Autore     :    DromegaWeb
 * Data       :    25 Marzo 2015    -  Inizio - esito OK
 *                 29 Marzo 2015    -  Aggiunto gestione Usart InSketch - esito OK
 *                 03 aprile 2015   -  Aggiunto 2 PIR in ingresso
 *                 02 maggio 2015   -  Aggiornato a arduino 1.6.3
 * 
 * 
 ***************************************************************************/
  
#define USARTDRIVER_INSKETCH
#define USARTDRIVER	        Serial	//Dico al driver vNet di usare la seriale 0 dell'Arduino UNO
#define USART_TXENABLE      1
#define USART_TXENPIN		    3
#define USART_DEBUG  		    0
  
// Configure the framework
#include "bconf/StandardArduino.h"			// Uso un Arduino standard
#include "conf/usart.h"	                        // uso la seriale
    
#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"

#define LIGHT1_N5      0
#define LIGHT2_N5	     1
#define LIGHT3_N5	     2
#define LIGHT4_N5	     3
#define LIGHT5_N5	     4
#define ALLARME        5
#define WATCHDOG       6

void setup()
{
  Initialize();
  Souliss_SetAddress(Nodo05_Bridge01_Peer02, myvNet_subnet, Nodo02_Bridge01_RS485);

  Set_T11(LIGHT1_N5);
  Set_T11(LIGHT2_N5);
  Set_T11(LIGHT3_N5);
  Set_T11(LIGHT4_N5);
  Set_T11(LIGHT5_N5);

                                        //  pinMode(7,INPUT);
                                        //  pinMode(8,INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);					
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);					
  pinMode(10,OUTPUT);

   // Set tipici e I/O ALLARME  
  Set_T42(ALLARME);      // setta l'allarme
  pinMode(11, INPUT);    // ingressso pull-down per sensore d'allarme PIR
  
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_50ms() {
                                                   //   DigIn(7,Souliss_T1n_ToggleCmd,LIGHT1_N5);	
                                                   //   DigIn(8,Souliss_T1n_ToggleCmd,LIGHT2_N5);	
      Logic_T11(LIGHT1_N5);
      Logic_T11(LIGHT2_N5);
      Logic_T11(LIGHT3_N5);
      Logic_T11(LIGHT4_N5);
      Logic_T11(LIGHT5_N5);
      
      DigOut(6, Souliss_T1n_Coil, LIGHT1_N5);      
      DigOut(7, Souliss_T1n_Coil, LIGHT2_N5);      	
      DigOut(8, Souliss_T1n_Coil, LIGHT3_N5);
      DigOut(9, Souliss_T1n_Coil, LIGHT4_N5);      	
      DigOut(10, Souliss_T1n_Coil, LIGHT5_N5);
    }

    FAST_510ms() {
      DigIn(11, Souliss_T4n_Alarm, ALLARME);   // ingressso pull-down per sensore d'allarme
      Logic_T42(ALLARME, ip_Gateway);  //esegue locica allarme
        }
        
    FAST_2110ms()   {   // watchdog di controllo presenza dei nodi             
      mInput(ALLARME) = Watchdog(ip_Gateway, WATCHDOG, Souliss_T4n_Alarm);
        }

    FAST_PeerComms()

    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		// Gestiamo il timer con un tempo base di 10 secondi
        Timer_T11(LIGHT1_N5);
        Timer_T11(LIGHT2_N5);
        Timer_T11(LIGHT3_N5);
        Timer_T11(LIGHT4_N5);
        Timer_T11(LIGHT5_N5);

    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }

}






