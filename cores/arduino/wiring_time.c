/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "Arduino.h"
#include "FreeRTOS.h"
#include "timers.h"

unsigned long millis(void)
{
    return xTaskGetTickCount();
}

unsigned long micros(void)
{
    uint32_t ticks, ticks2;
    uint32_t pend, pend2;
    uint32_t count, count2;

    ticks2 = SysTick->VAL;
    pend2 = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk);
    count2 = millis();

    do
    {
        ticks = ticks2;
        pend = pend2;
        count = count2;
        ticks2 = SysTick->VAL;
        pend2 = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk);
        count2 = millis();
    } while ((pend != pend2) || (count != count2) || (ticks < ticks2));

    return ((count + pend) * 1000) + (((SysTick->LOAD - ticks) * (1048576 / (SystemCoreClock / 1000000))) >> 20);
}

void delay(uint32_t ms)
{
    // 注意：不能在中断中调用
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

// 微妙级延时 阻塞线程 理论上其取值不能高于一个osTick
void delayMicroseconds(unsigned int us)
{
    if (us == 0)
        return;

    uint32_t start = micros();

    while (us > 0)
    {
        yield();
        while (us > 0 && (micros() - start) >= 1)
        {
            us--;
            start += 1;
        }
    }
}
