/**
 * @file task_protocol.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-07-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef __TASK_PROCOTOL_H__
#define __TASK_PROCOTOL_H__

#include "stdint.h"

#define OPEN_PROTO_LOCAL_ADDR_BASE      0x0100u     //在开放协议中的地址起始


void task_protocol(void const * argument);

#endif //__TASK_DJI_PROCOTOL_H__
