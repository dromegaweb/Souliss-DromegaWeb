/**************************************************************************
    Souliss - Hello World
    
    This is the basic example, control one LED via a push-button or Android
    using SoulissApp (get it from Play Store).  
    
    Run this code on ESP8266 as WiFi SoC.
        
***************************************************************************/

// Configure the framework
#include "bconf/MCU_ESP8266.h"          
#include "conf/Gateway.h"                  
#include "conf/IPBroadcast.h"

// **** Define the WiFi name and password ****
#define WIFICONF_INSKETCH
#define WiFi_SSID               "DrHouse"
#define WiFi_Password           "bf14473bf"   

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define ALLARME          0               

// Define the network configuration according 
// to your router settings
uint8_t ip_address[4]  = {192, 168, 2, 10};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 2, 1};
#define ip_Gateway 10
#define myvNet_address  ip_address[3]  
#define myvNet_subnet   0xFF00
#define myvNet_supern   ip_Gateway

// **** Definiti i pin del modulo ESP **** 
#define INPUTPIN   0

void setup()
{   
    Initialize();

     // Set network parameters
    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
    SetAsGateway(myvNet_address);  
    
    // Definisce logica ALLARME 
    Set_T41(ALLARME);        
    
    pinMode(INPUTPIN, INPUT);  // ingersso allarme
}

void loop()
{ 
    // Inizio ciclo
    EXECUTEFAST() {                     
        UPDATEFAST();   
        FAST_510ms() {   
              DigIn(INPUTPIN, Souliss_T4n_Alarm, ALLARME);    // Drive the LED as per command
              Logic_T41(ALLARME);                                // Execute the anti-theft logic        
        } 
        FAST_GatewayComms();                                        
        
    }
} 
