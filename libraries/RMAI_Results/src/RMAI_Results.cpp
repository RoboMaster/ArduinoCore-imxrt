/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-14     robomaster   first version
 */

#include "open_protocol.h"
#include <Arduino.h>
#include <RMAI_Results.h>

// 该库需要与AI模块配合使用
// 如果没有定义 CONNECT_AI_CAMERA
// 在编译阶段报错，避免后面的错误
#if !defined(CONNECT_AI_CAMERA)
#error "Your Using RMAI_Results Lib."
#error "Please Open `Connect AI Camera` Switch !!!!!!!!!!!!"
#endif

results_tag results[21];
uint8_t result_num = 0;
uint8_t _result_avaliable = 0;
uint8_t _result_initialized = 0;

OPEN_PROTOCOL_MUTEX_DECLARE(open_protocol_mutex);

// 将AI模块小端存储的数据转换为大端存储
uint16_t little2big(uint8_t first_num, uint8_t second_num)
{
    uint16_t result = 0;
    result = (second_num << 8) + first_num;
    return result;
}

RMAI_Results::RMAI_Results()
{
}

RMAI_Results::~RMAI_Results()
{
}

void RMAI_Results::begin()
{
    OPEN_PROTOCOL_MUTEX_INIT(open_protocol_mutex);
    _result_initialized = 1;
}

void RMAI_Results::setCallback(std::function<void(results_tag *, uint8_t)> func)
{
    this->_callback = func;
}

void RMAI_Results::run()
{
    // 未初始化不能执行，没有初始化Mutex
    if (1U != _result_initialized) return;

    OPEN_PROTOCOL_MUTEX_LOCK(open_protocol_mutex);

    if (1U == _result_avaliable)
    {
        _result_avaliable = 0;
        this->_result_num = result_num;
        memcpy(this->_results, results, this->_result_num * sizeof(results[0]));

        OPEN_PROTOCOL_MUTEX_UNLOCK(open_protocol_mutex);

        //回调
        this->_callback(this->_results, this->_result_num);
    }
    else
    {
        OPEN_PROTOCOL_MUTEX_UNLOCK(open_protocol_mutex);
    }
}

extern "C" {
void open_cmd_results(open_protocol_header_t *pack_desc)
{
    // 未初始化不能执行，没有初始化Mutex
    if (1U != _result_initialized) return;

    if (pack_desc->is_ack == 0)
    {
        //当命令码为0x0210时代表接受的数据是推理结果
        if (pack_desc->cmd_id == 0x0210)
        {
            OPEN_PROTOCOL_MUTEX_LOCK(open_protocol_mutex);

            //物体数量
            result_num = pack_desc->data_len / 8;
            //循环遍历data区的数据
            for (int i = 0; i < result_num; i++)
            {
                // 识别到物体的id
                results[i].id = pack_desc->data[0 + i * 8];
                //识别到物体的x坐标,使用little2big()函数将小端转为大端
                results[i].x = little2big(pack_desc->data[1 + i * 8], pack_desc->data[2 + i * 8]);
                //识别到物体的y坐标
                results[i].y = pack_desc->data[3 + i * 8];
                //识别到物体的w宽度,使用little2big()函数将小端转为大端
                results[i].w = little2big(pack_desc->data[4 + i * 8], pack_desc->data[5 + i * 8]);
                //识别到物体的h高度
                results[i].h = pack_desc->data[6 + i * 8];
                //识别到物体的c置信度
                results[i].c = pack_desc->data[8 + i * 8];
            }

            _result_avaliable = 1;

            OPEN_PROTOCOL_MUTEX_UNLOCK(open_protocol_mutex);
        }
    }
}
}