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

enum SockCMD {
  Sock_OPEN      = 0x01,
  Sock_LISTEN    = 0x02,
  Sock_CONNECT   = 0x04,
  Sock_DISCON    = 0x08,
  Sock_CLOSE     = 0x10,
  Sock_SEND      = 0x20,
  Sock_SEND_MAC  = 0x21,
  Sock_SEND_KEEP = 0x22,
  Sock_RECV      = 0x40
};

enum class SocSta: uint8_t {
  CLOSED      = 0x00,
  INIT,
  LISTEN,
  CONNECT,
  DISCON,
  /*
  static const uint8_t LISTEN      = 0x14;
  static const uint8_t SYNSENT     = 0x15;
  static const uint8_t SYNRECV     = 0x16;
  static const uint8_t ESTABLISHED = 0x17;
  static const uint8_t FIN_WAIT    = 0x18;
  static const uint8_t CLOSING     = 0x1A;
  static const uint8_t TIME_WAIT   = 0x1B;
  static const uint8_t CLOSE_WAIT  = 0x1C;
  static const uint8_t LAST_ACK    = 0x1D;
  static const uint8_t UDP         = 0x22;
  static const uint8_t IPRAW       = 0x32;
  static const uint8_t MACRAW      = 0x42;
  static const uint8_t PPPOE       = 0x5F;*/
}

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

typedef struct SOCKET_s{
  uint8_t status;
  uint8_t protocol;
  uint16_t port;
  uint16_t dstIP;
}SOCKET_t;

typedef struct enetIP_s{
  uint8_t localIP[4];
  uint8_t gateway[4];
  uint8_t subnetMask[4];
}enetIP_t;

class k66Class {
private:
  static enetIP_t _enetIP;
  static SOCKET_t _socket[MAX_SOCK_NUM];

  static uint8_t _mac[6];

  static uint8_t _rxNum;
  static uint8_t _txNum;

  static enetbufferdesc_t rxRing[RXSIZE] __attribute__ ((aligned(16)));
  static enetbufferdesc_t txRing[TXSIZE] __attribute__ ((aligned(16)));

  static uint32_t rxBuffer[RXSIZE*BUFLEN] __attribute__ ((aligned(16)));
  // static uint32_t txBuffer[TXSIZE*BUFLEN] __attribute__ ((aligned(16)));

  static void checkRxBuffer();

public:
  static uint8_t init(const uint8_t * macAddrd);

  static void setGatewayIp(const uint8_t * addr);
  static void getGatewayIp(uint8_t * addr);

  static void setSubnetMask(const uint8_t * addr);
  static void getSubnetMask(uint8_t * addr);

  static void setIPAddress(const uint8_t * addr);
  static void getIPAddress(uint8_t * addr);

  static void openSn(uint8_t socket);
  static void closeSn(uint8_t socket);

  static uint8_t getSnStatus(uint8_t socket);
  static void setSnState(uint8_t socket, uint8_t state);

  static void setSnDstIP(uint8_t socket, uint8_t *addr);
  static void setSnDstPort(uint8_t socket, uint16_t port);

  static void setSnLocalIP(uint8_t socket, uint8_t *addr);
  static void setSnLocalPort(uint8_t socket, uint16_t port);

  static void setSnProtocol(uint8_t socket, uint8_t protocol);

  static void readSnData(uint8_t socket, uint8_t *dst, uint16_t length);
  static void writeSnData(uint8_t socket, uint8_t *src, uint16_t length);

  /*static uint8_t readSnStatus(uint8_t socket);
  static void writeSnProtocol(uint8_t socket, uint8_t protocol);
  static void writeSnPort(uint8_t socket, uint16_t port);
  static void execCmdSn(uint8_t socket, uint8_t cmd);
  static void openSn(uint8_t socket);
  static void closeSn(uint8_t socket);
  static void readSnData(uint8_t s, uint16_t src, uint8_t *dst, uint16_t len);*/

  /*
  void setGatewayIp(const uint8_t * addr);
  void getGatewayIp(uint8_t * addr);

  void setSubnetMask(const uint8_t * addr);
  void getSubnetMask(uint8_t * addr);

  void setIPAddress(const uint8_t * addr);
  void getIPAddress(uint8_t * addr);

  static uint16_t write(uint16_t addr, const uint8_t *buf, uint16_t len);
  static uint8_t write(uint16_t addr, uint8_t data) {
    return write(addr, &data, 1);
  }
  */
};



#endif
