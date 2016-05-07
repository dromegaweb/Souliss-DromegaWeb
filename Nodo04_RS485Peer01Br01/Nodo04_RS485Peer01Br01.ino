/**************************************************************************
* 
* Progetto   :    Nodo04 Peer01 RS485 su Bridge01 (Nodo02) - Arduino Uno standalone
* Autore     :    DromegaWeb
* Data       :    22 Marzo 2015    -  Inizio - esito OK
*                 29 Marzo 2015    -  Aggiunto gestione Usart InSketch - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*
****************************************************************************/

  #define USARTDRIVER_INSKETCH
  #define USARTDRIVER         Serial	//Dico al driver vNet di usare la seriale 0 dell'Arduino UNO
  #define USART_TXENABLE      1
  #define USART_TXENPIN		    3
  #define USART_DEBUG  		    0
  
  
  // Configure the framework
  #include "bconf/StandardArduino.h"			// Uso un Arduino standard
  #include "conf/usart.h"	                        // uso la seriale
  
  
  #include <SPI.h>
  #include "Souliss.h"

// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------

#define ip_Router		10
#define	ip_Gateway	11			// indirizzo ip Gateway Souliss   
#define ip_Eth_N2		12			// indirizzo ip su Ethernet Nodo02
#define	ip_Eth_N3		13			// indirizzo ip su Ethernet Nodo03

uint8_t ip_address_Gw[4]  = {192, 168, 2, ip_Gateway};		// indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, ip_Eth_N2};		// indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, ip_Eth_N3};		// indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, ip_Router};	// indirizzo Gateway router

#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		  0xFF00
#define myvNet_supern		  0x0000
#define	myvNet_supern_E2	Nodo02_Bridge01_RS485
#define	myvNet_supern_E3	Nodo03_Bridge02_RS485

#define Nodo02_Bridge01_RS485		0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_Bridge01_Peer01			0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_Bridge01_Peer02			0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_Bridge02_RS485		0xCF01	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_Bridge02_Peer01			0xCF02	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_Bridge02_Peer02			0xCF03	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_Bridge02_Peer03			0xCF04	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------


#define LIGHT1_N4      0
#define LIGHT2_N4	     1
#define LIGHT3_N4      2
#define LIGHT4_N4	     3
#define ALLARME        4
#define WATCHDOG       5



void setup()
{
  Initialize();
  Souliss_SetAddress(Nodo04_Bridge01_Peer01, myvNet_subnet, Nodo02_Bridge01_RS485);

 // Set tipici e I/O LED
  Set_T11(LIGHT1_N4);
  Set_T11(LIGHT2_N4);
  Set_T11(LIGHT3_N4);
  Set_T11(LIGHT4_N4);
  
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  	
  pinMode(6, OUTPUT);					
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);					
  pinMode(9, OUTPUT);
  
 // Set tipici e I/O ALLARME  
  Set_T42(ALLARME);      // setta l'allarme
  pinMode(11, INPUT);    // ingressso pull-down per sensore d'allarme PIR

}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_50ms() {      
      DigIn(4, Souliss_T1n_ToggleCmd, LIGHT1_N4);		
      DigIn(5, Souliss_T1n_ToggleCmd, LIGHT2_N4);
      Logic_T11( LIGHT1_N4);
      Logic_T11( LIGHT2_N4);
      Logic_T11( LIGHT3_N4);
      Logic_T11( LIGHT4_N4);      
      DigOut(6, Souliss_T1n_Coil, LIGHT1_N4);      	
      DigOut(7, Souliss_T1n_Coil, LIGHT2_N4);
      DigOut(8, Souliss_T1n_Coil, LIGHT3_N4);      	
      DigOut(9, Souliss_T1n_Coil, LIGHT4_N4);

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

    SLOW_10s() {		// We handle the light timer with a 10 seconds base time
        Timer_T11(LIGHT1_N4);
        Timer_T11(LIGHT2_N4);
        Timer_T11(LIGHT3_N4);
        Timer_T11(LIGHT4_N4);
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }

}





