/**
 * @file ReadResults.ino
 * @author Leitao Yu (flame.yu@dji.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-15
 * 
 * @copyright Copyright (c) 2022 Dji.
 * 
 * 
 * @attention   该库与AI相机模块通信获取数据，对底层有依赖
 *              请使用前在ArduinoIDE中打开 "Connect AI Camera" 开关
 * 
 */

#include <Arduino.h>
#include <RMAI_Results.h>

RMAI_Results ai_results;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    SerialUSB.begin(SERIAL_8N1);

    ai_results.begin();
    ai_results.setCallback(ai_objects_callback);
}

void loop()
{
    ai_results.run();

    gpioTogglePin(LED_BUILTIN);

    delay(50);
}

void ai_objects_callback(results_tag *results, uint8_t result_num)
{
    for (uint8_t i = 0; i < result_num; i++)
    {
        SerialUSB.print("ID: ");
        SerialUSB.println(results[i].id);
        SerialUSB.print("    x: ");
        SerialUSB.println(results[i].x);
        SerialUSB.print("    y: ");
        SerialUSB.println(results[i].y);
        SerialUSB.print("    w: ");
        SerialUSB.println(results[i].w);
        SerialUSB.print("    h: ");
        SerialUSB.println(results[i].h);
        SerialUSB.print("    C: ");
        SerialUSB.println(results[i].c);
    }
}
