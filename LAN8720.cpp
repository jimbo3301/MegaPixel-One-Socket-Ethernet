////////////////////////////////////////////////////////////////////////////////
// LAN8720.cpp
//
// MIT License
//
// Copyright (c) 2016 Alex Christoffer Rasmussen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include LAN8720.h

void LAN8720Class::setMACAddress(const uint8_t *addr) {
  memcpy(MACADDR[0], addr, 6);
}

void LAN8720Class::setIPAddress(const uint8_t *addr) {
  memcpy(ip, addr, 4);
}
void LAN8720Class::getIPAddress(uint8_t *addr) {
  memcpy(addr, ip, 4);
}

void LAN8720Class::setGatewayIp(const uint8_t *addr) {
  memcpy(gateway, addr, 4);
}
void LAN8720Class::getGatewayIp(uint8_t *addr) {
  memcpy(addr, gateway, 4);
}

void LAN8720Class::setSubnetMask(const uint8_t *addr) {
  memcpy(subnetMask, addr, 4);
}
void LAN8720Class::getSubnetMask(uint8_t *addr) {
  memcpy(addr, subnetMask, 4);
}

void LAN8720Class::init(void) {
  memcpy(MACADDR_H+1, MACADDR[0], 3);
  memcpy(MACADDR_L+1, MACADDR[3], 3);


  MACADDR_H = MACADDR_H & 0x00FFFFFF;
  MACADDR_L = MACADDR_L & 0x00FFFFFF;

  MPU_RGDAAC0 |= 0x007C0000;
	SIM_SCGC2 |= SIM_SCGC2_ENET;
	CORE_PIN3_CONFIG =  PORT_PCR_MUX(4); // RXD1
	CORE_PIN4_CONFIG =  PORT_PCR_MUX(4); // RXD0
	CORE_PIN24_CONFIG = PORT_PCR_MUX(2); // REFCLK
	CORE_PIN25_CONFIG = PORT_PCR_MUX(4); // RXER
	CORE_PIN26_CONFIG = PORT_PCR_MUX(4); // RXDV
	CORE_PIN27_CONFIG = PORT_PCR_MUX(4); // TXEN
	CORE_PIN28_CONFIG = PORT_PCR_MUX(4); // TXD0
	CORE_PIN39_CONFIG = PORT_PCR_MUX(4); // TXD1
	CORE_PIN16_CONFIG = PORT_PCR_MUX(4); // MDIO
	CORE_PIN17_CONFIG = PORT_PCR_MUX(4); // MDC
	SIM_SOPT2 |= SIM_SOPT2_RMIISRC | SIM_SOPT2_TIMESRC(3);
	// ENET_EIR	1356	Interrupt Event Register
	// ENET_EIMR	1359	Interrupt Mask Register
	// ENET_RDAR	1362	Receive Descriptor Active Register
	// ENET_TDAR	1363	Transmit Descriptor Active Register
	// ENET_ECR	1363	Ethernet Control Register
	// ENET_RCR	1369	Receive Control Register
	// ENET_TCR	1372	Transmit Control Register
	// ENET_PALR/UR	1374	Physical Address
	// ENET_RDSR	1378	Receive Descriptor Ring Start
	// ENET_TDSR	1379	Transmit Buffer Descriptor Ring
	// ENET_MRBR	1380	Maximum Receive Buffer Size
	//		1457	receive buffer descriptor
	//		1461	transmit buffer descriptor
	memset(rx_ring, 0, sizeof(rx_ring));
	memset(tx_ring, 0, sizeof(tx_ring));

	for (int i=0; i < RXSIZE; i++) {
		rx_ring[i].flags = 0x8000; // empty flag
		rx_ring[i].buffer = rxbufs + i * 128;
	}
	rx_ring[RXSIZE-1].flags = 0xA000; // empty & wrap flags
	for (int i=0; i < TXSIZE; i++) {
		tx_ring[i].buffer = txbufs + i * 128;
	}
	tx_ring[TXSIZE-1].flags = 0x2000; // wrap flag

	ENET_EIMR = 0;
	ENET_MSCR = ENET_MSCR_MII_SPEED(15);  // 12 is fastest which seems to work
	ENET_RCR = ENET_RCR_NLC | ENET_RCR_MAX_FL(1522) | ENET_RCR_CFEN |
		ENET_RCR_CRCFWD | ENET_RCR_PADEN | ENET_RCR_RMII_MODE |
		/* ENET_RCR_FCE | ENET_RCR_PROM | */ ENET_RCR_MII_MODE;
	ENET_TCR = ENET_TCR_ADDINS | /* ENET_TCR_RFC_PAUSE | ENET_TCR_TFC_PAUSE | */
    ENET_TCR_FDEN;

	ENET_PALR = (MACADDR_H << 8) | ((MACADDR_L >> 16) & 255);
	ENET_PAUR = ((MACADDR_L << 16) & 0xFFFF0000) | 0x8808;
	ENET_OPD = 0x10014;
	ENET_IAUR = 0;
	ENET_IALR = 0;
	ENET_GAUR = 0;
	ENET_GALR = 0;
	ENET_RDSR = (uint32_t)rx_ring;
	ENET_TDSR = (uint32_t)tx_ring;
	ENET_MRBR = 512;
	ENET_TACC = ENET_TACC_SHIFT16;
	//ENET_TACC = ENET_TACC_SHIFT16 | ENET_TACC_IPCHK | ENET_TACC_PROCHK;
	ENET_RACC = ENET_RACC_SHIFT16 | ENET_RACC_LINEDIS | ENET_RACC_PADREM;

	ENET_ECR = 0xF0000000 | ENET_ECR_DBSWP | ENET_ECR_EN1588 | ENET_ECR_ETHEREN;
	ENET_RDAR = ENET_RDAR_RDAR;
	ENET_TDAR = ENET_TDAR_TDAR;
}
