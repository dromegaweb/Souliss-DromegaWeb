/*  File di configurazione generale DromegaWebConf.h
*	elenco di paraletri comuni a tutti i nodi
*	revisioni:
*			16-03-2018  - sistemazione generale e sintassi
*
*
*/



#ifndef DromegaWebConf.h
#define DromegaWebConf.h

//---  Accesso WIFI -----
#define WIFICONF_INSKETCH
#define WiFi_SSID               "DrHouse"
#define WiFi_Password           "bf14473bf"

// -------------INIZIO configurazione di rete DromegaWeb------------------------------------

#define Add_Router		1     	// indirizzo ip Ethernet Router
#define	Add_Gateway		51		// indirizzo ip Ethernet Gateway Arduino Mega  Nodo01  
#define Add_Eth_N2		52		// indirizzo ip Ethernet Bridge01 Nodo02
#define	Add_Eth_N3		53		// indirizzo ip Ethernet Bridge02 Nodo03
#define	Add_Esp01_01		54		// indirizzo ip Ethernet Esp01 Allarme con 1 PIR

uint8_t ip_Gw[4]  = {192, 168, 2, Add_Gateway}; 
uint8_t ip_E2[4]  = {192, 168, 2, Add_Eth_N2};	
uint8_t ip_E3[4]  = {192, 168, 2, Add_Eth_N3};	
uint8_t ip_Esp01[4]  = {192, 168, 2, Add_Esp01_01};	

uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, Add_Router};// indirizzo Gateway router

#define myvNet_address		ip_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		  0xFF00
#define myvNet_supern		  0x0000
#define	myvNet_supern_E2	Nodo02_Bridge01_RS485
#define	myvNet_supern_E3	Nodo03_Bridge02_RS485

#define Nodo02_Bridge01_RS485		    0xCE00	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_Bridge01_Peer01			0xCE01	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_Bridge01_Peer02			0xCE02	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_Bridge02_RS485		    0xCF00	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_Bridge02_Peer01			0xCF01	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_Bridge02_Peer02			0xCF02	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_Bridge02_Peer03			0xCF03	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE configurazione di rete DromegaWeb------------------------------------

#endif


