/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef _CAN_H
#define _CAN_H

#include <stdint.h>
#include "fsl_flexcan.h"
#include "peripherals.h"

void can3_rx_int_handle(void);
void can3_tx_int_handle(void);

void can3_init(void);
void can_tx_id_set(uint16_t id);
void can_send(uint8_t *buf, uint16_t len);
uint8_t get_can_state(void);
uint16_t can_receive(uint8_t *buf, uint16_t len);
uint16_t can_receive0(uint8_t *buf, uint16_t len);
uint16_t can_receive1(uint8_t *buf, uint16_t len);
uint16_t can_receive2(uint8_t *buf, uint16_t len);

int can_test(void);

#endif
