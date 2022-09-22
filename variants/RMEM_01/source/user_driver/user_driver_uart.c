/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "user_driver_uart.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"


#define ASSERT(e)   configASSERT(e)

AT_NONCACHEABLE_SECTION_INIT(static char usart1_dma_tx_buff[USART1_DMA_TX_BUFFER_SIZE]);
AT_NONCACHEABLE_SECTION_INIT(static char usart1_dma_rx_buff[USART1_DMA_RX_BUFFER_SIZE]);
AT_NONCACHEABLE_SECTION_ALIGN(static edma_tcd_t usart1_tcdMemoryPoolPtr[2], sizeof(edma_tcd_t));

AT_NONCACHEABLE_SECTION_INIT(static char usart3_dma_tx_buff[USART3_DMA_TX_BUFFER_SIZE]);
AT_NONCACHEABLE_SECTION_INIT(static char usart3_dma_rx_buff[USART3_DMA_RX_BUFFER_SIZE]);
AT_NONCACHEABLE_SECTION_ALIGN(static edma_tcd_t usart3_tcdMemoryPoolPtr[2], sizeof(edma_tcd_t));

AT_NONCACHEABLE_SECTION_INIT(uart_dev_t uart_dev[]) =
{
    {LPUART1, usart1_dma_tx_buff, USART1_DMA_TX_BUFFER_SIZE,  usart1_dma_rx_buff, USART1_DMA_RX_BUFFER_SIZE, &LPUART1_LPUART_eDMA_Handle},
    {LPUART3, usart3_dma_tx_buff, USART3_DMA_TX_BUFFER_SIZE,  usart3_dma_rx_buff, USART3_DMA_RX_BUFFER_SIZE, &LPUART3_LPUART_eDMA_Handle},
};


static void uart1_init(uint32_t baudrate);
static void uart1_rx_edma_callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
static void uart1_tx_edma_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);
static void uart3_init(uint32_t baudrate);
static void uart3_rx_edma_callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
static void uart3_tx_edma_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);


/**
 * @brief UART1 初始化
 *
 */
static void uart1_init(uint32_t baudrate)
{
    edma_transfer_config_t xferConfig_0;
    edma_transfer_config_t xferConfig_1;

    LPUART_SetBaudRate(LPUART1, baudrate, LPUART1_CLOCK_SOURCE);

    /* Install TCD memory for using only one TCD queue. */
    EDMA_InstallTCDMemory(&LPUART1_RX_Handle, (edma_tcd_t *)&usart1_tcdMemoryPoolPtr[0], 1U);

    /* Prepare transfer to receive data to ring buffer. */
    EDMA_PrepareTransfer(&xferConfig_0, (void *)(uint32_t *)LPUART_GetDataRegisterAddress(LPUART1),
                         sizeof(uint8_t), usart1_dma_rx_buff, sizeof(uint8_t), sizeof(uint8_t), USART1_DMA_RX_BUFFER_SIZE / 2,
                         kEDMA_PeripheralToMemory);
    EDMA_PrepareTransfer(&xferConfig_1, (void *)(uint32_t *)LPUART_GetDataRegisterAddress(LPUART1),
                         sizeof(uint8_t), usart1_dma_rx_buff + USART1_DMA_RX_BUFFER_SIZE / 2, sizeof(uint8_t), sizeof(uint8_t), USART1_DMA_RX_BUFFER_SIZE / 2,
                         kEDMA_PeripheralToMemory);

    /* Submit transfer. */
    LPUART1_RX_Handle.tcdUsed = 2U;
    LPUART1_RX_Handle.tail    = 1U;
    EDMA_TcdReset(&LPUART1_RX_Handle.tcdPool[0U]);
    EDMA_TcdReset(&LPUART1_RX_Handle.tcdPool[1U]);
    EDMA_TcdSetTransferConfig(&LPUART1_RX_Handle.tcdPool[0U], &xferConfig_0, &LPUART1_RX_Handle.tcdPool[1U]);
    EDMA_TcdSetTransferConfig(&LPUART1_RX_Handle.tcdPool[1U], &xferConfig_1, usart1_tcdMemoryPoolPtr);

    /* Enable major interrupt for counting received bytes. */
    LPUART1_RX_Handle.tcdPool[0U].CSR |= DMA_CSR_INTMAJOR_MASK;
    LPUART1_RX_Handle.tcdPool[1U].CSR |= DMA_CSR_INTMAJOR_MASK;

    /* There is no live chain, TCD block need to be installed in TCD registers. */
    EDMA_InstallTCD(LPUART1_RX_Handle.base, LPUART1_RX_Handle.channel, &LPUART1_RX_Handle.tcdPool[0U]);

    /* Setup call back function. */
    EDMA_SetCallback(&LPUART1_RX_Handle, uart1_rx_edma_callback, NULL);
    LPUART1_LPUART_eDMA_Handle.callback = uart1_tx_edma_callback;
    //EDMA_SetCallback(&LPUART1_TX_Handle, uart1_tx_edma_callback, NULL);

    /* Start EDMA transfer. */
    EDMA_StartTransfer(&LPUART1_RX_Handle);

    /* Enable LPUART RX EDMA. */
    LPUART_EnableRxDMA(LPUART1, true);

    LPUART_EnableInterrupts(LPUART1, kLPUART_IdleLineInterruptEnable);
    EnableIRQ(LPUART1_IRQn);
}

/**
 * @brief LPUART RX EDMA call back.
 *
 * @param handle
 * @param param
 * @param transferDone
 * @param tcds
 */
static void uart1_rx_edma_callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
	if(tcds == 0)
	{
		uart_dma_rx_half_finish_interrupt(LPUART1);
	}
	else
	{
		uart_dma_rx_finish_interrupt(LPUART1);
	}
}

/**
 * @brief LPUART EDMA TX user callback
 *
 * @param base
 * @param handle
 * @param status
 * @param userData
 */
static void uart1_tx_edma_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    uart_dma_tx_finish_interrupt(LPUART1);
}

extern void LPUART1_DriverIRQHandler(void);

/**
 * @brief LPUART1中断处理函数
 *
 */
void LPUART1_IRQHandler(void)
{
    if(LPUART_GetStatusFlags(LPUART1) & kLPUART_IdleLineFlag)
    {
        LPUART_ClearStatusFlags(LPUART1, kLPUART_IdleLineFlag);
        uart_idle_interrupt(LPUART1);
        __DSB();
    }
    else
    {
        LPUART1_DriverIRQHandler();
    }
}

/**
 * @brief UART3 初始化
 *
 */
static void uart3_init(uint32_t baudrate)
{
    edma_transfer_config_t xferConfig_0;
    edma_transfer_config_t xferConfig_1;

    LPUART_SetBaudRate(LPUART3, baudrate, LPUART3_CLOCK_SOURCE);

    /* Install TCD memory for using only one TCD queue. */
    EDMA_InstallTCDMemory(&LPUART3_RX_Handle, (edma_tcd_t *)&usart3_tcdMemoryPoolPtr[0], 1U);

    /* Prepare transfer to receive data to ring buffer. */
    EDMA_PrepareTransfer(&xferConfig_0, (void *)(uint32_t *)LPUART_GetDataRegisterAddress(LPUART3),
                         sizeof(uint8_t), usart3_dma_rx_buff, sizeof(uint8_t), sizeof(uint8_t), USART3_DMA_RX_BUFFER_SIZE / 2,
                         kEDMA_PeripheralToMemory);
    EDMA_PrepareTransfer(&xferConfig_1, (void *)(uint32_t *)LPUART_GetDataRegisterAddress(LPUART3),
                             sizeof(uint8_t), usart3_dma_rx_buff + USART3_DMA_RX_BUFFER_SIZE / 2, sizeof(uint8_t), sizeof(uint8_t), USART3_DMA_RX_BUFFER_SIZE / 2,
                             kEDMA_PeripheralToMemory);

    /* Submit transfer. */
    LPUART3_RX_Handle.tcdUsed = 2U;
    LPUART3_RX_Handle.tail    = 1U;
    EDMA_TcdReset(&LPUART3_RX_Handle.tcdPool[0U]);
    EDMA_TcdReset(&LPUART3_RX_Handle.tcdPool[1U]);
    EDMA_TcdSetTransferConfig(&LPUART3_RX_Handle.tcdPool[0U], &xferConfig_0, &LPUART3_RX_Handle.tcdPool[1U]);
    EDMA_TcdSetTransferConfig(&LPUART3_RX_Handle.tcdPool[1U], &xferConfig_1, usart3_tcdMemoryPoolPtr);

    /* Enable major interrupt for counting received bytes. */
    LPUART3_RX_Handle.tcdPool[0U].CSR |= DMA_CSR_INTMAJOR_MASK;
    LPUART3_RX_Handle.tcdPool[1U].CSR |= DMA_CSR_INTMAJOR_MASK;

    /* There is no live chain, TCD block need to be installed in TCD registers. */
    EDMA_InstallTCD(LPUART3_RX_Handle.base, LPUART3_RX_Handle.channel, &LPUART3_RX_Handle.tcdPool[0U]);

    /* Setup call back function. */
    EDMA_SetCallback(&LPUART3_RX_Handle, uart3_rx_edma_callback, NULL);
    LPUART3_LPUART_eDMA_Handle.callback = uart3_tx_edma_callback;
    //EDMA_SetCallback(&LPUART3_TX_Handle, uart3_tx_edma_callback, NULL);

    /* Start EDMA transfer. */
    EDMA_StartTransfer(&LPUART3_RX_Handle);


    /* Enable LPUART RX EDMA. */
    LPUART_EnableRxDMA(LPUART3, true);

    LPUART_EnableInterrupts(LPUART3, kLPUART_IdleLineInterruptEnable);
    EnableIRQ(LPUART3_IRQn);

}

/**
 * @brief LPUART RX EDMA call back.
 *
 * @param handle
 * @param param
 * @param transferDone
 * @param tcds
 */
static void uart3_rx_edma_callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if(tcds == 0)
    {
        uart_dma_rx_half_finish_interrupt(LPUART3);
    }
    else
    {
        uart_dma_rx_finish_interrupt(LPUART3);
    }
}

/**
 * @brief LPUART EDMA TX user callback
 *
 * @param base
 * @param handle
 * @param status
 * @param userData
 */
static void uart3_tx_edma_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    uart_dma_tx_finish_interrupt(LPUART3);
}

extern void LPUART3_DriverIRQHandler(void);

#if defined(CONNECT_AI_CAMERA)
/**
 * @brief LPUART3中断处理函数
 *
 */
void LPUART3_IRQHandler(void)
{
    if(LPUART_GetStatusFlags(LPUART3) & kLPUART_IdleLineFlag)
    {
        LPUART_ClearStatusFlags(LPUART3, kLPUART_IdleLineFlag);
        uart_idle_interrupt(LPUART3);
        __DSB();
    }
    else
    {
        LPUART3_DriverIRQHandler();
    }
}
#endif

/**
 * @brief 获取UART设备结构体
 *
 * @param uart_periph
 * @return uart_dev_t*
 */
static uart_dev_t* uart_get_obj(LPUART_Type*  uart_periph)
{
    switch((uint32_t)uart_periph)
    {
        case (uint32_t)(LPUART1):
            return uart_dev + 0;
        case (uint32_t)(LPUART3):
            return uart_dev + 1;
        default:
            return uart_dev + 0;
    }
}

static void uart_receive_to_fifo(uart_dev_t* obj, char* data, uint16_t len)
{
    uint32_t ret = 0;
    if(obj->rx_handler != NULL)
    {
        ret = (obj->rx_handler)(data, len);
        ret = ret > len ? len : ret;
    }
    fifo_puts(&(obj->rx_fifo), data + ret, len - ret);
}


/**
 * @brief UART设备初始化
 *
 * @param uart_periph
 * @param baudrate
 * @param tx_fifo_size
 * @param rx_fifo_size
 */
void uart_init(LPUART_Type* uart_periph, uint32_t baudrate, uint32_t tx_fifo_size, uint32_t rx_fifo_size)
{
    uart_dev_t *obj;
    switch((uint32_t)uart_periph)
    {
        case (uint32_t)(LPUART1):
            uart1_init(baudrate);
            obj = uart_dev + 0;
            break;
        case (uint32_t)(LPUART3):
            uart3_init(baudrate);
            obj = uart_dev + 1;
            break;
        default:
            return;
    }

    char* tx_fifo_buff = pvPortMalloc(tx_fifo_size);
    char* rx_fifo_buff = pvPortMalloc(rx_fifo_size);

    ASSERT(tx_fifo_buff != NULL);
    ASSERT(rx_fifo_buff != NULL);

    fifo_init(&(obj->tx_fifo), tx_fifo_buff, tx_fifo_size);
    fifo_init(&(obj->rx_fifo), rx_fifo_buff, rx_fifo_size);

    obj->tcd_flag = 0;
}


/**
 * @brief UART发送数据
 *
 * @param uart_periph
 * @param data
 * @param len
 */
void uart_send(LPUART_Type* uart_periph, char* data, uint16_t len)
{
    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();
    lpuart_transfer_t sendXfer;
    uart_dev_t* obj = uart_get_obj(uart_periph);

    if(obj->is_sending)
    {
        fifo_puts(&(obj->tx_fifo), data, len);
    }
    else
    {
        memset(&sendXfer, 0, sizeof(sendXfer));

        if(len > obj->dma_tx_buff_size)
        {
            memcpy(obj->dma_tx_buff, data, obj->dma_tx_buff_size);

            sendXfer.data = obj->dma_tx_buff;
            sendXfer.dataSize =  obj->dma_tx_buff_size;
            fifo_puts(&(obj->tx_fifo), data + obj->dma_tx_buff_size,  len - obj->dma_tx_buff_size);
        }
        else
        {
            memcpy(obj->dma_tx_buff, data, len);
            sendXfer.data = obj->dma_tx_buff;
            sendXfer.dataSize =  len;
        }
        obj->is_sending = 1;
        LPUART_SendEDMA(obj->usart_periph, obj->edma_periph, &sendXfer);
    }

    __set_PRIMASK(cpu_sr);
}


/**
 * @brief UART接收数据
 *
 * @param uart_periph
 * @param data
 * @param len
 * @return int
 */
int uart_receive(LPUART_Type* uart_periph, char* data, uint16_t len)
{

    uart_dev_t* obj = uart_get_obj(uart_periph);
    int32_t ret;

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    ret = fifo_gets(&(obj->rx_fifo), data, len);

    __set_PRIMASK(cpu_sr);

    return ret;
}


/**
 * @brief 注册接收回调函数
 *
 * @param uart_periph
 * @param handler
 */
void uart_rx_handler_reg(LPUART_Type* uart_periph, uart_rx_handler_t handler)
{
    uart_dev_t* obj = uart_get_obj(uart_periph);
    obj->rx_handler = handler;
}

/**
 * @brief UART DMA TX发送完成中断
 *
 * @param uart_periph
 */
void uart_dma_tx_finish_interrupt(LPUART_Type* uart_periph)
{
    uart_dev_t* obj = uart_get_obj(uart_periph);
    lpuart_transfer_t sendXfer;

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    int len = fifo_gets(&(obj->tx_fifo), obj->dma_tx_buff, obj->dma_tx_buff_size);
    if(len > 0)
    {
        obj->is_sending = 1;

        memset(&sendXfer, 0, sizeof(sendXfer));
        sendXfer.data = obj->dma_tx_buff;
        sendXfer.dataSize =  len;
        LPUART_SendEDMA(obj->usart_periph, obj->edma_periph, &sendXfer);
    }
    else
    {
        obj->is_sending = 0;
    }

    __set_PRIMASK(cpu_sr);
}


/**
 * @brief UART DMA RX半完成终端
 *
 * @param uart_periph
 */
void uart_dma_rx_half_finish_interrupt(LPUART_Type* uart_periph)
{
    uart_dev_t* obj = uart_get_obj(uart_periph);

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    obj->tcd_flag = 1;

    if(obj->recv_idle_cnt >= obj->dma_rx_buff_size / 2)
    {
        obj->recv_idle_cnt = 0;
    }
    char* recv_addr = obj->dma_rx_buff + obj->recv_idle_cnt;
    uint16_t recv_nbr = obj->dma_rx_buff_size / 2 - obj->recv_idle_cnt;

    uart_receive_to_fifo(obj, recv_addr, recv_nbr);
    obj->recv_idle_cnt = 0;
    __set_PRIMASK(cpu_sr);
}


/**
 * @brief UART DMA RX完成中断
 *
 * @param uart_periph
 */
void uart_dma_rx_finish_interrupt(LPUART_Type*  uart_periph)
{
    uart_dev_t* obj = uart_get_obj(uart_periph);

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    obj->tcd_flag = 0;

    if(obj->recv_idle_cnt >= obj->dma_rx_buff_size / 2)
    {
        obj->recv_idle_cnt = 0;
    }
    char* recv_addr = obj->dma_rx_buff + obj->dma_rx_buff_size / 2 + obj->recv_idle_cnt;
    uint16_t recv_nbr = obj->dma_rx_buff_size / 2 - obj->recv_idle_cnt;
    uart_receive_to_fifo(obj, recv_addr, recv_nbr);
    obj->recv_idle_cnt = 0;

    __set_PRIMASK(cpu_sr);
}


/**
 * @brief UART接收空闲中断
 *
 * @param uart_periph
 */
void uart_idle_interrupt(LPUART_Type*  uart_periph)
{
    uart_dev_t* obj = uart_get_obj(uart_periph);

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();


    uint32_t dtr;
    uint32_t ndtr;
    char* recv_addr;
    uint16_t recv_nbr;

    ndtr = EDMA_GetRemainingMajorLoopCount(obj->edma_periph->rxEdmaHandle->base, obj->edma_periph->rxEdmaHandle->channel);
    if(obj->tcd_flag == 0)
    {
        ndtr += obj->dma_rx_buff_size / 2;
    }


    dtr = obj->dma_rx_buff_size - ndtr;

    if(obj->recv_idle_cnt >= obj->dma_rx_buff_size / 2)
    {
        obj->recv_idle_cnt = 0;

        __set_PRIMASK(cpu_sr);

        return;
    }

    if(ndtr > obj->dma_rx_buff_size / 2)
    {
        recv_addr = obj->dma_rx_buff + obj->recv_idle_cnt;
        recv_nbr = dtr - obj->recv_idle_cnt;
        obj->recv_idle_cnt = dtr ;
    }else
    {
        recv_addr = obj->dma_rx_buff + obj->dma_rx_buff_size / 2 + obj->recv_idle_cnt;
        recv_nbr = dtr - obj->dma_rx_buff_size / 2 - obj->recv_idle_cnt;
        obj->recv_idle_cnt = dtr - obj->dma_rx_buff_size / 2;
    }

    uart_receive_to_fifo(obj, recv_addr, recv_nbr);

    __set_PRIMASK(cpu_sr);
}
