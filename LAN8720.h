////////////////////////////////////////////////////////////////////////////////
// LAN8720.h
//
// MIT License
//
// Copyright (c) 2017 Alex Christoffer Rasmussen
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

#ifndef __LAN8720_h__
#define __LAN8720_h__

#include "IPAddress.h"

class LAN8720Class {
private:
  uint8_t MACADDR[6];
  IPAddress ip;
  IPAddress gateway;
  IPAddress subnetMask;


public:

  void init(void);

  void setMACAddress(const uint8_t *addr);

  void setIPAddress(const uint8_t *addr);
  void getIPAddress(uint8_t *addr);

  void setGatewayIp(const uint8_t *addr);
  void getGatewayIp(uint8_t *addr);

	void setSubnetMask(const uint8_t *addr);
  void getSubnetMask(uint8_t *addr);
};

extern LAN8720Class LAN8720;

#endif
