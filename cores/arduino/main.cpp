/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "Arduino.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#define ARDUINO_LOOP_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#ifndef ARDUINO_LOOP_STACK_SIZE
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 4 * 1024
#else
#define ARDUINO_LOOP_STACK_SIZE CONFIG_ARDUINO_LOOP_STACK_SIZE
#endif
#endif

TaskHandle_t loopTaskHandle = NULL;

bool loopTaskWDTEnabled;

__attribute__((weak)) size_t getArduinoLoopTaskStackSize(void)
{
    return ARDUINO_LOOP_STACK_SIZE;
}

void __attribute__((weak)) setup()
{
}

void __attribute__((weak)) loop()
{
}

void loopTask(void *pvParameters)
{
    setup();

    for (;;)
    {
        loop();
    }
}

int __attribute__((weak)) main()
{
    init();

    xTaskCreate(loopTask, "loopTask", getArduinoLoopTaskStackSize(), NULL, ARDUINO_LOOP_TASK_PRIORITY, &loopTaskHandle);

    vTaskStartScheduler();

    return 0;
}