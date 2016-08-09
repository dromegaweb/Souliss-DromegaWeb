
#ifndef DromegaWebConf.h
#define DromegaWebConf.h


// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------

#define ip_Router		  1     // indirizzo ip Ethernet Router
#define	ip_Gateway		 11		// indirizzo ip Ethernet Gateway  Nodo01  
#define ip_Eth_N2		 12		// indirizzo ip Ethernet Bridge01 Nodo02
#define	ip_Eth_N3		 13		// indirizzo ip Ethernet Bridge02 Nodo03

uint8_t ip_address_Gw[4]  = {192, 168, 2, ip_Gateway};	// indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, ip_Eth_N2};	// indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, ip_Eth_N3};	// indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, ip_Router};// indirizzo Gateway router

#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		  0xFF00
#define myvNet_supern		  0x0000
#define	myvNet_supern_E2	Nodo02_Bridge01_RS485
#define	myvNet_supern_E3	Nodo03_Bridge02_RS485

#define Nodo02_Bridge01_RS485		    0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_Bridge01_Peer01			0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_Bridge01_Peer02			0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_Bridge02_RS485		    0xCF01	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_Bridge02_Peer01			0xCF02	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_Bridge02_Peer02			0xCF03	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_Bridge02_Peer03			0xCF04	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------

#endif


