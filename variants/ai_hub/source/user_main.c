/**
 * @file main.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-12-06
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <stdio.h>
#include <string.h>

#include "can.h"
#include "cmsis_os.h"
#include "log.h"
#include "user_main.h"
#include "open_protocol.h"
#include "open_protocol_cmd.h"
// #include "sd_file.h"
#include "sys_param.h"
#include "task_cdc_uvc.h"
#include "task_protocol.h"
#include "user_driver_uart.h"
#include "user_board.h"

/* Private define ------------------------------------------------------------*/
#define TASK_START_STACK_SIZE (256)   //初始化任务堆栈使用空间
#define TASK_LED_STACK_SIZE (256)     //初始化任务堆栈使用空间
#define TASK_USB_VCP_STACK_SIZE (512) // USB_VCP任务堆栈使用空间
#define TASK_PROTOCOL_ACK_SIZE (1024) //协议任务堆栈使用空间

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
sys_param_t g_sys_param; //系统参数
uint16_t g_sn_crc16 = 0;

uint8_t g_factory_led_test_flag = 0; //工厂LED测试标志位

log_module_t *g_log_sys;

/* 任务启动相关 */
osThreadId task_start_handle;    // 初始化任务Handle
osThreadId task_key_handle;      // LED和按键任务Handle
osThreadId task_cdc_uvc_handle;  // USB_VCP任务Handle
osThreadId task_protocol_handle; // 协议任务Handle

/* 空闲任务和Timer的Stack空间和任务块 */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

/* Private function prototypes -----------------------------------------------*/
static void task_start(void const *argument);
static void task_key(void const *argument);
static uint32_t debug_uart_output(uint8_t *buf, uint32_t len);
static void start_print(void);

/* Exported functions --------------------------------------------------------*/

int vendor_start(void)
{
    user_board_int();
    
    // board_wdog_init(2000);

    /* 初始化StartTask */
    osThreadDef(TaskStart, task_start, osPriorityNormal, 0, TASK_START_STACK_SIZE);
    task_start_handle = osThreadCreate(osThread(TaskStart), NULL);

    /* 在main.cpp中启动RTOS内核  */
}

/**
 * @brief 初始化任务
 *
 * @param argument
 */
static void task_start(void const *argument)
{
    flash_init();
    // sd_file_init();
    can3_init();
    uart_init(LPUART3, 1500000, 512, 512);
    uart_init(LPUART1, 921600, 512, 512);

    /* 初始化Log模块 */
    log_init(osKernelSysTick);
    g_log_sys = log_module_add("sys");
    log_output_object_add("USART0", debug_uart_output);
    log_set_output_pri(LOG_INFO);

    start_print();
    log_printf(g_log_sys, 0, LOG_INFO, "Logger initial complete.");
    
    // if(board_wdog_is_timeout_reset())
    // {
    //     log_printf(g_log_sys, 0, LOG_INFO, "Last system Reset by WDOG timeout.");
    // }

    /* 初始化系统参数*/
    if (SYS_PARAM_OK == sys_param_read(&g_sys_param))
    {
        if (g_sys_param.app_ver != APP_VERSION)
        {
            /* 系统的Loader版本不符进行设置 */
            g_sys_param.app_ver = APP_VERSION;
            sys_param_save(&g_sys_param);
        }
    }

    g_sys_param.id = g_sys_param.id > 4 ? 4 : g_sys_param.id;
    g_sys_param.id = g_sys_param.id == 0 ? 1 : g_sys_param.id;

    g_sn_crc16 = board_sn_crc16();

    /* 初始化LedKeyTask*/
    osThreadDef(KeyTask, task_key, osPriorityNormal, 0, TASK_LED_STACK_SIZE);
    task_key_handle = osThreadCreate(osThread(KeyTask), NULL);

    /* 初始化UsbVcpTask*/
    osThreadDef(UsbVcpTask, task_cdc_uvc, osPriorityNormal, 0, TASK_USB_VCP_STACK_SIZE);
    task_cdc_uvc_handle = osThreadCreate(osThread(UsbVcpTask), NULL);

    /* 初始化ProtocolTask*/
    osThreadDef(ProtocolTask, task_protocol, osPriorityNormal, 0, TASK_PROTOCOL_ACK_SIZE);
    task_cdc_uvc_handle = osThreadCreate(osThread(ProtocolTask), NULL);

    uint8_t led_mask = 0;

    switch(g_sys_param.id)
    {
        case 4: led_mask |= 0x8;
        case 3: led_mask |= 0x4;
        case 2: led_mask |= 0x2;
        case 1: led_mask |= 0x1;
    }

    for (;;)
    {
        // board_wdog_refresh();

        if(g_factory_led_test_flag)
        {
            /* 工厂LED测试模式 */
            if(board_key_read())
            {
                board_led_set(0x0F, 1);
                osDelay(250);
            }
            else
            {
                board_led_set(0x0F, 0);
                osDelay(250);
                board_led_set(0x0F, 1);
                osDelay(250);
            }
        }
        else
        {
            /* 正常模式 */
            board_led_togle(led_mask);
            osDelay(500);
        }
    }
}

/**
 * @brief 按键任务
 *
 * @param argument
 */
static void task_key(void const *argument)
{
    open_cmd_ai_core_spi_video_en_req_t en_req;
    en_req.enable = 1;
    for (;;)
    {
        if (!g_factory_led_test_flag)
        {
            if (board_key_read())
            {
                osDelay(50);
                while (board_key_read());
                osDelay(50);

                // //发送开启关闭视频流指令
                // open_proto_send(OPEN_AI_CORE_SPI_VIDEO_EN, 0x0301, 1, (uint8_t*)(&en_req), sizeof(en_req));
                // en_req.enable = 1 - en_req.enable;
            }
        }
        osDelay(50);
    }
}

/**
 * @brief FreeRTOS获取空闲任务Stack Memory
 *
 * @param ppxIdleTaskTCBBuffer
 * @param ppxIdleTaskStackBuffer
 * @param pulIdleTaskStackSize
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @brief FreeRTOS获取定时器任务Stack Memory
 *
 * @param ppxTimerTaskTCBBuffer
 * @param ppxTimerTaskStackBuffer
 * @param pulTimerTaskStackSize
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/* Private functions ---------------------------------------------------------*/

/**
 * @brief DEBUG串口输出
 *
 * @param buf
 * @param len
 */
static uint32_t debug_uart_output(uint8_t *buf, uint32_t len)
{
    uart_send(LPUART1, (char *)buf, len);
    return 0;
}

/**
 * @brief 启动打印
 *
 * @return uint32_t
 */
static void start_print(void)
{
#ifdef USE_DEBUG
    char *ver_str = "Debug";
#else
    char *ver_str = "Release";
#endif

    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "\r\n\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "==================================================================\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "       ____        __          __  ___           __\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "      / __ \\____  / /_  ____  /  |/  /___ ______/ /____  _____\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "     / /_/ / __ \\/ __ \\/ __ \\/ /|_/ / __ `/ ___/ __/ _ \\/ ___/\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "    / _, _/ /_/ / /_/ / /_/ / /  / / /_/ (__  ) /_/  __/ /\r\n");
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "   /_/ |_|\\____/_.___/\\____/_/  /_/\\__,_/____/\\__/\\___/_/\r\n\r\n");

    log_printf_noprefix(g_log_sys, 0, LOG_INFO, " Robomaster AI Module Comm Board   v%d.%d.%d.%d-%s\r\n",
                        (APP_VERSION >> 24 & 0xFF), (APP_VERSION >> 16 & 0xFF), (APP_VERSION >> 8 & 0xFF), APP_VERSION & 0xFF,
                        ver_str);
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, " Build Time:%s %s\r\n", __DATE__, __TIME__);
    log_printf_noprefix(g_log_sys, 0, LOG_INFO, "==================================================================\r\n");
}
