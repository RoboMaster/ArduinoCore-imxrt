/**
 * @file slot_factory_test.h
 * @author Zhenling Pan(ryan.pan@dji.com)
 * @brief 14Pin插座和100Pin插座工厂测试用,将IO成对测试,
 *        一发一收测试.
 * @version 0.1
 * @date 2021-12-14
 *
 * @copyright Copyright (c) 2021
 *
 */



#ifndef __SOLT_FACTORY_TEST_H__
#define __SOLT_FACTORY_TEST_H__

#include "stdint.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
int slot_14pin_test(void);
int slot_100pin_test(void);

#endif