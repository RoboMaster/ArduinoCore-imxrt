/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include <stdint.h>
#include <functional>

typedef std::function<void(void)> callback_function_t;

void attachInterrupt(uint32_t pin, callback_function_t callback, uint32_t mode);
void detachInterrupt(uint32_t pin);

#endif /* _WIRING_INTERRUPTS_ */
