/**
 * @file task_protocol.c
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-07-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "task_protocol.h"
#include "task_cdc_uvc.h"
#include "open_protocol_cmd.h"
#include "ext_protocol_cmd.h"
#include "string.h"
#include "open_protocol.h"
#include "ext_protocol.h"
#include "cmsis_os.h"
#include "event_groups.h"
#include "FreeRTOS.h"
#include "board.h"
#include "user_driver_uart.h"
#include "can.h"
#include "user_main.h"
#include "crc.h"
#include "user_board.h"
#include "sys_param.h"

#include "log.h"

#include "string.h"
//#include "log.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    uint16_t             cmd_id;
    open_cmd_handler_t   handler;
}open_handler_keypair_t;

typedef struct
{
    uint16_t            cmd_id;
    cmd_handler_t       handler;
}ext_handler_keypair_t;

/* Private define ------------------------------------------------------------*/
#define APP_PROTOCOL_PORT_NUM   (sizeof(app_protocol_port) / sizeof(ST_APP_PROTOCOL_PORT))
#define RECV_BUFF_LEN     (512)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t proto_recv_buff[RECV_BUFF_LEN];

// Open Protocol 协议回调表
const open_handler_keypair_t open_protocol_fun_req[] =
{
    //{0xFFFE,                                open_cmd_default},
    {OPEN_CMD_REBOOT,                       open_cmd_reboot},
    {OPEN_CMD_VERSION,                      open_cmd_ver},
    {OPEN_CMD_ECHO,                         open_cmd_echo},
    {OPEN_CMD_SET_ID,                       open_cmd_set_id},
    {OPEN_CMD_UPGRADE_ENTER,                open_cmd_enter_loader},

    {OPEN_CMD_LED_TEST,                     open_cmd_led_test},
    {OPEN_CMD_SD_CARD_TEST,                 open_cmd_sd_card_test},
    {OPEN_CMD_CAN_TEST,                     open_cmd_can_test},
    {OPEN_CMD_SLOT14_TEST,                  open_cmd_slot14_test},
    {OPEN_CMD_SLOT100_TEST,                 open_cmd_slot100_test},
    {OPEN_CMD_EN_AI_BOARD_IMG,              open_cmd_en_ai_board_img},
    {OPEN_CMD_BURN_SN,                      open_cmd_burn_sn},

    {OPEN_CMD_V1_VER,                       open_cmd_v1_ver},

    {OPEN_CMD_OLD_PROTO_REQ_ACK_FOWARD,     open_cmd_proxy_common}
};

// Ext Protocol 协议回调表
const ext_handler_keypair_t ext_protocol_fun_req[] =
{
    {0xFFFF,                            ext_cmd_all},
    {0xFFFE,                            ext_cmd_default},

    {0x0107,                            ext_cmd_commmon_forward_handle},
    {0x0109,                            ext_cmd_commmon_forward_handle},
    {0x0305,                            ext_cmd_commmon_forward_handle},
    {0x0307,                            ext_cmd_commmon_forward_handle},
    {0x0309,                            ext_cmd_commmon_forward_handle},
};

log_module_t* g_log_proto;
static uint8_t usb_cdc_port_idx;
static uint8_t can_port_idx;
static uint8_t uart3_port_idx;
extern sys_param_t g_sys_param;                        //系统参数


/* Private function prototypes -----------------------------------------------*/
static void unpacket_process(uint8_t port, uint8_t *buf, uint16_t len);
static void uart3_send(uint8_t* data, uint16_t len);
static uint16_t uart3_receive(uint8_t* buff, uint16_t len);
static void set_ai_core_uart_speed(uint32_t uart_speed);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief 解包处理任务
 *
 * @param argument
 */
void task_protocol(void const * argument)
{
    // g_log_proto = log_module_add("proto");
    // log_printf(g_log_proto, 0, LOG_INFO, "Start task: protocol.");

    uint8_t last_usb_attach_status = 0;
    open_proto_init(g_sys_param.id | 0x0200);

    usb_cdc_port_idx = open_proto_port_add("USB_CDC", cdc_vcp_send, cdc_vcp_recv);
    can_port_idx = open_proto_port_add("CAN", can_send, can_receive);
    uart3_port_idx = open_proto_port_add("UART3", uart3_send, uart3_receive);
    open_proto_port_enable(usb_cdc_port_idx, 0);
    open_proto_static_route_add(0x0, 0x0, usb_cdc_port_idx, 252);
    open_proto_static_route_add(0x0, 0x0, can_port_idx, 253);
    open_proto_static_route_add(0x03FF, 0xFF00, uart3_port_idx, 2);

    for (int i = 0; i < sizeof(open_protocol_fun_req) / sizeof(open_handler_keypair_t); i++)
    {
        open_proto_handle_reg(open_protocol_fun_req[i].cmd_id, open_protocol_fun_req[i].handler);
    }

    for (int i = 0; i < sizeof(ext_protocol_fun_req) / sizeof(ext_handler_keypair_t); i++)
    {
        ext_handle_reg(ext_protocol_fun_req[i].cmd_id, ext_protocol_fun_req[i].handler);
    }

    uint32_t last_send_ai_core_set_speed_time = 0;
    for(;;)
    {
        open_proto_recv();

        if(last_usb_attach_status != cdc_vcp_is_attach() && cdc_vcp_is_attach())
        {
            last_usb_attach_status = cdc_vcp_is_attach();
            open_proto_port_enable(usb_cdc_port_idx, 1);
        }

        if(osKernelSysTick() - last_send_ai_core_set_speed_time > 250)
        {
            /* 定时发送设置波特率指令，维持AI核心板为高速串口模式 */
            last_send_ai_core_set_speed_time = osKernelSysTick();
            set_ai_core_uart_speed(1500000);
        }

        osDelay(2);
    }
}

static void set_ai_core_uart_speed(uint32_t uart_speed)
{
    open_ai_core_set_uart_speed_req_t req;
    req.baudrate = uart_speed;
    open_proto_send(OPEN_AI_CORE_SET_UART_SPEED_REQ, 0x03FF, 1, (uint8_t*)(&req), sizeof(req));
}

/**
 * @brief 串口3发送函数
 *
 * @param data
 * @param len
 */
static void uart3_send(uint8_t* data, uint16_t len)
{
    uart_send(LPUART3, (char*)(data), len);
}

/**
 * @brief 串口3接受
 *
 * @param data
 * @param len
 */
static uint16_t uart3_receive(uint8_t* buff, uint16_t len)
{
    return uart_receive(LPUART3, (char*)(buff), len);
}
