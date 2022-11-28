/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include <Arduino.h>
#include <Servo.h>

Servo sg90;

int pos = 0;

void setup()
{
    sg90.attach(PWM0);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    for (pos = 0; pos <= 180; pos += 1)
    {
        sg90.write(pos);
        delay(5);
    }
    gpioTogglePin(LED_BUILTIN);

    for (pos = 180; pos >= 0; pos -= 1)
    {
        sg90.write(pos);
        delay(5);
    }
    gpioTogglePin(LED_BUILTIN);
}
