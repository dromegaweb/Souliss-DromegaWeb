/**************************************************************************
* Utilizzo   :    Nodo CANTINA
* Progetto   :    Nodo03 - Arduino uno + Ethernet W5100 + RS485
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo è un Bridge ethrnet con RS485 verso i peer
* 
* Data       :    21 Giugno 2015   -  Prima bozza
*                 22 Dicembre 2015 -  Stesura I/O necessari
*                 23 Luglio 2016   -  Aggiunta sensore DHT11 
*                 02 Aprile 2017   -  Aggiornamento sistema di compilazione
*                 30 Aprile 2018   -  Ultimato Funzionante
*                 01 Feb 2019      -  Controllato Funzionante
**
** NOTE : Pin riservati 
*               RS485     ->  0,1,3
*                               0 ->  RO
*                               1 ->  DI
*                               3 ->  DE,RE
**              W5100     ->  10,11,12,13
**              DHT11     ->  A0
*               OUTPUT    ->  6,7,8,9,A2,A3
*                                6 -> Valvola irrigazione zona 01
*                                7 -> Valvola irrigazione zona 02
*                                8 -> Luce Cantina
*                                9 -> Luce Vini
*                               A2 -> Ventola scambio termico -> utilizzo ingresso analogico come uscita
*                               A3 -> Ventola anti umidità    -> utilizzo ingresso analogico come uscita
*               INPUT     ->  2,5
*                               2 -> PIR Luci Cantina
*                               5 -> PIR Luci Vini
*                             
*               
**
****************************************************************************/
// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

//*******INIZIO*************Set RS845 ***************
        #define USARTDRIVER_INSKETCH  // La RS485 utilizza i pin 0,1,3
        #define	USARTDRIVER		Serial	// Dico al driver vNet di usare la seriale 0 dell'UNO
        #define USART_TXENABLE  1
        #define USART_TXENPIN   3     // Pin 3 riservato ad enable seriale RS485 con 0 e 1 per RX e TX
        #define USART_DEBUG     0
//*******FINE *************Set RS845 ***************


// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard.
#include "conf/ethW5100.h"            // Uso connessione Ethernet Wiznet W5100
#include "conf/SuperNode.h"           // Include i parametri per il SuperNode
#include "conf/usart.h"               // Uso la seriale
#include <SPI.h>
#include <Adafruit_Sensor.h>          //Usata dal DHT
#include <DHT_U.h>                    //Usata dal DHT
#include "Souliss.h"
#include "DromegaWebConf.h"           // Contiene set IP del nodo

    //TYPICALS
#define Luci_Cantina	  0   // 1 Slot
#define Luci_Vini       1   // 1 Slot
#define VALVOLA_01      2   // 1 Slot
#define VALVOLA_02      3   // 1 Slot
#define VENTOLA_TEMP    4   // 1 Slot
#define VENTOLA_UMID    5   // 1 Slot
#define TEMPERATURE     6   // 2 Slot
#define HUMIDITY        8   // 2 Slot

    // PIN SETTING
#define Pin_PIR_Luci_Cantina    2  // in
#define Pin_PIR_Luci_Vini       5  // in
#define Pin_VALVOLA_01          6  // out
#define Pin_VALVOLA_02          7  // out
#define Pin_Luci_Cantina        8  // out
#define Pin_Luci_Vini           9  // out
#define Pin_VENTOLA_TEMP        A2  // out
#define Pin_VENTOLA_UMID        A3  // out
#define Pin_DHT_11              A0  // in

    // DHT
#define DHTPIN Pin_DHT_11     // Per carenza di ingressi uso un pin analogico da connettere al sensore DHT11
#define DHTTYPE DHT11         // DHT 11      tipo di DHT usato
//#define DHTTYPE  DHT22      // DHT 22 (AM2302), AM2321
//#define DHTTYPE  DHT21      // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE, 15);


void setup()
{ 
  Initialize();
  INIT_Nodo03();  //Mappa rete nodo vedi DromegaWebConf.h
  dht.begin();    //inizializza DHT
  
  // T y p i c a l s
  Set_T12(Luci_Cantina);      
  Set_T12(Luci_Vini);
  Set_T11(VALVOLA_01);
  Set_T11(VALVOLA_02);
  Set_T11(VENTOLA_TEMP);
  Set_T11(VENTOLA_UMID);
  Set_Temperature(TEMPERATURE);
  Set_Humidity(HUMIDITY);
  //  I N P U T
  pinMode(Pin_PIR_Luci_Cantina, INPUT); // PIR Luci Cantina
  pinMode(Pin_PIR_Luci_Vini, INPUT);    // PIR Luci Vini
  pinMode(Pin_DHT_11, INPUT);           // ingresso DHT 11 , sensore temp_um  OK funziona su ingresso analogico (07-08-2016)
  //  O U T P U T
  pinMode(Pin_Luci_Cantina, OUTPUT);    // Luce Cantina
  pinMode(Pin_Luci_Vini, OUTPUT);       // Luce Vini    
  pinMode(Pin_VALVOLA_01, OUTPUT);      // Valvola irrigazione zona 01
  pinMode(Pin_VALVOLA_02, OUTPUT);      // Valvola irrigazione zona 02
  pinMode(Pin_VENTOLA_TEMP, OUTPUT);    // Ventola scambio termico -> utilizzo ingresso analogico come uscita
  pinMode(Pin_VENTOLA_UMID, OUTPUT);    // Ventola anti umidità    -> utilizzo ingresso analogico come uscita
 
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_90ms()  {

      DigIn( Pin_PIR_Luci_Cantina, Souliss_T1n_AutoCmd + 10 , Luci_Cantina);    //PIR Luci Cantina
      DigIn( Pin_PIR_Luci_Vini,    Souliss_T1n_AutoCmd + 10 , Luci_Vini);       //PIR Luci Vini   
        
      Logic_T12(Luci_Cantina);
      Logic_T12(Luci_Vini);
      Logic_T11(VALVOLA_01);
      Logic_T11(VALVOLA_02);     
      Logic_T11(VENTOLA_TEMP);
      Logic_T11(VENTOLA_UMID);
           
      nDigOut(Pin_Luci_Cantina, Souliss_T1n_Coil, Luci_Cantina);        //Luce Cantina
      nDigOut(Pin_Luci_Vini,    Souliss_T1n_Coil, Luci_Vini);           //Luce Vini        
      DigOut(Pin_VALVOLA_01,   Souliss_T1n_Coil, VALVOLA_01);           //Valvola irrigazione zona 01
      DigOut(Pin_VALVOLA_02,   Souliss_T1n_Coil, VALVOLA_02);           //Valvola irrigazione zona 02         
      DigOut(Pin_VENTOLA_TEMP, Souliss_T1n_Coil, VENTOLA_TEMP);         //Ventola scambio termico
      DigOut(Pin_VENTOLA_UMID, Souliss_T1n_Coil, VENTOLA_UMID);         //Ventola anti umidità  

    }
    
    FAST_2110ms() {
          Logic_Temperature(TEMPERATURE);
          Logic_Humidity(HUMIDITY);
    }

    FAST_7110ms() {
          Timer_T12(Luci_Cantina);
          Timer_T12(Luci_Vini);
    }

    FAST_PeerComms();  
	  START_PeerJoin();
   
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
      Souliss_ReadDHT(TEMPERATURE, HUMIDITY);
    }
  SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}

void Souliss_ReadDHT(uint8_t TEMPERATURE_SLOT, uint8_t HUMIDITY_SLOT){        // Legge la temperatura e l'umidutà da DHT ogni 10 secondi 
    float h = dht.readHumidity();     // Legge l'umidità
    float t = dht.readTemperature();  // Legge la temperatura in gradi Celsius             
    ImportAnalog(TEMPERATURE_SLOT, &t);
    ImportAnalog(HUMIDITY_SLOT, &h); 
 }

 


