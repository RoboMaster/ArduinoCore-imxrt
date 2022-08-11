/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "ext_protocol_cmd.h"
#include "ext_protocol.h"
#include "open_protocol_cmd.h"
#include "open_protocol.h"
#include "task_protocol.h"
#include "board.h"
#include "cmsis_os.h"
#include "string.h"
#include "log.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern log_module_t* g_log_proto;
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 默认命令解包
 *
 * @param pack_desc
 */

void ext_cmd_default(ext_protocol_header_t *pack_desc)
{
    open_proto_send(OPEN_CMD_OLD_PROTO_PUSH_FOWARD, 0x0406, 0, (uint8_t*)(pack_desc->body), pack_desc->body_len);
}

/**
 * @brief 所有命令解包
 *
 * @param pack_desc
 */
void ext_cmd_all(ext_protocol_header_t *pack_desc)
{

}

/**
 * @brief 通用转发处理 临时使用，正式版废弃
 *
 * @param pack_desc
 */

extern open_protocol_header_t g_proxy_common_open_header[];
extern uint8_t g_proxy_common_need_ack[];

void ext_cmd_commmon_forward_handle(ext_protocol_header_t *pack_desc)
{
    ext_protocol_body_t* ext_body = (ext_protocol_body_t*)pack_desc->body;

    int idx = get_proxy_common_idx(ext_body->cmd_id);

    if(idx < 0)
    {
        return;
    }

    if(g_proxy_common_need_ack[idx])
    {
        /* 转换为OPEN协议上发 */
        g_proxy_common_need_ack[idx] = 0;

        if (g_proxy_common_open_header[idx].need_ack)
        {
            open_proto_ack(&(g_proxy_common_open_header[idx]), pack_desc->body, pack_desc->body_len);
            log_printf(g_log_proto, 0, LOG_DEBUG_L3, "Open protocol proxy v1 ack: id:0x%04x, len:%d.",
                ext_body->cmd_id, pack_desc->body_len);
        }
    }
}
