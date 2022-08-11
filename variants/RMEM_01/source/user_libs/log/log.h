/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>
#include <stdarg.h>

#define MAX_LOG_MODULE_NUM         (16)    //最大支持LOG模块数量
#define MAX_LOG_OUTPUT_OBJECT_NUM  (2)     //最大支持LOG输出对象数量
#define MAX_LOG_PRINT_BUFF_SIZE    (512)   //格式化缓冲区

typedef  uint32_t(*log_output_fn)(uint8_t *buf, uint32_t len);
typedef  uint32_t(*log_get_sys_time_ms)(void);

/* Log模块优先级 */
typedef enum
{
    LOG_ASSERT   = 0,
    LOG_ERROR    = 1,
    LOG_WARN     = 2,
    LOG_INFO     = 3,
    LOG_DEBUG    = 4,
    LOG_DEBUG_L2 = 5,
    LOG_DEBUG_L3 = 6,
    LOG_OFF      ,
}log_pri_e;

/* Log模块优先级 */
typedef enum
{
    LOG_RET_ERROR = -1,
    LOG_RET_OK    = 0,
}log_return_e;

/* Log模块结构体 */
typedef struct
{
    char*   name;       //模块名
    uint8_t enable;     //输出使能
    uint8_t is_init;    //是否初始化
}log_module_t;

/* Log输出对象结构体 */
typedef struct
{
    char*                 name;         //模块名
    log_output_fn         output_fn;    //输出回调函数
    uint8_t               enable;       //输出使能
    uint8_t               is_init;      //是否初始化
}
log_output_object_t;

void                  log_init(log_get_sys_time_ms fn);
void                  log_set_output_pri(log_pri_e pri);
void                  log_set_module_en(log_module_t *obj, uint8_t en);
void                  log_set_output_en(log_output_object_t *obj, uint8_t en);
log_module_t*         log_module_inquire_name(char* name);
log_output_object_t*  log_output_object_add(char* name, log_output_fn fn);
log_module_t*         log_module_add(char* name);


void                  log_printf(log_module_t *module, uint32_t timeout, log_pri_e pri, char* fmt, ...);
void                  log_vprintf(log_module_t *module, log_pri_e pri, char* fmt, va_list arg);
void                  log_printf_noprefix(log_module_t *module, uint32_t timeout, log_pri_e pri, char* fmt, ...);


#endif
