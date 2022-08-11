/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "ext_protocol.h"

#include <stdlib.h>
#include <string.h>


#define EXT_PROTOCOL_CRC16_INIT             (0xFFFF)
#define EXT_PROTOCOL_RECV_SEQ_WINDOW        (0)
#define EXT_PROTOCOL_RECV_SEQ_RESET_TIME    (3000)
#define EXT_PROTOCOL_PREAMBLE_CODE          (0xA5A5)

enum ext_unpack_stage
{
    STAGE_WAIT_HEADER = 0,
    STAGE_WAIT_BODY,
};

static uint8_t  unpack_buff[EXT_PROTOCOL_PACK_MAX_BYTE];    /*!< 包解析缓冲区  */
static uint8_t* unpack_buff_w_ptr = unpack_buff;        /*!< 缓冲区写指针  */
static uint8_t* unpack_buff_r_ptr = unpack_buff;        /*!< 缓冲区读指针  */

static uint32_t unpack_stage;                                       /*!< 解析阶段  */
static uint32_t unpack_wait_byte = sizeof(ext_protocol_header_t);     /*!< 解析等待字节  */

static uint16_t pack_seq_cnt;

static ext_handle_kv_t ext_handle_kv[EXT_PROTOCOL_CMD_HANDLE_NBR_MAX];
static uint32_t ext_handle_kv_nbr;

uint32_t g_unpack_success_cnt = 0;
uint32_t g_unpack_fail_cnt = 0;


/**
 * @brief CRC16
 *
 * @param pdata
 * @param len
 * @return uint16_t
 */
static uint16_t crc16(uint8_t *pdata, uint32_t len, uint16_t sum)
{

    while (len--)
    {
        sum  = (sum >> 8) | (sum << 8);
        sum ^= *pdata++;
        sum ^= ((uint8_t) sum) >> 4;
        sum ^= sum << 12;
        sum ^= (sum & 0xFF) << 5;
    }

    return sum;
}

/**
 * @brief 包解析回调处理
 *
 * @param pack
 */
static void ext_handle_main(ext_protocol_header_t* pack)
{
    ext_protocol_body_t* body = (ext_protocol_body_t*)(pack->body);

    for(int i = 0; i < ext_handle_kv_nbr; i++)
    {
        if(body->cmd_id == ext_handle_kv[i].cmd_id)
        {
            ext_handle_kv[i].p_fun(pack);
        }
        else if(ext_handle_kv[i].cmd_id == 0xFFFE)
        {
            ext_handle_kv[i].p_fun(pack);
        }

        if(ext_handle_kv[i].cmd_id == 0xFFFF)
        {
            ext_handle_kv[i].p_fun(pack);
        }
    }
}


/**
 * @brief 注册解包回调函数
 *
 * @param cmd_id 命令码
 * @param p_fun  对应命令码的回调函数
 * @return int
 */
int ext_handle_reg(uint32_t cmd_id, cmd_handler_t p_fun)
{
    if(ext_handle_kv_nbr < EXT_PROTOCOL_CMD_HANDLE_NBR_MAX)
    {
        ext_handle_kv[ext_handle_kv_nbr].cmd_id = cmd_id;
        ext_handle_kv[ext_handle_kv_nbr].p_fun = p_fun;
        ext_handle_kv_nbr++;
        return 0;
    }
    else
    {
        return -1;
    }

}

/**
 * @brief 外部包解析
 *
 * @param data
 * @param len
 */
void ext_unpack(uint8_t* data, uint32_t len)
{
    uint8_t* data_end = data + len;

    while(data < data_end)
    {
        uint32_t buff_use = unpack_buff_w_ptr - unpack_buff_r_ptr;
        uint32_t buff_remain = EXT_PROTOCOL_PACK_MAX_BYTE - buff_use;
        uint32_t copy_byte = buff_remain > len ? len : buff_remain;

        /* 判断尾部空间是否够，不够就向前移动 */
        if (EXT_PROTOCOL_PACK_MAX_BYTE - (unpack_buff_w_ptr - unpack_buff) < copy_byte)
        {
            memmove(unpack_buff, unpack_buff_r_ptr, buff_use);
            unpack_buff_r_ptr = unpack_buff;
            unpack_buff_w_ptr = unpack_buff + buff_use;
        }

        /* 将读入内容拷贝至缓冲区 */
        memcpy(unpack_buff_w_ptr, data, copy_byte);
        data += copy_byte;
        unpack_buff_w_ptr += copy_byte;

        /* 开始解包 */
        while(unpack_buff_w_ptr - unpack_buff_r_ptr >= unpack_wait_byte)
        {
            ext_protocol_header_t *header = (ext_protocol_header_t *)unpack_buff_r_ptr;

            switch(unpack_stage)
            {
                case STAGE_WAIT_HEADER:
                {
                    /* 判断前导码、Header CRC16、包长度是否符合*/
                    if((header->preamble == EXT_PROTOCOL_PREAMBLE_CODE) &&
                       ((header->crc16 == crc16((uint8_t*)(header), GET_OFFSET(ext_protocol_header_t, crc16), EXT_PROTOCOL_CRC16_INIT)) ||
                            (!EXT_PROTOCOL_USE_CRC)) &&
                       (header->body_len <= EXT_PROTOCOL_BODY_MAX_BYTE))
                    {
                        /* Header解析接收成功，等待剩余字节 */
                        unpack_stage = STAGE_WAIT_BODY;
                        unpack_wait_byte = EXT_PROTOCOL_NON_BODY_SIZE + header->body_len;
                    }
                    else
                    {
                        /* Header解析失败，从下一个字节继续解析 */
                        unpack_buff_r_ptr++;
                        g_unpack_fail_cnt++;
                    }
                    break;
                }

                case STAGE_WAIT_BODY:
                {
                    /* 校验尾部CRC16 */
                    uint16_t tail_crc = *((uint16_t*)(header->body + header->body_len));
                    if((crc16((uint8_t*)(header),sizeof(ext_protocol_header_t) + header->body_len, EXT_PROTOCOL_CRC16_INIT) == tail_crc) ||
                        (!EXT_PROTOCOL_USE_CRC))
                    {
                        /* 包数据全部解析成功，跳转对应回调 */
                        ext_handle_main(header);
                        unpack_buff_r_ptr += sizeof(ext_protocol_header_t) + header->body_len + EXT_PROTOCOL_TAIL_CRC_SIZE;
                        g_unpack_success_cnt++;
                    }
                    else
                    {
                        /* 校验失败，跳过包头继续解析 */
                        unpack_buff_r_ptr += sizeof(ext_protocol_header_t);
                        g_unpack_fail_cnt++;
                    }
                    unpack_stage = STAGE_WAIT_HEADER;
                    unpack_wait_byte = sizeof(ext_protocol_header_t);
                    break;
                }
            }
        }
    }
}


/**
 * @brief 数据包封装
 *
 * @param data
 * @param out_buff
 * @param cmd_id
 * @param data_len
 * @param out_buff_size
 * @return int
 */
int ext_pack(uint8_t* data, uint8_t* out_buff, uint16_t cmd_id, uint32_t data_len, uint32_t out_buff_size)
{
    if((data_len > EXT_PROTOCOL_DATA_MAX_BYTE) || (out_buff_size < data_len + EXT_PROTOCOL_NON_DATA_SIZE))
    {
        return -1;
    }

    ext_protocol_header_t *header   = (ext_protocol_header_t *)(out_buff);
    ext_protocol_body_t   *body     = (ext_protocol_body_t*)(header->body);
    uint16_t              *tail_crc = (uint16_t*)(body->data + data_len);

    header->preamble = EXT_PROTOCOL_PREAMBLE_CODE;
    header->body_len = data_len + EXT_PROTOCOL_BODY_CMD_SIZE;
    header->control_word = 0;
    header->seq_num = pack_seq_cnt++;
    header->crc16 = crc16((uint8_t*)header, GET_OFFSET(ext_protocol_header_t, crc16), EXT_PROTOCOL_CRC16_INIT);
    body->cmd_id = cmd_id;

    if(data_len > 0)
    {
        memcpy(body->data, data , data_len);
    }

    *tail_crc = crc16((uint8_t*)(header), sizeof(ext_protocol_header_t) + header->body_len, EXT_PROTOCOL_CRC16_INIT);

    return data_len  + EXT_PROTOCOL_NON_DATA_SIZE;
}
