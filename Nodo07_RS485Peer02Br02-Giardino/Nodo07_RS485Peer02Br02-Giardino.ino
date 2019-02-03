/**************************************************************************
* Utilizzo   :    Nodo Giardino
* Progetto   :    Nodo07 Peer01 RS485 su Bridge02 (Nodo03) 
*					       Arduino Standalone Programmato con USBasp
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo è un peer su RS485
* Data       :    30 Aprile 2018    -  Prima bozza 
*                 
* 
** NOTE : Pin riservati 
*               RS485     ->  0,1,3
*                               0 ->  RO
*                               1 ->  DI
*                               3 ->  DE,RE
**              DHT22     ->  2    DHT 22 AM2302 T
*               OUTPUT    ->  4,5,6
*                               4 -> Illuminazione Ingresso  (Uscita PWM)
*                               5 -> Illuminazione Ingresso  (Uscita PWM)
*                               6 -> Illuminazione Giardino  (Uscita PWM) 
*               INPUT     ->  9,7,8
*                               9 -> PIR direzione Giardino
*                               7 -> PIR direzione Ingresso
*                               8 -> PIR direzione Ingresso
*                             
*              
**
****************************************************************************/
// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

//*******INIZIO*************Set RS845 ***************
        #define USARTDRIVER_INSKETCH  // La RS485 utilizza i pin 0,1,3
        #define  USARTDRIVER   Serial  // Dico al driver vNet di usare la seriale 0 dell'UNO
        #define USART_TXENABLE  1
        #define USART_TXENPIN   3     // Pin 3 riservato ad enable seriale RS485 con 0 e 1 per RX e TX
        #define USART_DEBUG     0
//*******FINE *************Set RS845 ***************


// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard.
#include "conf/usart.h"               // Uso la seriale
#include <SPI.h>
#include <Adafruit_Sensor.h>          //Usata dal DHT
#include <DHT_U.h>                    //Usata dal DHT
#include "Souliss.h"
#include "DromegaWebConf.h"           // Contiene set IP del nodo

#define LIGHT_ON_CYCLE          10          // Light ON for 10 cycles if triggered by a PIR sensor

    //TYPICALS
#define Luce_Giardino     0   // 1 Slot
#define Luce_Ingresso     1   // 1 Slot
#define TEMPERATURE       2   // 2 Slot
#define HUMIDITY          4   // 2 Slot

    // PIN SETTING
#define DHTPIN                2  // in - utilizzato per il sensore DHT22 
#define Pin_Luce_Ingresso     5  // out
#define Pin_Luce_Giardino     6  // out
#define Pin_PIR_Ingresso_1    7  // in
#define Pin_PIR_Ingresso_2    8  // in
#define Pin_PIR_Giardino      9  // in

    // DHT
#define DHTTYPE DHT22        // DHT 22 AM2302 tipo di DHT usato    
DHT dht(DHTPIN, DHTTYPE, 15);

void setup()
{
  Initialize();
  INIT_Nodo07();   //Mappa rete nodo vedi DromegaWebConf.h
  dht.begin();    //inizializza DHT
  
  // T y p i c a l s
  Set_T12(Luce_Giardino);
  Set_T12(Luce_Ingresso);       
  Set_Temperature(TEMPERATURE);
  Set_Humidity(HUMIDITY);
  //  I N P U T
  pinMode(DHTPIN, INPUT);    // ingresso DHT 22 , sensore temp_um 
  pinMode(Pin_PIR_Giardino,   INPUT);    // PIR ditezione giarnino
  pinMode(Pin_PIR_Ingresso_1, INPUT);    // PIR ditezione Ingresso
  pinMode(Pin_PIR_Ingresso_2, INPUT);    // PIR ditezione Ingresso
  //  O U T P U T
  pinMode(Pin_Luce_Giardino, OUTPUT);    // Illuminazione Giardino   (Uscita PWM)
  pinMode(Pin_Luce_Ingresso, OUTPUT);    // Illuminazione Ingresso  (Uscita PWM)
    
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    
     FAST_90ms()  {
  
        DigIn( Pin_PIR_Giardino,   Souliss_T1n_AutoCmd + 10 , Luce_Giardino);    	//PIR Luce_Giardino
        DigIn( Pin_PIR_Ingresso_1 ,  Souliss_T1n_AutoCmd + 10  , Luce_Ingresso);  //PIR Luce_Ingresso  
        DigIn( Pin_PIR_Ingresso_2 ,  Souliss_T1n_AutoCmd + 10  , Luce_Ingresso);  //PIR Luce_Ingresso  
      
        Logic_T12(Luce_Giardino);
        Logic_T12(Luce_Ingresso);
                   
        nDigOut(Pin_Luce_Giardino, Souliss_T1n_Coil, Luce_Giardino);      //Luce_Giardino
        nDigOut(Pin_Luce_Ingresso, Souliss_T1n_Coil, Luce_Ingresso);      //Luce_Ingresso       

      }    
    FAST_2110ms() {
           Logic_Temperature(TEMPERATURE);
           Logic_Humidity(HUMIDITY);
        }        
    FAST_7110ms() {
          Timer_T12(Luce_Giardino);
          Timer_T12(Luce_Ingresso);
        }          
    FAST_PeerComms();
    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		//  timer con un tempo base di 10 secondi
        Souliss_ReadDHT(TEMPERATURE, HUMIDITY); 
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}

void Souliss_ReadDHT(uint8_t TEMPERATURE_SLOT, uint8_t HUMIDITY_SLOT){
     //Legge la temperatura e l'umidutà da DHT ogni 10 secondi  
    float h = dht.readHumidity();     // Legge l'umidità
    float t = dht.readTemperature();  // Legge la temperatura in gradi Celsius
    ImportAnalog(TEMPERATURE_SLOT, &t);
    ImportAnalog(HUMIDITY_SLOT, &h); 
}




