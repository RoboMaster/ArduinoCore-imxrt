/*
 * task_cdc_uvc.c
 *
 *  Created on: 2021年12月3日
 *      Author: ryan.pan
 */

#include "board.h"
#include "clock_config.h"
#include "fifo.h"
#include "pin_mux.h"
#include "user_board.h"
#include "user_driver_spi.h"
#include "composite.h"
#include "cr_section_macros.h"
#include "log.h"
#include "task_cdc_uvc.h"
#include "usb_device.h"
#include "virtual_com.h"

#include "cmsis_os.h"

#include "open_protocol_cmd.h"

/* Private define ------------------------------------------------------------*/
#define USB_VCP_TX_FIFO_SIZE 1024
#define USB_VCP_RX_FIFO_SIZE 1024

#define IMAGE_BUFF_NUM  (2)
#define IMAGE_BUFF_SIZE (320 * 240 * 2)

typedef enum
{
    IMAGE_BUFF_IDLE,
    IMAGE_BUFF_SPI_RECVING,
    IMAGE_BUFF_YUYV2NV12,
    IMAGE_BUFF_WAIT_SEND,
    IMAGE_BUFF_UVC_SENDING,
} image_buff_sta_e;

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern log_module_t *g_log_sys;

/* Private variables ---------------------------------------------------------*/
AT_NONCACHEABLE_SECTION_INIT(static fifo_t usb_vcp_tx_fifo);
AT_NONCACHEABLE_SECTION_INIT(static fifo_t usb_vcp_rx_fifo);
AT_NONCACHEABLE_SECTION_INIT(static char usb_vcp_tx_fifo_buff[USB_VCP_TX_FIFO_SIZE]);
AT_NONCACHEABLE_SECTION_INIT(static char usb_vcp_rx_fifo_buff[USB_VCP_RX_FIFO_SIZE]);

uint8_t usb_cdc_is_recv = 0;

__attribute__((section(".m_data3"))) static uint8_t image_buff[IMAGE_BUFF_NUM][IMAGE_BUFF_SIZE];

static image_buff_sta_e image_buff_status[IMAGE_BUFF_NUM];

extern usb_device_composite_struct_t g_composite;
extern USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
extern USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currSendBuf[DATA_BUFF_SIZE];
extern volatile uint32_t s_recvSize;
extern volatile uint32_t s_sendSize;

/* Private function prototypes -----------------------------------------------*/
static void spi_cs_falling(void);
static void spi_rx_complete(void);
static uint8_t *usb_video_req_data(void);
static void yuyv2nv12(uint8_t *buff);
/* Exported functions --------------------------------------------------------*/

/**
 * @brief USB VCP任务
 *
 * @param argument
 */
void task_cdc_uvc(void *argument)
{
    log_printf(g_log_sys, 0, LOG_INFO, "Start task: usb_vcp.");

    fifo_init(&usb_vcp_tx_fifo, usb_vcp_tx_fifo_buff, USB_VCP_TX_FIFO_SIZE);
    fifo_init(&usb_vcp_rx_fifo, usb_vcp_rx_fifo_buff, USB_VCP_RX_FIFO_SIZE);

#ifdef USE_AICAMERA
    spi3_slave_init();
    spi3_slave_receive_finish_handle_reg(spi_rx_complete);
    board_gpio_spi3_cs0_int_reg(spi_cs_falling);
    board_gpio_spi3_cs0_int_enable(1);
#endif
    usb_video_data_req_handle_reg(usb_video_req_data);
    usb_video_set_source(UVC_SOURCE_INPUT);

    char sn[16];
    board_sn_read((uint8_t *)(sn));
    usb_set_serial_num(sn);

    USB_DeviceApplicationInit();

    uint8_t need_send = 0;
    uint32_t last_spi_recv_time = osKernelSysTick();
    uint8_t cam_opened = 0;
    uint8_t last_cam_opened = 0;

    for (;;)
    {
        if ((1 == g_composite.cdcVcom.attach) && (1 == g_composite.cdcVcom.startTransactions))
        {
            /* User Code */
            /* endpoint callback length is USB_CANCELLED_TRANSFER_LENGTH (0xFFFFFFFFU) when transfer is canceled */
            if ((0 != s_recvSize) && (USB_CANCELLED_TRANSFER_LENGTH != s_recvSize))
            {
                fifo_puts(&usb_vcp_rx_fifo, s_currRecvBuf, s_recvSize);
                s_recvSize = 0;
                need_send = 1;
                if (!usb_cdc_is_recv)
                {
                    usb_cdc_is_recv = 1;
                }
            }

            if (fifo_used(&usb_vcp_tx_fifo) > 0)
            {
                need_send = 0;
                s_sendSize = fifo_gets(&usb_vcp_tx_fifo, s_currSendBuf, DATA_BUFF_SIZE);

                int error = USB_DeviceCdcAcmSend(g_composite.cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, s_currSendBuf,
                                                 s_sendSize);
                if (error != kStatus_USB_Success)
                {
                    /* Failure to send Data Handling code here */
                }
            }

            if (need_send)
            {
                USB_DeviceCdcAcmSend(g_composite.cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT, s_currSendBuf, 0);
            }
        }

        for (int i = 0; i < IMAGE_BUFF_NUM; i++)
        {
            CPU_SR_T cpu_sr = ENTER_CRITICAL();

            if (image_buff_status[i] == IMAGE_BUFF_YUYV2NV12)
            {
                EXIT_CRITICAL(cpu_sr);
                last_spi_recv_time = osKernelSysTick();
                yuyv2nv12(image_buff[i]);

                CPU_SR_T cpu_sr = ENTER_CRITICAL();
                image_buff_status[i] = IMAGE_BUFF_WAIT_SEND;
            }

            EXIT_CRITICAL(cpu_sr);
        }

        CPU_SR_T cpu_sr = ENTER_CRITICAL();
        for (int i = 0; i < IMAGE_BUFF_NUM; i++)
        {
            if (image_buff_status[i] == IMAGE_BUFF_WAIT_SEND)
            {
                last_spi_recv_time = osKernelSysTick();
                break;
            }
        }
        EXIT_CRITICAL(cpu_sr);

        /* 判断相机打开状态 */
        last_cam_opened = cam_opened;
        if (osKernelSysTick() - usb_video_get_last_req_timer() < 500 && usb_video_get_last_req_timer() != 0)
        {
            cam_opened = 1;
        }
        else
        {
            cam_opened = 0;
        }

        if (osKernelSysTick() - last_spi_recv_time > 500)
        {
            usb_video_set_source(UVC_SOURCE_GRAYSCREEN);

            /* 相机打开同时触发相机打开 */
            if (cam_opened == 1 && last_cam_opened != cam_opened)
            {
                open_cmd_ai_core_spi_video_en_req_t en_req;
                en_req.enable = 1;
                //发送开启关闭视频流指令
                open_proto_send(OPEN_AI_CORE_SPI_VIDEO_EN, 0x03FF, 1, (uint8_t *)(&en_req), sizeof(en_req));
            }
        }
        else
        {
            usb_video_set_source(UVC_SOURCE_INPUT);
        }

        osDelay(2);
    }
}

/**
 * @brief CDC发送
 *
 * @param data
 * @param size
 * @param timeout_ms
 * @return int
 */
void cdc_vcp_send(uint8_t *data, uint16_t size)
{
    uint32_t reg;
    if (__get_IPSR() != 0)
    {
        reg = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else
    {
        taskENTER_CRITICAL();
    }

    if (size <= fifo_free(&usb_vcp_tx_fifo))
    {
        fifo_puts(&usb_vcp_tx_fifo, (char *)data, size);
    }

    if (__get_IPSR() != 0)
    {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(reg);
    }
    else
    {
        taskEXIT_CRITICAL();
    }
}

/**
 * @brief CDC接受
 *
 * @param buff
 * @param buff_size
 * @param timeout_ms
 * @return int
 */
uint16_t cdc_vcp_recv(uint8_t *buff, uint16_t buff_size)
{
    uint32_t reg;
    uint16_t ret;
    if (__get_IPSR() != 0)
    {
        reg = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else
    {
        taskENTER_CRITICAL();
    }

    ret = fifo_gets(&usb_vcp_rx_fifo, (char *)buff, buff_size);

    if (__get_IPSR() != 0)
    {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(reg);
    }
    else
    {
        taskEXIT_CRITICAL();
    }
    return ret;
}

/**
 * @brief 判断USB是否被插入
 *
 * @return uint8_t
 */
uint8_t cdc_vcp_is_attach(void)
{
    // return g_composite.attach;
    return usb_cdc_is_recv;
}

/**
 * @brief SPI传输完成中断
 *
 */
static void spi_rx_complete(void)
{
    CPU_SR_T cpu_sr = ENTER_CRITICAL();

    for (int i = 0; i < IMAGE_BUFF_NUM; i++)
    {
        if (image_buff_status[i] == IMAGE_BUFF_SPI_RECVING)
        {
            image_buff_status[i] = IMAGE_BUFF_YUYV2NV12;
        }
    }

    EXIT_CRITICAL(cpu_sr);
}

/**
 * @brief SPI_CS中断
 *
 */
static void spi_cs_falling(void)
{
    spi3_slave_cancel_receive();

    CPU_SR_T cpu_sr = ENTER_CRITICAL();

    for (int i = 0; i < IMAGE_BUFF_NUM; i++)
    {
        if (image_buff_status[i] == IMAGE_BUFF_SPI_RECVING)
        {
            image_buff_status[i] = IMAGE_BUFF_IDLE;
        }
    }

    for (int i = 0; i < IMAGE_BUFF_NUM; i++)
    {
        if (image_buff_status[i] == IMAGE_BUFF_IDLE)
        {
            spi3_slave_start_receive(image_buff[i], IMAGE_BUFF_SIZE);
            image_buff_status[i] = IMAGE_BUFF_SPI_RECVING;
            break;
        }
    }

    EXIT_CRITICAL(cpu_sr);

    return;
}

/**
 * @brief USB获取下一帧数据回调
 *
 */
static uint8_t *usb_video_req_data(void)
{
    CPU_SR_T cpu_sr = ENTER_CRITICAL();

    for (int i = 0; i < IMAGE_BUFF_NUM; i++)
    {
        if (image_buff_status[i] == IMAGE_BUFF_UVC_SENDING)
        {
            image_buff_status[i] = IMAGE_BUFF_IDLE;
        }
    }

    for (int i = 0; i < IMAGE_BUFF_NUM; i++)
    {
        if (image_buff_status[i] == IMAGE_BUFF_WAIT_SEND)
        {
            image_buff_status[i] = IMAGE_BUFF_UVC_SENDING;
            EXIT_CRITICAL(cpu_sr);
            return image_buff[i];
        }
    }

    EXIT_CRITICAL(cpu_sr);
    return NULL;
}

/**
 * @brief YUYV转NV12
 *
 */
static void yuyv2nv12(uint8_t *buff)
{
    const int w = 320;
    const int h = 240;

    __attribute__((section(".m_data3"))) static uint8_t temp_buff[320 * 240 * 3 / 2];

    uint8_t *yuyv_ptr = buff;
    uint8_t *nv12_y_ptr = temp_buff;
    uint8_t *nv12_uv_ptr = temp_buff + w * h;
    uint16_t u_temp = 0;
    uint16_t v_temp = 0;

    SCB_DisableDCache();
    for (int i = 0; i < w * h / 2; i++)
    {
        if ((i / (w / 2)) % 2 == 0)
        {
            *(nv12_y_ptr++) = *(yuyv_ptr++);
            *(nv12_uv_ptr++) = *(yuyv_ptr++);
            *(nv12_y_ptr++) = *(yuyv_ptr++);
            *(nv12_uv_ptr++) = *(yuyv_ptr++);
        }
        else
        {
            *(nv12_y_ptr++) = *(yuyv_ptr++);
            *(nv12_uv_ptr - w / 2) = (*(yuyv_ptr++) + *(nv12_uv_ptr - w / 2)) / 2;
            *(nv12_y_ptr++) = *(yuyv_ptr++);
            *(nv12_uv_ptr - w / 2) = (*(yuyv_ptr++) + *(nv12_uv_ptr - w / 2)) / 2;
        }
    }
    SCB_EnableDCache();

    memcpy(buff, temp_buff, 320 * 240 * 3 / 2);
}
