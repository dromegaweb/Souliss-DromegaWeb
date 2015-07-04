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
#define MYLEDLOGIC          0               

// Define the network configuration according 
// to your router settings
uint8_t ip_address[4]  = {192, 168, 2, 10};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 2, 1};
#define ip_Gateway 10
#define myvNet_address  ip_address[3]  
#define myvNet_subnet   0xFF00
#define myvNet_supern   ip_Gateway

// **** Define here the right pin for your ESP module **** 
#define OUTPUTPIN   0

void setup()
{   
    Initialize();

     // Set network parameters
    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
    SetAsGateway(myvNet_address);  
    
    // Define a simple LED light logic
    Set_SimpleLight(MYLEDLOGIC);        
    
    pinMode(OUTPUTPIN, OUTPUT);  // Power the LED
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        // We process the logic and relevant input 
        // and output every 50 milliseconds
        FAST_50ms() {   
            // Drive the LED as per command
            Logic_SimpleLight(MYLEDLOGIC);    
            DigOut(OUTPUTPIN, Souliss_T1n_Coil, MYLEDLOGIC);                
        } 
              
        // Here we handle here the communication
        FAST_GatewayComms();                                        
        
    }
} 
