#ifndef _ethernetK66_h_
#define _ethernetK66_h_

#ifndef __MK66FX1M0__
#error only for T 3.6
#endif

// #include <inttypes.h>
#include "core_pins.h"
// #include <stdlib.h>
#include <string.h>

#define MAX_SOCK_NUM 4

#define RXSIZE 12
#define TXSIZE 10
#define BUFLEN 128

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

typedef struct enetIP_s{
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t subnetMask[4];
}enetIP_t;

class k66Class {
private:
  static uint8_t _mac[6];

  static uint8_t _rxNum;
  static uint8_t _txNum;

  static enetbufferdesc_t rxRing[RXSIZE] __attribute__ ((aligned(16)));
  static enetbufferdesc_t txRing[TXSIZE] __attribute__ ((aligned(16)));

  static uint32_t rxBuffer[RXSIZE*BUFLEN] __attribute__ ((aligned(16)));
  static uint32_t txBuffer[TXSIZE*BUFLEN] __attribute__ ((aligned(16)));


public:
  static void init(const uint8_t * macAddrd);
};



#endif
