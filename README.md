# Souliss-DromegaWeb

## Casa Domotica con sistema Souliss.

Data Aggiornamento
07-02-2019

#### Nodo01 - Gateway - Funzionante con:
- Nodo03
- Nodo07 

		Hardaware -> Arduino Mega 2560 + Ethernet W5100
	
Nodo03 - Funzionante 
       - Hardaware -> Arduino uno + Ethernet W5100 + RS485
       - Software -> Collegato via Ethernet alla rete funziona da ponte (Bridge02) verso altri nodi con una linea RS485.
       
Nodo07 - Funzionante
       - Hardaware -> Arduino Standalone Programmato con USBasp
       - Software -> Peer01 RS485 su Bridge02(Nodo03) 

