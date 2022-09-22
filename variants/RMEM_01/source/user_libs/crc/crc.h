/**
 * @file crc.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-11-29
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __CRC_H__
#define __CRC_H__

#include "stdint.h"

uint8_t crc8_checksum_get(uint8_t *buf, uint32_t len, uint8_t crc8);
uint16_t crc16_checksum_get(uint8_t *buf, uint32_t len, uint16_t crc16);

#endif
