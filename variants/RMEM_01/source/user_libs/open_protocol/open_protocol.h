/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __OPEN_PROTOCOL_H__
#define __OPEN_PROTOCOL_H__

#include <stdint.h>

#define OPEN_PROTOCOL_USE_CRC                (1)
#define OPEN_PROTOCOL_CMD_HANDLE_NBR_MAX     (32)

#define OPEN_PROTOCOL_ROUTER_ENABLE          (1)

#ifndef OPEN_PROTOCO_PORT_NUM_MAX
    #define OPEN_PROTOCO_PORT_NUM_MAX            (5)
#endif

#ifndef OPEN_PROTOCO_ROUTE_NUM_MAX
    #define OPEN_PROTOCO_ROUTE_NUM_MAX           (10)
#endif

#define GET_OFFSET(type,member)              ((size_t)(&(((typeof(type)*)0)->member)))   //从结构体中获得成员的位置偏移
#define MEM_SIZE(type, member)               ((size_t)(sizeof(((type*)0)->member)))      //从结构体中获得成员的大小
#define OPEN_PROTOCOL_PACK_MAX_BYTE          (512)
#define OPEN_PROTOCOL_TAIL_CRC_SIZE          (sizeof(uint16_t))
#define OPEN_PROTOCOL_NON_DATA_SIZE          (sizeof(open_protocol_header_t) + OPEN_PROTOCOL_TAIL_CRC_SIZE)
#define OPEN_PROTOCOL_DATA_MAX_BYTE          (OPEN_PROTOCOL_PACK_MAX_BYTE - OPEN_PROTOCOL_NON_DATA_SIZE)

#define OPEN_PROTOCOL_LOCAL_ADDR             (0x0000)
#define OPEN_PROTOCOL_BOARDCAST_ADDR         (0xFFFF)
#pragma pack(push, 1)

/**
 * @brief 开放协议Header
 *
 */
typedef struct
{
    uint8_t    sof;         /*!< 前导码 固定为0xAA  */
    uint16_t   data_len;    /*!< 数据段长度  */
    uint8_t    crc8;        /*!< 对以上字节的CRC8校验  */
    uint16_t   src_addr;    /*!< 源地址  */
    uint16_t   dst_addr;    /*!< 目标地址  */
    union
    {
        struct
        {
            uint8_t is_ack   : 1;  /*!< 本包是ACK包  */
            uint8_t need_ack : 1;  /*!< 本包需要ACK */
            uint8_t res:       6;
        };
        uint8_t attr;
    };
    uint16_t   seq_num;     /*!< 序列号  */
    uint16_t   cmd_id;      /*!< 命令号  */
    uint8_t    data[];      /*!< data段  */
}open_protocol_header_t;

#pragma pack(pop)

typedef void (*open_cmd_handler_t)(open_protocol_header_t *pack_desc);          /*!< 解包回调函数  */
typedef void (*open_proto_send_port_t)(uint8_t* data, uint16_t data_size);      /*!< 对外发送接口函数  */
typedef uint16_t (*open_proto_recv_port_t)(uint8_t* buff, uint16_t buff_size);  /*!< 数据接收接口函数  */


/**
 * @brief 协议接口结构体
 *
 */
typedef struct
{
    uint8_t                   is_init;      /*!< 该接口是否已被初始化  */
    char*                     name;         /*!< 该接口的友好标识名  */
    uint8_t                   idx;          /*!< 该接口的序号  */
    open_proto_send_port_t    send_port;    /*!< 发送端口函数  */
    open_proto_recv_port_t    recv_port;    /*!< 接收端口函数  */
    uint8_t                   enable;       /*!< 端口使能  */

    uint8_t                   buff[OPEN_PROTOCOL_PACK_MAX_BYTE];     /*!< 包解析缓冲区  */
    uint8_t                   *buff_w_p;                             /*!< 缓冲区读指针  */
    uint8_t                   *buff_r_p;                             /*!< 缓冲区写指针  */
}open_proto_port_t;


/**
 * @brief 协议接口结构体
 *
 */
typedef struct
{
    uint8_t                   is_init;          /*!< 该路由是否已被初始化  */
    uint8_t                   idx;              /*!< 该路由的序号  */
    uint16_t                  dst_addr;         /*!< 目的地址  */
    uint16_t                  dst_mask;         /*!< 目的地址的掩码  */
    uint8_t                   use_port_idx;     /*!< 该路由使用接口的序号  */
    uint8_t                   pri;              /*!< 该路由优先级 */
}open_proto_route_t;

/**
 * @brief 外部协议回调函数kv对
 *
 */
typedef struct
{
    uint16_t      cmd_id;           /*!< 命令码  */
    open_cmd_handler_t p_fun;       /*!< 解包函数  */
}open_handle_kv_t;

void open_proto_init(uint16_t local_addr);
void open_proto_set_addr(uint16_t local_addr);
void open_proto_recv(void);

int open_proto_send(uint16_t cmd_id, uint16_t dst_addr, uint8_t need_ack,
                     uint8_t* data, uint32_t data_len);
int open_proto_ack(open_protocol_header_t* req_header,
                  uint8_t* data, uint32_t data_len);

int open_proto_port_add(char* name, open_proto_send_port_t send_port, open_proto_recv_port_t recv_port);
void open_proto_port_enable(uint8_t port_idx, uint8_t enable);

int open_proto_static_route_add(uint16_t dst_addr, uint16_t dst_mask, uint8_t port_idx, uint8_t pri);
void open_proto_static_route_set_pri(uint8_t router_idx, uint8_t pri);

void open_proto_set_local_addr(uint16_t addr);
int open_proto_handle_reg(uint32_t cmd_id, open_cmd_handler_t p_fun);

#endif // __OPEN_PROTOCOL_H__
