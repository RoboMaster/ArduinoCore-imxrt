/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "open_protocol.h"
#include "crc.h"

#include <stdlib.h>
#include <string.h>

/* Private define ------------------------------------------------------------*/
#define OPEN_PROTOCOL_CRC16_INIT             (0x4F19)
#define OPEN_PROTOCOL_CRC8_INIT              (0x11)
#define OPEN_PROTOCOL_SOF                    (0xAA)

#define OPEN_PROTOCOL_LOCAL_PORT             (0xFF)
#define OPEN_PROTOCOL_LOOKBACK_PORT          (0xFE)

#define RECV_BUFF_SIZE                       (256)

/* Private typedef -----------------------------------------------------------*/
enum open_unpack_stage
{
    STAGE_WAIT_HEADER = 0,
    STAGE_WAIT_BODY,
};

/* Extern variables ----------------------------------------------------------*/
uint32_t g_open_unpack_success_cnt = 0;
uint32_t g_open_unpack_fail_cnt = 0;

uint16_t g_local_addr = 0x101;

/* Private variables ---------------------------------------------------------*/                                     /*!< 缓冲区读指针  */
static uint32_t unpack_stage;                                           /*!< 解析阶段  */
static uint32_t unpack_wait_byte = sizeof(open_protocol_header_t);      /*!< 解析等待字节  */

static uint16_t pack_seq_cnt;

static open_handle_kv_t open_handle_kv[OPEN_PROTOCOL_CMD_HANDLE_NBR_MAX];
static uint32_t open_handle_kv_nbr;

static open_proto_port_t open_port[OPEN_PROTOCO_PORT_NUM_MAX];
static open_proto_route_t open_s_router_rule[OPEN_PROTOCO_ROUTE_NUM_MAX];

/* Private function prototypes -----------------------------------------------*/
static void handle_process(open_protocol_header_t* pack);
static void open_proto_route(open_protocol_header_t* pack, uint8_t src_port_idx);
static void open_proto_unpack(uint8_t* data, uint32_t len, uint8_t port_idx);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 对外开放协议初始化
 *
 * @param addr    本地地址
 */
void open_proto_init(uint16_t local_addr)
{
    for(int i = 0; i < OPEN_PROTOCO_PORT_NUM_MAX; i++)
    {
        open_port[i].buff_r_p = open_port[i].buff;
        open_port[i].buff_w_p = open_port[i].buff;
    }
    g_local_addr = local_addr;
}

/**
 * @brief 设置对外开放协议地址
 *
 * @param local_addr
 */
void open_proto_set_addr(uint16_t local_addr)
{
    g_local_addr = local_addr;
}

/**
 * @brief 协议接收
 *
 */
void open_proto_recv(void)
{
    uint8_t recv_buff[RECV_BUFF_SIZE];
    uint16_t ret_len;

    for(int i = 0; i < OPEN_PROTOCO_PORT_NUM_MAX; i++)
    {
        if(open_port[i].enable && open_port[i].recv_port != NULL)
        {
            ret_len = open_port[i].recv_port(recv_buff, RECV_BUFF_SIZE);
            if(ret_len > 0)
            {
                open_proto_unpack(recv_buff, ret_len, i);
            }
        }
    }
}

/**
 * @brief 发送正常包
 *
 * @param cmd_id    命令ID
 * @param dst_addr  目的地址
 * @param need_ack  该数据包需要Ack
 * @param data      数据内容
 * @param data_len  数据成功
 * @return int      0：发送成功 -1：发送失败
 */
int open_proto_send(uint16_t cmd_id, uint16_t dst_addr, uint8_t need_ack,
                     uint8_t* data, uint32_t data_len)
{
    if((data_len > OPEN_PROTOCOL_DATA_MAX_BYTE))
    {
        return -1;
    }

    uint8_t out_buff[data_len + OPEN_PROTOCOL_NON_DATA_SIZE];

    open_protocol_header_t *header   = (open_protocol_header_t *)(out_buff);
    uint16_t              *tail_crc = (uint16_t*)(header->data + data_len);

    header->sof = OPEN_PROTOCOL_SOF;
    header->data_len = data_len;
    header->attr = 0;
    header->seq_num = pack_seq_cnt++;
    header->src_addr = g_local_addr;
    header->dst_addr = dst_addr;
    header->is_ack = 0;
    header->need_ack = need_ack ? 1 : 0;
    header->crc8 = crc8_checksum_get((uint8_t*)header, GET_OFFSET(open_protocol_header_t, crc8), OPEN_PROTOCOL_CRC8_INIT);
    header->cmd_id = cmd_id;

    if(data_len > 0)
    {
        memcpy(header->data, data , data_len);
    }

    *tail_crc = crc16_checksum_get((uint8_t*)(header), sizeof(open_protocol_header_t) + header->data_len, OPEN_PROTOCOL_CRC16_INIT);

    open_proto_route(header, OPEN_PROTOCOL_LOCAL_PORT);

    return 0;
}

/**
 * @brief ACK响应包
 *
 * @param req_header
 * @param data
 * @param data_len
 * @return int
 */
int open_proto_ack(open_protocol_header_t* req_header,
                  uint8_t* data, uint32_t data_len)
{
    if(data_len > OPEN_PROTOCOL_DATA_MAX_BYTE)
    {
        return -1;
    }

    uint8_t out_buff[data_len + OPEN_PROTOCOL_NON_DATA_SIZE];

    open_protocol_header_t *header   = (open_protocol_header_t *)(out_buff);
    uint16_t              *tail_crc = (uint16_t*)(header->data + data_len);

    header->sof = OPEN_PROTOCOL_SOF;
    header->data_len = data_len;
    header->attr = 0;
    header->crc8 = crc8_checksum_get((uint8_t*)header, GET_OFFSET(open_protocol_header_t, crc8), OPEN_PROTOCOL_CRC8_INIT);
    header->seq_num = req_header->seq_num;
    header->src_addr = g_local_addr;
    header->dst_addr = req_header->src_addr;
    header->is_ack = 1;
    header->need_ack = 0;
    header->cmd_id = req_header->cmd_id;

    if(data_len > 0)
    {
        memcpy(header->data, data , data_len);
    }

    *tail_crc = crc16_checksum_get((uint8_t*)(header), sizeof(open_protocol_header_t) + header->data_len, OPEN_PROTOCOL_CRC16_INIT);

    open_proto_route(header, OPEN_PROTOCOL_LOCAL_PORT);

    return 0;
}

/**
 * @brief 添加一个发送接收接口
 *
 * @param name 接口名称
 * @param send_port 接口发送函数
 * @return int 返回接口的Index，返回-1为接口添加失败
 */
int open_proto_port_add(char* name, open_proto_send_port_t send_port, open_proto_recv_port_t recv_port)
{
    int ret = -1;
    for(int i = 0; i < OPEN_PROTOCO_PORT_NUM_MAX; i++)
    {
        if(!(open_port[i].is_init))
        {
            open_port[i].enable = 1;
            open_port[i].name = name;
            open_port[i].send_port = send_port;
            open_port[i].recv_port = recv_port;
            open_port[i].idx = i;
            open_port[i].is_init = 1;

            ret = open_port[i].idx ;
            break;
        }
    }
    return ret;
}

/**
 * @brief 使能/关闭 协议的接口
 *
 * @param port_idx 需要接口的Index
 * @param enable 非0：使能/0：关闭
 */
void open_proto_port_enable(uint8_t port_idx, uint8_t enable)
{
    if(port_idx < OPEN_PROTOCO_PORT_NUM_MAX && open_port[port_idx].is_init)
    {
        open_port[port_idx].enable = enable ? 1 : 0;
    }
}

/**
 * @brief 添加静态路由,匹配命中规则为：地址中掩码置位的位相等即为匹配命中
 *
 * @param dst_addr 目的路由地址
 * @param dst_mask 目的路由掩码
 * @param use_port_idx  目的路由所使用的接口
 * @param pri 当前路由的优先级，0~254:值低优先，255:当前路由无效
 * @return int 返回路由的Index，返回-1为路由添加失败
 */
int open_proto_static_route_add(uint16_t dst_addr, uint16_t dst_mask, uint8_t use_port_idx, uint8_t pri)
{
    int ret = -1;
    for(int i = 0; i < OPEN_PROTOCO_ROUTE_NUM_MAX; i++)
    {
        if(!(open_s_router_rule[i].is_init))
        {
            open_s_router_rule[i].is_init = 1;
            open_s_router_rule[i].idx = i;
            open_s_router_rule[i].dst_addr = dst_addr;
            open_s_router_rule[i].dst_mask = dst_mask;
            open_s_router_rule[i].use_port_idx = use_port_idx;
            open_s_router_rule[i].pri = pri;
            ret = open_s_router_rule[i].idx;
            break;
        }
    }
}

/**
 * @brief 设置静态路由的优先级
 *
 * @param router_idx 需要静态路由的Index
 * @param pri 设置路由优先级
 */
void open_proto_static_route_set_pri(uint8_t router_idx, uint8_t pri)
{
    if(router_idx < OPEN_PROTOCO_ROUTE_NUM_MAX && open_port[router_idx].is_init)
    {
        open_s_router_rule[router_idx].pri = pri;
    }
}

/**
 * @brief 设置本地地址
 *
 * @param addr
 */
void open_proto_set_local_addr(uint16_t addr)
{
    g_local_addr = addr;
}

/**
 * @brief 注册解包回调函数
 *
 * @param cmd_id 命令码
 * @param p_fun  对应命令码的回调函数
 * @return int
 */
int open_proto_handle_reg(uint32_t cmd_id, open_cmd_handler_t p_fun)
{
    if(open_handle_kv_nbr < OPEN_PROTOCOL_CMD_HANDLE_NBR_MAX)
    {
        open_handle_kv[open_handle_kv_nbr].cmd_id = cmd_id;
        open_handle_kv[open_handle_kv_nbr].p_fun = p_fun;
        open_handle_kv_nbr++;
        return 0;
    }
    else
    {
        return -1;
    }

}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief 包解析回调处理
 *
 * @param pack
 */
static void handle_process(open_protocol_header_t* pack)
{
    uint8_t have_cmd_id = 0;
    for(int i = 0; i < open_handle_kv_nbr; i++)
    {
        if(pack->cmd_id == open_handle_kv[i].cmd_id)
        {
            have_cmd_id = 1;
            open_handle_kv[i].p_fun(pack);
        }

        if(open_handle_kv[i].cmd_id == 0xFFFF)
        {
            open_handle_kv[i].p_fun(pack);
        }
    }

    if(have_cmd_id == 0)
    {
        for(int i = 0; i < open_handle_kv_nbr; i++)
        {
            if(open_handle_kv[i].cmd_id == 0xFFFE)
            {
                open_handle_kv[i].p_fun(pack);
            }
        }
    }
}

/**
 * @brief 路由包
 *
 * @param pack 需要发送的包
 * @param src_port_idx 包来源端口序列号，如由本机发出填0xFF(OPEN_PROTOCOL_LOCAL_PORT)
 */
static void open_proto_route(open_protocol_header_t* pack, uint8_t src_port_idx)
{
    uint16_t pack_len = pack->data_len + OPEN_PROTOCOL_NON_DATA_SIZE;

    if((pack->dst_addr == OPEN_PROTOCOL_LOCAL_ADDR) || (pack->dst_addr == g_local_addr))
    {
        if(src_port_idx == OPEN_PROTOCOL_LOCAL_PORT)
        {
            //路由回环包到本机解包
            open_proto_unpack((uint8_t*)pack, pack_len, OPEN_PROTOCOL_LOOKBACK_PORT);
        }
    }
    else if(pack->dst_addr == OPEN_PROTOCOL_BOARDCAST_ADDR)
    {
        //路由广播包到各个接口，除了来源接口
        for(int i = 0; i < OPEN_PROTOCO_PORT_NUM_MAX; i++)
        {
            if((open_port[i].enable) && (open_port[i].idx != src_port_idx) &&
                (open_port[i].send_port != NULL))
            {
                open_port[i].send_port((uint8_t*)pack, pack_len);
            }
        }
    }
    else
    {
        //路由其他正常包
        uint8_t cur_prio = 255;
        int match_port_idx = -1;

        for(int i = 0; i < OPEN_PROTOCO_ROUTE_NUM_MAX; i++)
        {
            uint16_t dst_addr = open_s_router_rule[i].dst_mask & open_s_router_rule[i].dst_addr;
            uint16_t cur_addr = open_s_router_rule[i].dst_mask & pack->dst_addr;
            if(open_s_router_rule[i].is_init && dst_addr == cur_addr && open_s_router_rule[i].pri < cur_prio)
            {
                uint8_t cur_port_idx = open_s_router_rule[i].use_port_idx;
                if(open_port[cur_port_idx].enable && open_port[cur_port_idx].send_port != NULL && (open_port[cur_port_idx].idx != src_port_idx))
                {
                    match_port_idx = cur_port_idx;
                    cur_prio = open_s_router_rule[i].pri;
                }
            }
        }
        if(match_port_idx >= 0 && match_port_idx < OPEN_PROTOCO_ROUTE_NUM_MAX)
        {
            open_port[match_port_idx].send_port((uint8_t*)pack, pack_len);
        }
    }
}

/**
 * @brief 外部包解析
 *
 * @param data
 * @param len
 */
static void open_proto_unpack(uint8_t* data, uint32_t len, uint8_t port_idx)
{
    if(port_idx >= OPEN_PROTOCO_PORT_NUM_MAX)
    {
        return;
    }

    uint8_t* data_end = data + len;

    while(data < data_end)
    {
        uint32_t buff_use = open_port[port_idx].buff_w_p - open_port[port_idx].buff_r_p;
        uint32_t buff_remain = OPEN_PROTOCOL_PACK_MAX_BYTE - buff_use;
        uint32_t copy_byte = buff_remain > len ? len : buff_remain;

        /* 判断尾部空间是否够，不够就向前移动 */
        if (OPEN_PROTOCOL_PACK_MAX_BYTE - (open_port[port_idx].buff_w_p - open_port[port_idx].buff) < copy_byte)
        {
            memmove(open_port[port_idx].buff, open_port[port_idx].buff_r_p, buff_use);
            open_port[port_idx].buff_r_p = open_port[port_idx].buff;
            open_port[port_idx].buff_w_p = open_port[port_idx].buff + buff_use;
        }

        /* 将读入内容拷贝至缓冲区 */
        memcpy(open_port[port_idx].buff_w_p, data, copy_byte);
        data += copy_byte;
        open_port[port_idx].buff_w_p += copy_byte;

        /* 开始解包 */
        while(open_port[port_idx].buff_w_p - open_port[port_idx].buff_r_p >= unpack_wait_byte)
        {
            open_protocol_header_t *header = (open_protocol_header_t *)(open_port[port_idx].buff_r_p);

            switch(unpack_stage)
            {
                case STAGE_WAIT_HEADER:
                {
                    /* 判断前导码、Header CRC16、包长度是否符合*/
                    if((header->sof == OPEN_PROTOCOL_SOF) &&
                       ((header->crc8 == crc8_checksum_get((uint8_t*)(header), GET_OFFSET(open_protocol_header_t, crc8), OPEN_PROTOCOL_CRC8_INIT)) ||
                            (!OPEN_PROTOCOL_USE_CRC)) &&
                       (header->data_len <= OPEN_PROTOCOL_DATA_MAX_BYTE))
                    {
                        /* Header解析接收成功，等待剩余字节 */
                        unpack_stage = STAGE_WAIT_BODY;
                        unpack_wait_byte = OPEN_PROTOCOL_NON_DATA_SIZE + header->data_len;
                    }
                    else
                    {
                        /* Header解析失败，从下一个字节继续解析 */
                        open_port[port_idx].buff_r_p++;
                        g_open_unpack_fail_cnt++;
                    }
                    break;
                }

                case STAGE_WAIT_BODY:
                {
                    /* 校验尾部CRC16 */
                    uint16_t tail_crc = *((uint16_t*)(header->data + header->data_len));
                    if((crc16_checksum_get((uint8_t*)(header),sizeof(open_protocol_header_t) + header->data_len, OPEN_PROTOCOL_CRC16_INIT) == tail_crc) ||
                        (!OPEN_PROTOCOL_USE_CRC))
                    {
                        /* 包数据全部解析成功*/
                        if((header->dst_addr == OPEN_PROTOCOL_LOCAL_ADDR) || (header->dst_addr == g_local_addr))
                        {
                            /* 接收地址匹配本机，跳转对应回调处理 */
                            handle_process(header);
                        }
                        else if(header->dst_addr == OPEN_PROTOCOL_BOARDCAST_ADDR)
                        {
                            /* 广播地址，先路由转发然后跳转对应回调处理 */
#if OPEN_PROTOCOL_ROUTER_ENABLE == 1
                            open_proto_route(header, port_idx);
#endif
                            handle_process(header);

                        }
                        else
                        {
                            /* 其他非本机地址，路由转发 */
#if OPEN_PROTOCOL_ROUTER_ENABLE == 1
                            open_proto_route(header, port_idx);
#endif
                        }

                        open_port[port_idx].buff_r_p += sizeof(open_protocol_header_t) + header->data_len + OPEN_PROTOCOL_TAIL_CRC_SIZE;
                        g_open_unpack_success_cnt++;
                    }
                    else
                    {
                        /* 校验失败，跳过包头继续解析 */
                        open_port[port_idx].buff_r_p += sizeof(open_protocol_header_t);
                        g_open_unpack_fail_cnt++;
                    }
                    unpack_stage = STAGE_WAIT_HEADER;
                    unpack_wait_byte = sizeof(open_protocol_header_t);
                    break;
                }
            }
        }
    }
}
