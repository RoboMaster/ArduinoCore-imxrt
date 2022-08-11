/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __OPEN_PROTOCOL_COMMON_H__
#define __OPEN_PROTOCOL_COMMON_H__

#include "stdint.h"
#include "open_protocol.h"


#pragma pack(push, 1)

typedef struct
{
    uint8_t err_code;
}open_comm_rsp_t;

#define OPEN_CMD_REBOOT                     (0x0001)

#define OPEN_CMD_VERSION                    (0x0002)

typedef struct
{
    uint32_t loader_ver;
    uint32_t app_ver;
    char     hw_id[16];
    char     sn[16];
}open_cmd_version_rsp_t;

#define OPEN_CMD_ECHO                       (0x0003)

typedef struct
{
    uint8_t error;
    uint16_t sn_crc16;
}open_cmd_echo_rsp_t;

#define OPEN_CMD_SET_ID                     (0x0004)

typedef struct
{
    uint8_t  set_id;
    char     sn[16];
}open_cmd_set_id_req_t;

typedef struct
{
    uint16_t sn_crc16;
    uint8_t error;
}open_cmd_set_id_rsp_t;

#define OPEN_CMD_UPGRADE_ENTER              (0x0021)


#define OPEN_CMD_LED_TEST                   (0x01F0)
typedef struct
{
    uint8_t en;
}open_cmd_led_test_t;

#define OPEN_CMD_OLD_PROTO_PUSH_FOWARD      (0x01E0)
#define OPEN_CMD_OLD_PROTO_REQ_ACK_FOWARD   (0x01E1)
#define OPEN_CMD_OLD_PROTO_HEART_PUSH       (0x01E2)

typedef struct
{
    uint8_t msg_num;
    uint16_t msg[];
}open_cmd_old_proto_heart_push_t;

#define OPEN_CMD_SD_CARD_TEST               (0x01F1)
#define OPEN_CMD_CAN_TEST                   (0x01F2)
#define OPEN_CMD_SLOT14_TEST                (0x01F3)
#define OPEN_CMD_SLOT100_TEST               (0x01F4)

#define OPEN_CMD_EN_AI_BOARD_DET            (0x01F5)
typedef struct
{
    uint8_t en;
}open_cmd_en_ai_board_det_t;

#define OPEN_CMD_EN_AI_BOARD_IMG            (0x01F6)
typedef struct
{
    uint8_t en;
}open_cmd_en_ai_board_img_req_t;

#define OPEN_CMD_BURN_SN                    (0x01F7)
typedef struct
{
    uint8_t sn[16];
}open_cmd_burn_sn_req;

#define OPEN_CMD_V1_VER                     (0x01F8)
typedef struct
{
    uint8_t err_code;
    struct
    {
        uint8_t minor:4;
        uint8_t major:4;
    } ver;
    uint8_t hw_ver[16];
    uint32_t loader_ver;
    uint32_t firmware_ver;
    uint32_t set_supported;
    uint8_t goback;
}open_cmd_ver_query_rsp;

#define OPEN_AI_CORE_SPI_VIDEO_EN           (0x0202)
typedef struct
{
    uint8_t enable;
}open_cmd_ai_core_spi_video_en_req_t;

#define OPEN_AI_CORE_DET_PUSH               (0x0210)

typedef struct
{
    struct
    {
        uint8_t     id;
        uint16_t    x;
        uint8_t     y;
        uint16_t    w;
        uint8_t     h;
        uint8_t     C;
    }tag[20];        //最多支持20个
}open_ai_core_det_push_t;

#define OPEN_AI_CORE_SET_UART_SPEED_REQ     (0x020E)
typedef struct
{
    uint32_t baudrate;
}open_ai_core_set_uart_speed_req_t;

#pragma pack(pop)

void open_cmd_default(open_protocol_header_t *pack_desc);
void open_cmd_reboot(open_protocol_header_t *pack_desc);
void open_cmd_ver(open_protocol_header_t *pack_desc);
void open_cmd_echo(open_protocol_header_t *pack_desc);
void open_cmd_set_id(open_protocol_header_t *pack_desc);
void open_cmd_enter_loader(open_protocol_header_t *pack_desc);
void open_cmd_led_test(open_protocol_header_t *pack_desc);
void open_cmd_sd_card_test(open_protocol_header_t *pack_desc);
void open_cmd_can_test(open_protocol_header_t *pack_desc);
void open_cmd_slot14_test(open_protocol_header_t *pack_desc);
void open_cmd_slot100_test(open_protocol_header_t *pack_desc);
void open_cmd_en_ai_board_img(open_protocol_header_t *pack_desc);
void open_cmd_burn_sn(open_protocol_header_t *pack_desc);

void open_cmd_v1_ver(open_protocol_header_t *pack_desc);

int get_proxy_common_idx(uint16_t ext_cmd);
void open_cmd_proxy_common(open_protocol_header_t *pack_desc);
#endif
