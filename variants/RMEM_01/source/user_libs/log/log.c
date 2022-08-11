/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "log.h"
#include "cmsis_os.h"
#include "cmsis_gcc.h"
#include <string.h>
#include <stdio.h>

log_module_t        log_module[MAX_LOG_MODULE_NUM];
log_output_object_t log_output_obj[MAX_LOG_OUTPUT_OBJECT_NUM];
uint8_t             log_output_pri;
log_get_sys_time_ms get_sys_time_fn;
char                log_printf_buff[MAX_LOG_PRINT_BUFF_SIZE];
const char          log_pri_char[] = {'A','E','W','I','D','D','D'};
uint8_t             critical_flag;

/**
 * @brief log系统初始化
 *
 * @param fn 获取系统时间回调
 */
void log_init(log_get_sys_time_ms fn)
{
    memset(log_module, 0, sizeof(log_module));
    memset(log_output_obj, 0, sizeof(log_output_obj));
    get_sys_time_fn  = fn;
    log_output_pri = LOG_DEBUG_L3;
}

/**
 * @brief 设置log系统打印优先级
 *
 * @param pri
 */
void log_set_output_pri(log_pri_e pri)
{
    log_output_pri = pri;
}

/**
 * @brief 设置log模块使能
 *
 * @param obj 模块对象
 * @param en 是否使能
 */
void log_set_module_en(log_module_t *obj, uint8_t en)
{
    if(en)
    {
        obj->enable = 1;
    }
    else
    {
        obj->enable = 0;
    }
}

/**
 * @brief 设置log系统输出使能
 *
 * @param obj 输出接口对象
 * @param en 是否使能
 */
void log_set_output_en(log_output_object_t *obj, uint8_t en)
{
    if(en)
    {
        obj->enable = 1;
    }
    else
    {
        obj->enable = 0;
    }
}

/**
 * @brief 从字符串查找Log模块结构体
 *
 * @param name
 * @return log_output_object_t*
 */
log_module_t* log_module_inquire_name(char* name)
{
    for(int i = 0; i < MAX_LOG_MODULE_NUM; i++)
    {
        if(!log_module[i].is_init)
        {
            break;
        }

        if(strcmp(log_module[i].name, name) == 0)
        {

             return &(log_module[i]);
        }
    }

    return NULL;
}

/**
 * @brief 添加Log系统输出接口
 *
 * @param name 输出接口名
 * @param fn 输出接口回调函数
 * @return log_module_t 接口结构体
 */
log_output_object_t *log_output_object_add(char* name, log_output_fn fn)
{
    if(fn == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < MAX_LOG_OUTPUT_OBJECT_NUM; i++)
    {
        if(!log_output_obj[i].is_init)
        {
            log_output_obj[i].is_init   = 1;
            log_output_obj[i].name      = name;
            log_output_obj[i].enable    = 1;
            log_output_obj[i].output_fn = fn;

            return (log_output_obj + i);
        }
    }
    return NULL;

}

/**
 * @brief 添加Log系统模块
 *
 * @param name 模块名
 * @return log_output_object_t* 模块结构体指针
 */
log_module_t*  log_module_add(char* name)
{
    for(int i = 0; i < MAX_LOG_MODULE_NUM; i++)
    {
        if(!log_module[i].is_init)
        {
            log_module[i].is_init = 1;
            log_module[i].name    = name;
            log_module[i].enable  = 1;

            return (log_module + i);
        }
    }
    return NULL;
}

/**
 * @brief 打印Log
 *
 * @param module 模块对象
 * @param pri 优先级
 * @param fmt 格式化文本
 * @param ...
 */
void log_printf(log_module_t *module, uint32_t timeout, log_pri_e pri, char* fmt, ...)
{
    if(module == NULL)
    {
        return;
    }

    if(pri > log_output_pri || (!module->enable))
    {
        return;
    }

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();
    while(critical_flag)
    {
        __set_PRIMASK(cpu_sr);
        osDelay(1);
        cpu_sr = __get_PRIMASK();
        __disable_irq();
        if(!(timeout--))
        {
            return;
        }
    }
    critical_flag = 1;
    __set_PRIMASK(cpu_sr);

    va_list arg;
    uint16_t printf_len = 0;
    va_start(arg, fmt);

    char  pri_char = log_pri_char[pri];
    uint32_t time_sec = (get_sys_time_fn)();
    uint32_t time_ms = time_sec % 1000;
    time_sec = time_sec / 1000;

    printf_len = snprintf(log_printf_buff , MAX_LOG_PRINT_BUFF_SIZE - printf_len,
                           "[%c][%d.%03d][%s]:",pri_char, time_sec, time_ms, module->name);
    printf_len +=  vsnprintf(log_printf_buff + printf_len, MAX_LOG_PRINT_BUFF_SIZE, fmt, arg);
    printf_len += snprintf(log_printf_buff + printf_len, MAX_LOG_PRINT_BUFF_SIZE - printf_len,
                           "\r\n");

    va_end(arg);

    if(printf_len > MAX_LOG_PRINT_BUFF_SIZE)
    {
        printf_len = MAX_LOG_PRINT_BUFF_SIZE;
    }

    for(int i = 0; i < MAX_LOG_OUTPUT_OBJECT_NUM; i++)
    {
        if(log_output_obj[i].enable && log_output_obj[i].is_init)
        {
            (*(log_output_obj[i].output_fn))((uint8_t*)log_printf_buff, printf_len);
        }
    }

    cpu_sr = __get_PRIMASK();
    __disable_irq();
    critical_flag = 0;
    __set_PRIMASK(cpu_sr);

    // va_list arg;
    // va_start(arg, fmt);

    // log_vprintf(module, pri, fmt, arg);

    // va_end(arg);

}

/**
 * @brief 打印Log
 *
 * @param module 模块对象
 * @param pri 优先级
 * @param fmt 格式化文本
 * @param va_list
 */
void log_vprintf(log_module_t *module, log_pri_e pri, char* fmt, va_list arg)
{
    if(module == NULL)
    {
        return;
    }

    if(pri > log_output_pri || (!module->enable))
    {
        return;
    }

    uint16_t printf_len = 0;

    char  pri_char = log_pri_char[pri];
    uint32_t time_sec = (get_sys_time_fn)();
    uint32_t time_ms = time_sec % 1000;
    time_sec = time_sec / 1000;

    printf_len = snprintf(log_printf_buff , MAX_LOG_PRINT_BUFF_SIZE - printf_len,
                           "[%c][%d.%03d][%s]:",pri_char, time_sec, time_ms, module->name);
    printf_len +=  vsnprintf(log_printf_buff + printf_len, MAX_LOG_PRINT_BUFF_SIZE, fmt, arg);
    printf_len += snprintf(log_printf_buff + printf_len, MAX_LOG_PRINT_BUFF_SIZE - printf_len,
                           "\r\n");

    if(printf_len > MAX_LOG_PRINT_BUFF_SIZE)
    {
        printf_len = MAX_LOG_PRINT_BUFF_SIZE;
    }

    for(int i = 0; i < MAX_LOG_OUTPUT_OBJECT_NUM; i++)
    {
        if(log_output_obj[i].enable && log_output_obj[i].is_init)
        {
            (*(log_output_obj[i].output_fn))((uint8_t*)log_printf_buff, printf_len);
        }
    }

}

/**
 * @brief 打印Log
 *
 * @param module 模块对象
 * @param pri 优先级
 * @param fmt 格式化文本
 * @param ...
 */
void log_printf_noprefix(log_module_t *module, uint32_t timeout, log_pri_e pri, char* fmt, ...)
{
    if(module == NULL)
    {
        return;
    }

    if(pri > log_output_pri || (!module->enable))
    {
        return;
    }

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();
    while(critical_flag)
    {
        __set_PRIMASK(cpu_sr);
        osDelay(1);
        cpu_sr = __get_PRIMASK();
        __disable_irq();
        if(!(timeout--))
        {
            return;
        }
    }
    critical_flag = 1;
    __set_PRIMASK(cpu_sr);

    va_list arg;
    uint16_t printf_len;
    va_start(arg, fmt);

    printf_len = vsnprintf(log_printf_buff, MAX_LOG_PRINT_BUFF_SIZE, fmt, arg);

    va_end(arg);

    if(printf_len > MAX_LOG_PRINT_BUFF_SIZE)
    {
        printf_len = MAX_LOG_PRINT_BUFF_SIZE;
    }

    for(int i = 0; i < MAX_LOG_OUTPUT_OBJECT_NUM; i++)
    {
        if(log_output_obj[i].enable && log_output_obj[i].is_init)
        {
            (*(log_output_obj[i].output_fn))((uint8_t*)log_printf_buff, printf_len);
        }
    }

    cpu_sr = __get_PRIMASK();
    __disable_irq();
    critical_flag = 0;
    __set_PRIMASK(cpu_sr);

    // va_list arg;
    // va_start(arg, fmt);

    // log_vprintf(module, pri, fmt, arg);

    // va_end(arg);

}
