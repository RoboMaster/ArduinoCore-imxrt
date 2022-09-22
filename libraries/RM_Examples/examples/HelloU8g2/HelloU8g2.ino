/**
 * @file HelloU8g2.ino
 * @author Leitao Yu (flame.yu@dji.com)
 * @brief
 * @version 0.1
 * @date 2022-09-21
 *
 * @copyright Copyright (c) 2022 Dji.
 *
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void setup(void)
{
    u8g2.begin();
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Hello RoboMaster !!!");
    u8g2.sendBuffer();
    delay(1000);
    gpioTogglePin(LED_BUILTIN);
}