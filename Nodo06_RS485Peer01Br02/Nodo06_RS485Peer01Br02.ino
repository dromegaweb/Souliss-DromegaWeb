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
  
// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------

#define ip_Router		10
#define	ip_Gateway		11			// indirizzo ip Gateway Souliss   
#define ip_Eth_N2		12			// indirizzo ip su Ethernet Nodo02
#define	ip_Eth_N3		13			// indirizzo ip su Ethernet Nodo03

uint8_t ip_address_Gw[4]  = {192, 168, 2, ip_Gateway};		// indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, ip_Eth_N2};		// indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, ip_Eth_N3};		// indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, ip_Router};	// indirizzo Gateway router

#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		0xFF00
#define myvNet_supern		0x0000
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






