/**
 * @file RMAI_Libs.h
 * @author Leitao Yu (flame.yu@dji.com)
 * @brief
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022 Dji.
 *
 */
#ifndef _RMAI_LIBS_INCLUDED
#define _RMAI_LIBS_INCLUDED

#include <Arduino.h>
#include <functional>

#include "FreeRTOS.h"
#include "semphr.h"

#define OPEN_PROTOCOL_MUTEX_DECLARE(mutex) SemaphoreHandle_t mutex = NULL
#define OPEN_PROTOCOL_MUTEX_INIT(mutex)    (mutex) = xSemaphoreCreateMutex()
#define OPEN_PROTOCOL_MUTEX_LOCK(mutex)    xSemaphoreTake(mutex, portMAX_DELAY)
#define OPEN_PROTOCOL_MUTEX_UNLOCK(mutex)  xSemaphoreGive(mutex)

typedef struct tag
{
    uint8_t id;
    uint16_t x;
    uint8_t y;
    uint16_t w;
    uint8_t h;
    uint8_t c;
} results_tag;

class RMAI_Results
{
  public:
    RMAI_Results();
    ~RMAI_Results();

    void begin();
    void setCallback(std::function<void(results_tag *, uint8_t)> func);
    void run();

  private:
    uint8_t _available;
    std::function<void(results_tag *, uint8_t)> _callback;
    results_tag _results[21];
    uint8_t _result_num = 0;
};

#endif
