/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

void TaskBlink(void *pvParameters);
void TaskHello(void *pvParameters);

void setup()
{
    SerialUSB.begin();
    xTaskCreate(TaskBlink, "Blink", 128, NULL, 2, NULL);
    xTaskCreate(TaskHello, "Hello", 128, NULL, 1, NULL);
}

void loop()
{
}

void TaskBlink(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        vTaskDelay(500);
        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(500);
    }
}

void TaskHello(void *pvParameters)
{
    for (;;)
    {
        SerialUSB.println("Hello RoboMaster !!!");
        vTaskDelay(1000);
    }
}