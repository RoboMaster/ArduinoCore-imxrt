/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __CRC_H__
#define __CRC_H__

#include "stdint.h"

uint8_t crc8_checksum_get(uint8_t *buf, uint32_t len, uint8_t crc8);
uint16_t crc16_checksum_get(uint8_t *buf, uint32_t len, uint16_t crc16);

#endif
