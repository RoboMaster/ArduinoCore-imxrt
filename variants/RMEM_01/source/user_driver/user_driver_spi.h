/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "board.h"
#include "user_board.h"

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef void(*spi_rx_handler_t)(void);  /*!< 解包回调函数  */
/* Exported function prototypes ----------------------------------------------*/

void spi3_slave_init(void);
int spi3_slave_start_receive(void* buff, uint32_t size);
int spi3_slave_cancel_receive(void);
int spi3_slave_in_receive(void);
void spi3_slave_receive_finish_handle_reg(spi_rx_handler_t handle);

#endif
