
/**************************************************************************
* Utilizzo   :    Nodo CANTINA
* Progetto   :    Nodo03 - Arduino uno + Ethernet W5100 + RS485
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo è un Bridge ethrnet con RS485 verso i peer
* 
* Data       :    21 Giugno 2015   -  Prima bozza
*                 22 Dicembre 2015 - stesura I/O necessari
*                 23 Luglio 2016 - aggiunta sensore DHT11 
*
**
** NOTE : Pin riservati 
*               RS485     ->  0,1,3
**              DHT11     ->  A0
*               OUTPUT    ->  6,7,8,9
*               INPUT     ->  4,5
**
****************************************************************************/

#define USARTDRIVER_INSKETCH  // La RS485 utilizza i pin 0,1,3              
#define	USARTDRIVER		Serial	// Dico al driver vNet di usare la seriale 0 dell'UNO
#define USART_TXENABLE  1
#define USART_TXENPIN   3     // Pin 3 riservato ad enable seriale RS485 con 0 e 1 per RX e TX
#define USART_DEBUG     0

// Indirizzo MAC per Scheda Ethernet Nodo03
#define MAC_INSKETCH                                           // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0x03};  // l'ultimo indirizzo "0x03" identifica il Nodo03
#define AUTO_MAC         0
#define MAC_DEBUG        0

// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard.
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/usart.h"                // uso la seriale
#include "conf/SuperNode.h"           // The main node is the SuperNode





#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"

void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_E3, subnet_mask, ip_gateway_Router);        // set IP a 192.168.2.13
  Souliss_SetAddress(Nodo03_Bridge02_RS485, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    //FAST_BridgeComms();  
     
    START_PeerJoin();
    
  }
}
