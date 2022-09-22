/**
 * @file HelloServo.ino
 * @author Leitao Yu (flame.yu@dji.com)
 * @brief
 * @version 0.1
 * @date 2022-09-21
 *
 * @copyright Copyright (c) 2022 Dji.
 *
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
