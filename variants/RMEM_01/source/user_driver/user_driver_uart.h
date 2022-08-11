/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef USER_DRIVER_UART_H_
#define USER_DRIVER_UART_H_

#include "fsl_lpuart_edma.h"
#include "fsl_lpuart.h"
#include "fsl_edma.h"
#include "fifo.h"

#define USART1_DMA_TX_BUFFER_SIZE       (256)
#define USART1_DMA_RX_BUFFER_SIZE       (256)

#define USART3_DMA_TX_BUFFER_SIZE       (256)
#define USART3_DMA_RX_BUFFER_SIZE       (256)

typedef uint32_t (*uart_rx_handler_t)(char* data, uint32_t num);  /*!< 解包回调函数  */

typedef struct
{
	LPUART_Type*         	usart_periph;
    char*               	dma_tx_buff;
    uint32_t            	dma_tx_buff_size;
    char*               	dma_rx_buff;
    uint32_t            	dma_rx_buff_size;
    lpuart_edma_handle_t*   edma_periph;

    uart_rx_handler_t   rx_handler;
    uint8_t				tcd_flag;

    fifo_t   tx_fifo;
    fifo_t   rx_fifo;
    uint8_t  is_sending;
    uint32_t recv_idle_cnt;
}uart_dev_t;


void uart_init(LPUART_Type* uart_periph, uint32_t baudrate, uint32_t tx_fifo_size, uint32_t rx_fifo_size);
void uart_send(LPUART_Type* uart_periph, char* data, uint16_t len);
int uart_receive(LPUART_Type* uart_periph, char* data, uint16_t len);
void uart_rx_handler_reg(LPUART_Type* uart_periph, uart_rx_handler_t handler);

void uart_dma_tx_finish_interrupt(LPUART_Type* uart_periph);
void uart_dma_rx_half_finish_interrupt(LPUART_Type* uart_periph);
void uart_dma_rx_finish_interrupt(LPUART_Type* uart_periph);
void uart_idle_interrupt(LPUART_Type* uart_periph);

#endif /* USER_DRIVER_UART_H_ */
