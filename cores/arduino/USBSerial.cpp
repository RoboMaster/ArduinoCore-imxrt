#include "USBSerial.h"
#include "virtual_com.h"
#include "wiring_private.h"

extern "C" {
extern void board_boot_1rd_pattern(void);
extern uint32_t *stop_boot_app_flag;
}

uint8_t cdc_reset_signal_buf[2];

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
    uint8_t data[64];
    uint32_t length;
    length = vcom_get_recBuf(data);
    for (uint8_t i = 0; i < length; i++)
    {
        // 重启信号检测
        //-----------------------------------------
        cdc_reset_signal_buf[0] = data[i];
        if (cdc_reset_signal_buf[0] == 0x55 && cdc_reset_signal_buf[1] == 0xaa)
        {
            *stop_boot_app_flag = 0x1234abcd;
            board_boot_1rd_pattern();
        }
        cdc_reset_signal_buf[1] = cdc_reset_signal_buf[0];
        //-----------------------------------------

        // 只有在单独使用或者配合AI使用时，USB作为调试端口使用
#if (EXT_FEATURE_MODE == 1) || (EXT_FEATURE_MODE == 3)
        _rb->store_char(data[i]);
#endif

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
}

void USBSerial::begin(void)
{
    _rb = new RingBufferN<512>();
    _rb->clear();
}

void USBSerial::init(unsigned long baud, uint16_t config)
{
}

void USBSerial::end()
{
    delete _rb;
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
