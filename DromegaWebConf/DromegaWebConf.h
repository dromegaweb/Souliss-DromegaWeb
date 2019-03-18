/*  File di configurazione generale DromegaWebConf.h
*	elenco di paraletri comuni a tutti i nodi
*	revisioni:
*			16-03-2018  - sistemazione generale e sintassi
*
*
*/



#ifndef DromegaWebConf.h
#define DromegaWebConf.h


// -------------INIZIO configurazione di rete DromegaWeb------------------------------------

#define Add_Router			1     	// indirizzo ip Ethernet Router
#define	Add_Gateway			60		// indirizzo ip Ethernet Gateway Arduino Mega  Nodo01  
#define Add_Eth_N2			52		// indirizzo ip Ethernet Bridge01 Nodo02
#define	Add_Eth_N3			53		// indirizzo ip Ethernet Bridge02 Nodo03
#define	Add_Esp01_Nodo13	54		// indirizzo ip Ethernet Esp01 Nodo13 Allarme con 1 PIR
#define	Add_Esp01_Nodo14	55		// indirizzo ip Ethernet Esp01 Nodo13 Allarme con 1 PIR


uint8_t ip_Router[4] = {192, 168, 2, Add_Router};// indirizzo router
uint8_t ip_Gw[4]  = {192, 168, 2, Add_Gateway}; 
uint8_t ip_E2[4]  = {192, 168, 2, Add_Eth_N2};	
uint8_t ip_E3[4]  = {192, 168, 2, Add_Eth_N3};	
uint8_t ip_Nodo13[4]  = {192, 168, 2, Add_Esp01_Nodo13};	
uint8_t ip_Nodo14[4]  = {192, 168, 2, Add_Esp01_Nodo14};

uint8_t subnet_mask[4]    = {255, 255, 255, 0};


#define Nodo02_B01		    0xCE00	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_B01_P01		0xCE01	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_B01_P02		0xCE02	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485
#define Nodo03_B02		    0xCF00	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_B02_P01		0xCF01	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_B02_P02		0xCF02	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_B02_P03		0xCF03	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485
#define Nodo01_Gateway		0xAB01
#define Nodo13_ESP01		0xAB02
#define Nodo14_ESP01		0xAB03


#define myvNet_address		ip_Gw[3]		// ultimo byte dell' IP address (51) è anche l'indirizzo vNet
#define	myvNet_subnet		  0xFF00
#define myvNet_supern		  0x0000
#define	myvNet_supern_E2	Nodo02_B01
#define	myvNet_supern_E3	Nodo03_B02
// -------------FINE configurazione di rete DromegaWeb------------------------------------


/*-------------Nodi OK--------INIZIO------------------------------------------------*/
#define INIT_Nodo01()   SetIPAddress(ip_Gw, subnet_mask, ip_Router); \
						SetAsGateway(myvNet_address); \
						SetAsPeerNode(Add_Eth_N3, 1); \
						SetAsPeerNode(Nodo07_B02_P02, 2);
						
#define INIT_Nodo03()   SetIPAddress(ip_E3, subnet_mask, ip_Router); \
						SetAddress(Nodo03_B02, myvNet_subnet, myvNet_supern);
						
#define INIT_Nodo07()  	SetAddress(Nodo07_B02_P02, myvNet_subnet, Nodo03_B02);

/*-------------Nodi OK--------FINE-------------------------------------------------*/



/*-------------Nodi in test----------INIZIO----------------------------------------*/

#define INIT_Nodo01_TEST()  SetIPAddress(ip_Gw, subnet_mask, ip_Router); \
							SetAsGateway(myvNet_address); \
							SetAddress(Nodo01_Gateway, myvNet_subnet, myvNet_supern); \
							SetAsPeerNode(Nodo13_ESP01, 1);
							
#define INIT_Nodo13_TEST()  GetIPAddress(); \
							SetAddress(Nodo13_ESP01, myvNet_subnet, Nodo01_Gateway);
							
#define INIT_Nodo13_TEST1()  SetIPAddress(ip_Nodo13, subnet_mask, ip_Router); \
							SetAddress(Nodo13_ESP01, myvNet_subnet, Nodo01_Gateway);
							
							
							  


/*-------------Nodi in test----------FINE------------------------------------------*/







/*----------------------------------VECCHIO MEMO----------------------------------------*/

/*  
  SetIPAddress(ip_Gw, subnet_mask, ip_Router);    // Questo nodo sarà --> 192.168.2.51
  SetAsGateway(myvNet_address);                                           // Questo nodo è Gateway Souliss app
  SetAsPeerNode(Add_Eth_N2, 1);                   // definisco Nodo02 Eth + brige01 RS485
  SetAsPeerNode(Add_Eth_N3, 1);                   // definisco Nodo03 Eth + brige02 RS485
  SetAsPeerNode(Nodo04_B01_P01, 3);      // definisco Nodo04 peer01 su bridge01
  SetAsPeerNode(Nodo05_B01_P02, 4);      // definisco Nodo05 peer02 su bridge01
  SetAsPeerNode(Nodo06_B02_P01, 5);      // definisco Nodo06 peer01 su bridge02
  SetAsPeerNode(Nodo07_B02_P02, 2);      // definisco Nodo07 peer02 su bridge02

  
  SetAddress(0xAB01, 0xFF00, 0x0000); 
  
  
  
*/	





#endif


