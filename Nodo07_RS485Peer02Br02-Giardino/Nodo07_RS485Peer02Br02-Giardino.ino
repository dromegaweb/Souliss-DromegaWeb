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
#include <DHT.h>
#include "Souliss.h"
#include "DromegaWebConf.h"


#define DHTPIN 2             // pin per sensore DHT11    
#define DHTTYPE DHT22        // DHT 22 AM2302 tipo di DHT usato
#define TEMPERATURE     0
#define HUMIDITY        2
DHT dht(DHTPIN, DHTTYPE, 15);


void setup()
{
  Initialize();
  Souliss_SetAddress(Nodo07_Bridge02_Peer02, myvNet_subnet, Nodo03_Bridge02_RS485);
  dht.begin();

  
// Define inputs, outputs pins

  pinMode(2, INPUT);     // ingresso DHT 22 , sensore temp_um 

  Set_Temperature(TEMPERATURE);
  Set_Humidity(HUMIDITY);
  
}

void loop()
{
  EXECUTEFAST() {						
    UPDATEFAST();
    FAST_10ms() {
      
        //   ProcessCommunication();   	
    }
    FAST_2110ms() {
           Logic_Temperature(TEMPERATURE);
           Logic_Humidity(HUMIDITY);
        }    
    FAST_PeerComms();
    
    START_PeerJoin();

  }

  EXECUTESLOW() {
    UPDATESLOW();

    SLOW_10s() {		// Gestiamo il timer con un tempo base di 10 secondi
        
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




