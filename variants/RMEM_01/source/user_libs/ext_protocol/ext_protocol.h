/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __EXT_PROTOCOL_H__
#define __EXT_PROTOCOL_H__

#include <stdint.h>

#define EXT_PROTOCOL_USE_CRC                (1)
#define EXT_PROTOCOL_CMD_HANDLE_NBR_MAX     (32)

#define GET_OFFSET(type,member)             ((size_t)(&(((typeof(type)*)0)->member)))   //从结构体中获得成员的位置偏移
#define MEM_SIZE(type, member)              ((size_t)(sizeof(((type*)0)->member)))      //从结构体中获得成员的大小
#define EXT_PROTOCOL_PACK_MAX_BYTE          (512)
#define EXT_PROTOCOL_TAIL_CRC_SIZE          (sizeof(uint16_t))
#define EXT_PROTOCOL_BODY_CMD_SIZE          (MEM_SIZE(ext_protocol_body_t, cmd_id))
#define EXT_PROTOCOL_NON_BODY_SIZE          (EXT_PROTOCOL_TAIL_CRC_SIZE + sizeof(ext_protocol_header_t))
#define EXT_PROTOCOL_NON_DATA_SIZE          (EXT_PROTOCOL_NON_BODY_SIZE + EXT_PROTOCOL_BODY_CMD_SIZE)
#define EXT_PROTOCOL_BODY_MAX_BYTE          (EXT_PROTOCOL_PACK_MAX_BYTE - sizeof(ext_protocol_header_t) - EXT_PROTOCOL_TAIL_CRC_SIZE)
#define EXT_PROTOCOL_DATA_MAX_BYTE          (EXT_PROTOCOL_BODY_MAX_BYTE - MEM_SIZE(ext_protocol_body_t, cmd_id))

#pragma pack(push, 1)

/**
 * @brief 外部协议Header
 *
 */
typedef struct
{
    uint16_t   preamble;    /*!< 前导码 固定为0xA5 0xA5  */
    union
    {
        struct
        {
            uint32_t res  : 28; /*!< 保留  */
            uint32_t ver  : 4;  /*!< 协议版本，当前为0 */
        };
        uint32_t control_word;
    };
    uint16_t   body_len;    /*!< 数据段长度  */
    uint16_t   seq_num;     /*!< 序列号  */
    uint16_t   crc16;       /*!< 对以上字节的CRC16校验  */
    uint8_t    body[];      /*!< Body段  */
}ext_protocol_header_t;

/**
 * @brief 外部协议Body
 *
 */
typedef struct
{
    uint16_t   cmd_id;      /*!< 命令码  */
    uint8_t    data[];      /*!< 数据  */
}ext_protocol_body_t;

#pragma pack(pop)

typedef void (*cmd_handler_t)(ext_protocol_header_t *pack_desc);        /*!< 解包回调函数  */

/**
 * @brief 外部协议回调函数kv对
 *
 */
typedef struct
{
    uint16_t      cmd_id;   /*!< 命令码  */
    cmd_handler_t p_fun;    /*!< 解包函数  */
}ext_handle_kv_t;


int ext_handle_reg(uint32_t cmd_id, cmd_handler_t p_fun);
void ext_unpack(uint8_t* data, uint32_t len);
int ext_pack(uint8_t* data, uint8_t* out_buff, uint16_t cmd_id, uint32_t data_len, uint32_t out_buff_size);

#endif // __EXT_PROTOCOL_H__
