/**************************************************************************
    Souliss - RGB LED Strip Hack
    
    Control and RGB LED or a Strip using the PWM, fade in/out and flash 
    effects are available as the Android interface or openHAB.
 
    Run this code on one of the following boards:
      - RGBW Led Strip Driver based on ESP8266
    
***************************************************************************/

// Configure the framework

#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/IPBroadcast.h"


#include "G:\dromegawebwifi.h"         // **** Define the private WiFi name and password in external file ****   
/*
#define WIFICONF_INSKETCH
#define WiFi_SSID               "********"
#define WiFi_Password           "********"  
*/

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"


#define LEDCONTROL    0               // This is the memory slot for the logic that handle the light
#define LEDRED        1               // This is the memory slot for the logic that handle the light
#define LEDGREEN      2               // This is the memory slot for the logic that handle the light
#define LEDBLUE       3               // This is the memory slot for the logic that handle the light
#define LEDWHITE1			4               // This is the memory slot for the logic that handle the light
#define LEDWHITE2			6               // This is the memory slot for the logic that handle the light
/*
uint8_t ip_address[4]  = {192, 168, 2, 11};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 2, 1};
#define ip_Gateway 10
#define myvNet_address  ip_address[3]  
#define myvNet_subnet   0xFF00
#define myvNet_supern   ip_Gateway
*/

void setup()
{   
    Initialize();
    analogWriteFreq(500);  // Imposta la frequenza di PWM
    analogWriteRange(255);    // riduce la scala PWN da 1024 campioni a 255 per adattarlo a SoulissApp

    // Set network parameters
    //Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
    //SetAsGateway(myvNet_address);  

    // Connect to the WiFi network and get an address from DHCP
    GetIPAddress();                           
    SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp 
  
                
    Set_LED_Strip(LEDCONTROL);                  // Set a logic to control a LED strip
	  Set_DimmableLight(LEDWHITE1);
	  Set_DimmableLight(LEDWHITE2);
   
    // Define inputs, outputs pins
    pinMode(16, OUTPUT);                 // Power the LED BLUE
    pinMode(14, OUTPUT);                 // Power the LED GREEN
    pinMode(12, OUTPUT);                 // Power the LED RED   
    pinMode(13, OUTPUT);                 // Power the LED
    pinMode(4, OUTPUT);                 // Power the LED 
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   

        // Execute the code every 1 time_base_fast      
        FAST_10ms() {
                    
            // Execute the logic that handle the LED
            Logic_LED_Strip(LEDCONTROL);
			      Logic_DimmableLight(LEDWHITE1);
			      Logic_DimmableLight(LEDWHITE2);

            // Use the output values to control the PWM
            analogWrite(12, mOutput(LEDRED));
            analogWrite(14, mOutput(LEDGREEN));
            analogWrite(16, mOutput(LEDBLUE));
            analogWrite(13, mOutput(LEDWHITE1));
            analogWrite(4, mOutput(LEDWHITE2));

            // Just process communication as fast as the logics
            ProcessCommunication();
        } 

		// Process the other Gateway stuffs
		FAST_GatewayComms();
          
    }
    EXECUTESLOW()
    {   
        UPDATESLOW();

        SLOW_10s()  {
        
            // The timer handle timed-on states
            Timer_LED_Strip(LEDCONTROL); 
			      Timer_DimmableLight(LEDWHITE1);
			      Timer_DimmableLight(LEDWHITE2);                       
        }     
    }       
} 
