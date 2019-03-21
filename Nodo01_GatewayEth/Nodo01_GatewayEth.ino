/**************************************************************************
*
* Progetto   :    Nodo01 Eth - Gateway - Arduino Mega 2560 + Ethernet W5100
* Autore     :    DromegaWeb
* Data       :    Gennaio 2015     -  Inizio - esito OK
*                 21 Marzo 2015    -  Introdotto SpeakEasy - esito OK
*                 02 maggio 2015   -  Aggiornato ad arduino 1.6.3
*                 28 Dic 2015      -  Aggiornato parametri MAC
*                 29/03/2016       -  Attivazione interfaccia HTML/XML
*                 08 Aprile 2017   -  Aggiornamento sistema di compilazione
*                 05 Nov 2017      -  Disabilitato tutto , Uso solo come Gateway per test h24
*                                       e NODO_03
*                 01 Feb 2019      -  Ristruttirazione sistema
*                 09 Mar 2019      -  Aggiunto Nodo 13 
*
****************************************************************************/

//#define DW_DEBUG            // Se attivo abilita il debug


// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/ethW5100.h"                  // Ethernet through Wiznet W5100
#include "conf/Gateway.h"                   // The main node is the Gateway
#include <SPI.h>
#include "Souliss.h"
#include "DromegaWebConf.h"

// Typical
#define ALLARME    0   
#define INPUTPIN   0

int WachDog_LED_pin = 7;        // Led di verifica funzionamento CPU
int value = 1;                  

void setup()
{
  pinMode(WachDog_LED_pin,OUTPUT);
  pinMode(INPUTPIN, INPUT);  // ingersso allarme
  digitalWrite(WachDog_LED_pin, value); // Accende il LED di WachDog nela fase di setup
  Initialize();
  INIT_Nodo01_TEST();
  Set_T41(ALLARME);
}

void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();
      FAST_510ms() {   
                DigIn(INPUTPIN, Souliss_T4n_Alarm, ALLARME);    
                Logic_T41(ALLARME);                                      
          }

      FAST_1110ms() {
        value = !value;
        digitalWrite(WachDog_LED_pin, value);        //Lampeggio di WachDog_LED nela fase di lavoro
      }
      
    FAST_GatewayComms();   //Elaborazione di tutte le comunicazioni con altri nodi la Souliss App
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
          #ifdef DW_DEBUG
            Serial.print("IP Gateway:");
          #endif  
    }  
  }
}


