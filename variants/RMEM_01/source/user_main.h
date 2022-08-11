/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __USER_MAIN_H__
#define __USER_MAIN_H__

#include "MIMXRT1064.h"

#define APP_VERSION (0x0001001A)    //0.1.0.26
#define HW_VER_ID   "AI_BOARD_V4.0"

#define UNUSED(X) (void)X

int vendor_start(void);

#endif
