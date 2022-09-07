#include "USBSerial.h"
#include "virtual_com.h"
#include "wiring_private.h"

extern "C" {
extern void board_boot_1rd_pattern(void);
extern uint32_t *stop_boot_app_flag;

extern uint8_t arduino_use_usb_serial;
}

uint8_t temp[2];

void CDC_IRQHandel()
{
    SerialUSB.CDC_IRQHandel();
}

uint32_t CDC_available()
{
    return SerialUSB.available();
}

void USBSerial::CDC_IRQHandel(void)
{
    if (arduino_use_usb_serial == 0) return;

    uint8_t data[64];
    uint32_t length;
    length = vcom_get_recBuf(data);
    for (uint8_t i = 0; i < length; i++)
    {
        temp[0] = data[i];
        // 收到重启信号
        if (temp[0] == 0x55 && temp[1] == 0xaa)
        {
            *stop_boot_app_flag = 0x1234abcd;
            board_boot_1rd_pattern();
        }
        temp[1] = temp[0];

        _rb->store_char(data[i]);
    }
}

USBSerial::USBSerial()
{
}

void USBSerial::begin(unsigned long baud, uint16_t config)
{
    _rb = new RingBufferN<512>();
    _rb->clear();
    init(baud, config);
    arduino_use_usb_serial = 1;
}

void USBSerial::begin(void)
{
    _rb = new RingBufferN<512>();
    _rb->clear();
    arduino_use_usb_serial = 1;
}

void USBSerial::init(unsigned long baud, uint16_t config)
{
}

void USBSerial::end()
{
    delete _rb;
    arduino_use_usb_serial = 0;
}

int USBSerial::available(void)
{
    return _rb->available();
}

int USBSerial::peek(void)
{
    return _rb->peek();
}

int USBSerial::read(void)
{
    int temp;
    while (true)
    {
        if (0 != _rb->available())
        {
            temp = _rb->read_char();
            return temp;
        }
        else
        {
            return -1;
        }
    }
}
size_t USBSerial::write(uint8_t c)
{
    // TODO:错误参数传递
    vcom_write_buf(&c, 1);
    return 1;
}

void USBSerial::flush(void)
{
    _rb->clear();
}

USBSerial SerialUSB;
