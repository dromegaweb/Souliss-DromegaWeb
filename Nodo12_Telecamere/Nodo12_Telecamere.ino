
/**************************************************************************
* Utilizzo   :    Nodo 12 -  Telecamere
* Progetto   :    Nodo03 - Arduino uno + ENC28j60
* Autore     :    DromegaWeb
* Scopo      :    Gestione Pir ingresso, sensore crepuscolare
* 
* Data       :    10 Agosto 2017
* 
*  
**
** NOTE : Pin riservati 
*               Fotoresistenza per crepuscolare   ->  
**              PIR     ->   
*               OUTPUT    ->  6,7,8,9
*               INPUT     ->  4,5
**
****************************************************************************/
// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Indirizzo MAC per Scheda Ethernet Nodo12
#define MAC_INSKETCH                                           // "0xBF" identifica la mia rete BF 
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0x12};  // l'ultimo indirizzo "0x12" identifica il Nodo12
#define AUTO_MAC         0
#define MAC_DEBUG        0

// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard.
#include "conf/ethENC28J60.h"              // Uso connessione Ethernet con ENC28J60

#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"

void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_E12, subnet_mask, ip_gateway_Router);        // set IP Nodo12
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    //FAST_BridgeComms();  
     
    START_PeerJoin();
    
  }
}
