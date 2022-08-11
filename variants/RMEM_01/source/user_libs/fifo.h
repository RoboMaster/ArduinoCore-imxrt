/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#ifndef __FIFO_H__
#define __FIFO_H__

#include "stdint.h"

typedef struct
{
    char* p_start_addr;                  //!< FIFO Memory Pool Start Address
    char* p_end_addr;                    //!< FIFO Memory Pool End Address
    int   free_num;                      //!< The remain capacity of FIFO
    int   used_num;                      //!< The number of elements in FIFO
    char* p_read_addr;                   //!< FIFO Data Read Index Pointer
    char* p_write_addr;                  //!< FIFO Data Write Index Pointer
}fifo_t;

int fifo_init(fifo_t* p_fifo, void* p_base_addr, int uint_cnt);
int fifo_puts(fifo_t *p_fifo, char *p_source, int len);
int fifo_gets(fifo_t* p_fifo, char* p_dest, int len);
int fifo_prereads(fifo_t* p_fifo, char *p_dest, int offset, int len);
int fifo_used(fifo_t* p_fifo);
int fifo_free(fifo_t* p_fifo);

#endif //__FIFO_H__
