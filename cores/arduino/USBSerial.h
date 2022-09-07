#pragma once

#include "Arduino.h"
#include "HardwareSerial.h"
#include "RingBuffer.h"
#include "Stream.h"
#include "board.h"
#include "fsl_lpuart.h"
#include <inttypes.h>

class USBSerial : public HardwareSerial
{
  public:
    void begin(unsigned long baudrate, uint16_t config = SERIAL_8N1);
    void begin(void);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool()
    {
        return true;
    }
    void CDC_IRQHandel(void);
    USBSerial();
    ~USBSerial(){};

  private:
    void init(unsigned long baud, uint16_t config);
    RingBufferN<512> *_rb;
};

extern USBSerial SerialUSB;