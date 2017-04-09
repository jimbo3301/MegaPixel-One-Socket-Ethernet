#ifndef _ethernetK66_h_
#define _ethernetK66_h_

#ifndef __MK66FX1M0__
#error only for T 3.6
#endif

// #include <inttypes.h>
#include "core_pins.h"
// #include <stdlib.h>
#include <string.h>

#include "DMAChannel.h"

#define RXSIZE 12
#define TXSIZE 10
#define BUFLEN 128

#define goodRXflag 0x1000100000110011

typedef struct EthernetHeader_s {
  uint16_t   pad;
  uint8_t   dstMAC[6];
  uint8_t   srcMAC[6];
  uint16_t  type;
} EthernetHeader_t;

typedef struct {
  uint16_t length;
  uint16_t flags;
  void *buffer;
  uint32_t moreflags;
  uint16_t checksum;
  uint8_t ipProtocol;
  uint8_t etherType;
  uint32_t dmadone;
  uint32_t timestamp;
  uint32_t unused1;
  uint32_t unused2;
} enetbufferdesc_t;

class k66Class {
private:
  static uint8_t _mac[6];

  static uint8_t _rxNum;
  static uint8_t _txNum;

  static enetbufferdesc_t rxRing[RXSIZE] __attribute__ ((aligned(16)));
  static enetbufferdesc_t txRing[TXSIZE] __attribute__ ((aligned(16)));

  static uint32_t rxBuffer[RXSIZE*BUFLEN] __attribute__ ((aligned(16)));
  static uint32_t txBuffer[TXSIZE*BUFLEN] __attribute__ ((aligned(16)));
  static void rxInterrupt();

public:
  static void init(const uint8_t *macAddrd);
  static uint8_t sendTxBuffer(const uint32_t *buffer);

};

class k66ethernetWriteClass {
private:
  uint32_t _buffer[BUFLEN] __attribute__ ((aligned(16)));
public:
  //takes ip and mac pointer
  //returns pointer to the buffer after the ethernet headder
  uint32_t* begin(const uint8_t* dstIP, const uint8_t* dstMac);
  // without a mac a ip look up will be done or a ARP request
  uint32_t* begin(const uint8_t* dstIP);

  //sends the buffer to the ethernet controller
  int end();
};


#endif
