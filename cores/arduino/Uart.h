#pragma once

#include "Arduino.h"
#include "HardwareSerial.h"
#include "RingBuffer.h"
#include "Stream.h"
#include "board.h"
#include "fsl_lpuart.h"
#include <inttypes.h>

class UART : public HardwareSerial
{
  public:
    void begin(unsigned long baudrate, uint16_t config = SERIAL_8N1);
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
    void LPUART_IRQHandel(void);
    UART(LPUART_Type *uart_num_, IRQn_Type uart_num_IRQ, int rx_pin_, int tx_pin_);
    ~UART(){};

  private:
    void init(unsigned long baud, uint16_t config);
    RingBuffer *_rb;
    size_t read_timeout_ = 1000;
    LPUART_Type *_lpuart_num;
    IRQn_Type _lpuart_num_IRQ;
    lpuart_handle_t g_lpuartHandle;
    int _rx_pin;
    int _tx_pin;
};

#if defined(USART1)
extern UART Serial;
#endif
#if defined(USART2)
extern UART Serial1;
#endif
#if defined(USART3)
extern UART Serial2;
#endif