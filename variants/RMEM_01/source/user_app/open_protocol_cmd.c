/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "open_protocol_cmd.h"
#include "slot_factory_test.h"
#include "open_protocol.h"
#include "open_protocol_error.h"
#include "ext_protocol.h"
#include "task_protocol.h"
#include "user_board.h"
#include "cmsis_os.h"
#include "string.h"
#include "log.h"
#include "user_driver_uart.h"
// #include "sd_file.h"
#include "can.h"
#include "sys_param.h"
#include "user_main.h"
#include "cmsis_os.h"

/* Private define ------------------------------------------------------------*/
#define STOP_BOOT_APP_FLAG_VAR          (0x1234abcd)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t g_factory_led_test_flag;
extern log_module_t* g_log_proto;
extern uint16_t g_sn_crc16;
extern sys_param_t g_sys_param;                        //系统参数

//用于停止启动的标志(APP和Loader中都不初始化，指定位置0x2020000），重新上电可以被Loader读到
//如果等于0x1234abcd则停留在Loader中，该变量进入Loader后置0
uint32_t *stop_boot_app_flag = (uint32_t*)(0x20200000);

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 默认命令解包
 *
 * @param pack_desc
 */

void open_cmd_default(open_protocol_header_t *pack_desc)
{
    uint8_t ack_data = 0;
    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            open_proto_ack(pack_desc, &ack_data, 1);
        }
    }
}

/**
 * @brief 重启
 *
 * @param pack_desc
 */
void open_cmd_reboot(open_protocol_header_t *pack_desc)
{
    open_cmd_version_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        board_sys_reboot();
        if(pack_desc->need_ack)
        {
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief 查询版本
 *
 * @param pack_desc
 */
void open_cmd_ver(open_protocol_header_t *pack_desc)
{
    open_cmd_version_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            sys_param_t param;
            sys_param_read(&param);
            memset(&rsp, 0, sizeof(rsp));
            rsp.loader_ver = param.loader_ver;
            rsp.app_ver = APP_VERSION;
            memcpy(rsp.hw_id, HW_VER_ID, sizeof(HW_VER_ID));
            board_sn_read(rsp.sn);
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief ECHO
 *
 * @param pack_desc
 */
void open_cmd_echo(open_protocol_header_t *pack_desc)
{
    open_cmd_echo_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.error = 0;
            rsp.sn_crc16 = g_sn_crc16;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief 被设置ID
 *
 * @param pack_desc
 */
void open_cmd_set_id(open_protocol_header_t *pack_desc)
{
    open_cmd_set_id_req_t *req = (open_cmd_set_id_req_t*)(pack_desc->data);
    open_cmd_set_id_rsp_t rsp;
    uint8_t read_sn[16];
    uint8_t obj_sn[16];

    if(pack_desc->is_ack == 0 && pack_desc->data_len == sizeof(open_cmd_set_id_req_t))
    {
        memcpy(obj_sn, req->sn, 16);
        board_sn_read(read_sn);
        read_sn[15] = '\0';
        obj_sn[15] = '\0';

        uint8_t sn_len = strlen(read_sn) > strlen(obj_sn) ? strlen(read_sn) : strlen(obj_sn);

        if(memcmp(obj_sn, read_sn, sn_len) == 0)
        {
            rsp.sn_crc16 = g_sn_crc16;
            if(req->set_id <= 4 && req->set_id != 0)
            {
                rsp.error = 0;
                if(pack_desc->need_ack)
                {
                    open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
                }

                g_sys_param.id = req->set_id;
                open_proto_set_addr(req->set_id | 0x0200);
                sys_param_save(&g_sys_param);
            }
            else
            {
                rsp.error = OPEN_PROTO_INVALID_PARAM;
                if(pack_desc->need_ack)
                {
                    open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
                }
            }
        }
    }
}

/**
 * @brief 进入Loader并停留在Loader
 *
 * @param pack_desc
 */
void open_cmd_enter_loader(open_protocol_header_t *pack_desc)
{
    open_comm_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        *stop_boot_app_flag = STOP_BOOT_APP_FLAG_VAR;
        if(pack_desc->need_ack)
        {
            rsp.err_code = 0;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
            osDelay(2);
        }
        board_boot_1rd_pattern();
    }
}

/**
* @brief LED测试
*
* @param pack_desc
*/
void open_cmd_led_test(open_protocol_header_t *pack_desc)
{
    open_cmd_led_test_t *req = (open_cmd_led_test_t*)(pack_desc->data);

    g_factory_led_test_flag = req->en;

    open_comm_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.err_code = 0;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief SD卡读取测试(SD卡FAT格式化，根目录放一个test.txt文件，内容为"Hello")
 *
 */
void open_cmd_sd_card_test(open_protocol_header_t *pack_desc)
{
    // open_comm_rsp_t rsp;
    // int ret = sd_file_test();

    // if(pack_desc->is_ack == 0)
    // {
    //     if(pack_desc->need_ack)
    //     {
    //         rsp.err_code = ret == 0 ? 0 : 1;
    //         open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
    //     }
    // }
}

/**
 * @brief CAN测试
 *
 */
void open_cmd_can_test(open_protocol_header_t *pack_desc)
{
    open_comm_rsp_t rsp;
    int ret = can_test();

    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.err_code = ret == 0 ? 0 : 1;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief 14Pin插座链路测试
 *
 */
void open_cmd_slot14_test(open_protocol_header_t *pack_desc)
{
    open_comm_rsp_t rsp;
    int ret = slot_14pin_test();

    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.err_code = ret == 0 ? 0 : 1;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
 * @brief 100Pin插座链路测试
 *
 */
void open_cmd_slot100_test(open_protocol_header_t *pack_desc)
{
    open_comm_rsp_t rsp;
    int ret = slot_100pin_test();

    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.err_code = ret == 0 ? 0 : 1;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
* @brief 打开/关闭 AI核心板图像传输
*
* @param pack_desc
*/
void open_cmd_en_ai_board_img(open_protocol_header_t *pack_desc)
{
    open_cmd_en_ai_board_img_req_t *req = (open_cmd_en_ai_board_img_req_t*)(pack_desc->data);
    open_comm_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        // ext_spi_vedio_req_t k210_req;
        // k210_req.open = req->en ? 1 : 0;
        // uint8_t ext_buff[EXT_PROTOCOL_NON_DATA_SIZE + sizeof(ext_spi_vedio_req_t)];  //17bytes
        // int ret = ext_pack((uint8_t*)(&k210_req), ext_buff, EXT_SPI_VEDIO_STREAM_REQ, sizeof(k210_req), sizeof(ext_buff));
        // if(ret > 0)
        // {
        //     uart_send(UART3, (char*)ext_buff, ret);
        // }

        if(pack_desc->need_ack)
        {
            rsp.err_code = 0;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/**
* @brief 烧录SN
*
* @param pack_desc
*/
void open_cmd_burn_sn(open_protocol_header_t *pack_desc)
{
    open_cmd_burn_sn_req *req = (open_cmd_burn_sn_req*)(pack_desc->data);
    open_comm_rsp_t rsp;
    if(pack_desc->is_ack == 0)
    {
        int ret = board_sn_write(req->sn);
        g_sn_crc16 = board_sn_crc16();
        if(pack_desc->need_ack)
        {
            rsp.err_code = ret == 0 ? 0 : 255;
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}


/**
* @brief 支持原有V1升级逻辑的查询版本，用于大包升级
*
* @param pack_desc
*/
void open_cmd_v1_ver(open_protocol_header_t *pack_desc)
{
    open_cmd_ver_query_rsp rsp;
    if(pack_desc->is_ack == 0)
    {
        if(pack_desc->need_ack)
        {
            rsp.err_code = 0;
            rsp.ver.major = 1;
            rsp.ver.minor = 0;
            rsp.goback = 0;
            rsp.set_supported = 0x09;
            rsp.firmware_ver = APP_VERSION;
            rsp.loader_ver = 0xFFFFFFFF;
            memcpy(rsp.hw_ver, HW_VER_ID, sizeof(HW_VER_ID));
            open_proto_ack(pack_desc, (uint8_t*)(&rsp), sizeof(rsp));
        }
    }
}

/* 临时支持AI核心板的v1转发逻辑，正式版本废弃*/
static uint16_t cmd_proxy_common_support_cmd[5] = {
    0x0106, //查询模型名称
    0x0108, //查询模型MD5
    0x0304, //更新模型—Step1：发送模型信息
    0x0306, //更新模型—Step2：传输模型数据
    0x0308  //更新模型—Step3：模型传输完成
};

open_protocol_header_t g_proxy_common_open_header[5];
uint8_t g_proxy_common_need_ack[5];

int get_proxy_common_idx(uint16_t ext_cmd)
{
    for(int i = 0; i < sizeof(cmd_proxy_common_support_cmd); i++)
    {
        if(ext_cmd == cmd_proxy_common_support_cmd[i] || ext_cmd == cmd_proxy_common_support_cmd[i] + 1)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief DJI V1代理转发通用
 *
 * @param buf
 * @param len
 */
void open_cmd_proxy_common(open_protocol_header_t *pack_desc)
{
    /* 包最小2个字节 */
    if(pack_desc->data_len < 2)
    {
        return;
    }

    uint16_t* cmd_id = (uint16_t*)(pack_desc->data);
    uint8_t* data =  (uint8_t*)(pack_desc->data + 2);

    int idx = get_proxy_common_idx(*cmd_id);
    uint16_t data_len = pack_desc->data_len - 2;

    if(idx >= 0 && data_len < EXT_PROTOCOL_DATA_MAX_BYTE)
    {
        memcpy(&(g_proxy_common_open_header[idx]), pack_desc, sizeof(open_protocol_header_t));
        g_proxy_common_need_ack[idx] = 1;

        static uint8_t ext_buff[EXT_PROTOCOL_PACK_MAX_BYTE];  //33bytes
        int ret = ext_pack(data, ext_buff, *cmd_id, data_len, sizeof(ext_buff));
        if(ret > 0)
        {
            uart_send(LPUART3, (char*)ext_buff, ret);
        }

        log_printf(g_log_proto, 0, LOG_DEBUG_L3, "Open protocol proxy v1:id:0x%04x, len:%d.", *cmd_id, data_len);
        if(*cmd_id == 0x0306)
        {
            uint32_t *pack_idx = (uint32_t*)(data + 1);
            log_printf(g_log_proto, 0, LOG_DEBUG_L3, "pack_idx:%d.", *pack_idx);
        }
    }
}
