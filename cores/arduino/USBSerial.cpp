/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

//===================================================================

/**
 * The MIT License (MIT)
 *
 * Author: Hongtai Liu (lht856@fox.com)
 *
 * Copyright (C) 2019  Seeed Technology Co.,Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "USBSerial.h"
#include "virtual_com.h"
#include "wiring_private.h"

extern "C" {
extern void board_boot_1rd_pattern(void);
extern uint32_t *stop_boot_app_flag;
}

uint8_t cdc_reset_signal_buf[2] = {0, 0};

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

        // 连接EP会占用USB端口，Arduino层不能使用
#if !defined(CONNECT_EP)
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
