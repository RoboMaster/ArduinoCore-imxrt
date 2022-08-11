// #ifdef ARDUINO_MAIN

#include "Arduino.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define ARDUINO_LOOP_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#ifndef ARDUINO_LOOP_STACK_SIZE
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 2 * 1024
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
    // #if CONFIG_FREERTOS_UNICORE
    //     yieldIfNecessary();
    // #endif
    //     if (loopTaskWDTEnabled)
    //     {
    //       esp_task_wdt_reset();
    //     }

    loop();

    //   if (serialEventRun)
    //     serialEventRun();
    // }
  }
}

int __attribute__((weak)) main()
{
  // 执行板子初始化动作
  init();

  xTaskCreate(loopTask, "loopTask", getArduinoLoopTaskStackSize(), NULL, ARDUINO_LOOP_TASK_PRIORITY, &loopTaskHandle);

  vTaskStartScheduler();

  for (;;)
    ;
}

// #endif