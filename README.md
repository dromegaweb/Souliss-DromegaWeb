# Souliss-DromegaWeb

## Casa Domotica con sistema Souliss.

Data Aggiornamento
07-02-2019

---

#### [Nodo01](https://github.com/dromegaweb/Souliss-DromegaWeb/blob/master/Nodo01_GatewayEth/Nodo01_GatewayEth.ino) - Gateway - Funzionante con:

Nodo03

Nodo07

	Hardaware -> Arduino Mega 2560 + Ethernet W5100
	Software -> Collegato via Ethernet alla rete funziona come Gateway

---

#### [Nodo03](https://github.com/dromegaweb/Souliss-DromegaWeb/blob/master/Nodo03_Eth_RS485Bridge02_W5100/Nodo03_Eth_RS485Bridge02_W5100.ino) - Funzionante 

	Hardaware -> Arduino uno + Ethernet W5100 + RS485
	Software -> Collegato via Ethernet alla rete funziona da ponte (Bridge02) verso altri nodi con una linea RS485.

---

#### [Nodo07](https://github.com/dromegaweb/Souliss-DromegaWeb/blob/master/Nodo07_RS485Peer02Br02-Giardino/Nodo07_RS485Peer02Br02-Giardino.ino) - Funzionante

	Hardaware -> Arduino Standalone Programmato con USBasp
	Software -> Peer01 RS485 su Bridge02(Nodo03)
---
