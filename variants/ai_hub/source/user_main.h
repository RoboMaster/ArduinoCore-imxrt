/**
 * @file main.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 202122
 *
 */


#ifndef __USER_MAIN_H__
#define __USER_MAIN_H__

#include "MIMXRT1064.h"

#define APP_VERSION (0x0001001A)    //0.1.0.26
#define HW_VER_ID   "AI_BOARD_V4.0"

#define UNUSED(X) (void)X

int vendor_start(void);

#endif
