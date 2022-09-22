/**
 * @file HelloFreeRTOS.ino
 * @author Leitao Yu (flame.yu@dji.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022 Dji.
 * 
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