
/**************************************************************************
* Utilizzo   :    Nodo CANTINA
* Progetto   :    Nodo03 - Arduino Pro Mini + Eth ENC28j60
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
**              ENC28J60  ->  10,11,12,13
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
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethENC28J60.h"         // Uso connessione Ethernet ENC28J60
#include "conf/usart.h"	              // uso la seriale
#include "conf/SuperNode.h"           // The main node is the SuperNode



#include <SPI.h>
#include <DHT.h>
#include "Souliss.h"
#include "DromegaWebConf.h"



#define Luci_Cantina	  0
#define Luci_Vini       1
#define VALVOLA_01      2
#define VALVOLA_02      3
#define VENTOLA_TEMP    4
#define VENTOLA_UMID    5

#define DHTPIN A0 // Per carenza di ingressi uso un pin analogico da connettere al sensore DHT11
#define DHTTYPE DHT11     // DHT 11      tipo di DHT usato
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define TEMPERATURE     6
#define HUMIDITY        8
DHT dht(DHTPIN, DHTTYPE, 15);


void setup()
{
  Initialize();
  Souliss_SetIPAddress(ip_address_E3, subnet_mask, ip_gateway_Router);        // set IP a 13
  Souliss_SetAddress(Nodo03_Bridge02_RS485, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485

  dht.begin();

  Set_AutoLight(Luci_Cantina);
  Set_AutoLight(Luci_Vini);
  Set_SimpleLight(VALVOLA_01);
  Set_SimpleLight(VALVOLA_02);
  Set_SimpleLight(VENTOLA_TEMP);
  Set_SimpleLight(VENTOLA_UMID);
  Set_Temperature(TEMPERATURE);
  Set_Humidity(HUMIDITY);

  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);   //Riservato a seriale RS 485 pin Enable
  
  pinMode(4, INPUT);      //PIR Luci Cantina
  pinMode(5, INPUT);      //PIR Luci Vini
  pinMode(A0, INPUT);     // ingresso DHT 11 , sensore temp_um  OK funziona su ingresso analogico (07-08-2016)

  pinMode(6, OUTPUT);     //Valvola irrigazione zona 01
  pinMode(7, OUTPUT);     //Valvola irrigazione zona 02
  pinMode(8, OUTPUT);     //Luce Cantina
  pinMode(9, OUTPUT);     //Luce Vini
  pinMode(A2, OUTPUT);    //Ventola scambio termico -> utilizzo ingresso analogico come usctita
  pinMode(A3, OUTPUT);    //Ventola anti umidità    -> utilizzo ingresso analogico come usctita

}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_50ms()  {

      DigIn(4, Souliss_T1n_ToggleCmd, Luci_Cantina);     //PIR Luci Cantina
      DigIn(5, Souliss_T1n_ToggleCmd, Luci_Vini);        //PIR Luci Vini   
        
      Logic_AutoLight(Luci_Cantina);
      Logic_AutoLight(Luci_Vini);
      
      DigOut(6, Souliss_T1n_Coil, VALVOLA_01);            //Valvola irrigazione zona 01
      DigOut(7, Souliss_T1n_Coil, VALVOLA_02);            //Valvola irrigazione zona 02
      DigOut(8, Souliss_T1n_Coil, Luci_Cantina);          //Luce Cantina
      DigOut(9, Souliss_T1n_Coil, Luci_Vini);             //Luce Vini  
      DigOut(A2, Souliss_T1n_Coil, VENTOLA_TEMP);         //Ventola scambio termico
      DigOut(A3, Souliss_T1n_Coil, VENTOLA_UMID);         //Ventola anti umidità  
 
    }
    
    FAST_2110ms() {
           Logic_Temperature(TEMPERATURE);
           Logic_Humidity(HUMIDITY);
        }
    
    FAST_BridgeComms();
        
    START_PeerJoin();
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
       Timer_AutoLight(Luci_Cantina);
       Timer_AutoLight(Luci_Vini); 
       Souliss_ReadDHT(TEMPERATURE, HUMIDITY); 
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}

void Souliss_ReadDHT(uint8_t TEMPERATURE_SLOT, uint8_t HUMIDITY_SLOT){
    // Legge la temperatura e l'umidutà da DHT ogni 10 secondi  
    float h = dht.readHumidity();     // Legge l'umidità
    float t = dht.readTemperature();  // Legge la temperatura in gradi Celsius

                
    ImportAnalog(TEMPERATURE_SLOT, &t);
    ImportAnalog(HUMIDITY_SLOT, &h); 
}



