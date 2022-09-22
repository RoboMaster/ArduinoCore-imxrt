/**
 * @file fifo.c
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-07-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <user_libs/fifo.h>
#include "string.h"


int fifo_init(fifo_t* p_fifo, void* p_base_addr, int uint_cnt)
{
    p_fifo->p_start_addr    = (char*) p_base_addr;
    p_fifo->p_end_addr      = (char*) p_base_addr + uint_cnt - 1;
    p_fifo->free_num        = uint_cnt;
    p_fifo->used_num        = 0;
    p_fifo->p_read_addr     = (char*) p_base_addr;
    p_fifo->p_write_addr    = (char*) p_base_addr;
    return 0;
}


int fifo_puts(fifo_t *p_fifo, char *p_source, int len)
{
    int retval;
    int len_to_end;
    int len_from_start;

    if (NULL == p_source)
        return -1;

    if (0 == p_fifo->free_num)
        return 0;

    if(p_fifo->p_write_addr > p_fifo->p_end_addr)
        p_fifo->p_write_addr = p_fifo->p_start_addr;

    len = (len < p_fifo->free_num) ? len : p_fifo->free_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_write_addr + 1;

    if(len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        p_fifo->p_write_addr += len_to_end;
    }
    else  //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        memcpy(p_fifo->p_start_addr, p_source + len_to_end, len_from_start);
        p_fifo->p_write_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num -= len;
    p_fifo->used_num += len;
    retval = len;

    return retval;
}


int fifo_gets(fifo_t* p_fifo, char* p_dest, int len)
{
    int retval;
    int len_to_end;
    int len_from_start;

    if (NULL == p_dest)
        return -1;

    if (0 == p_fifo->used_num)
        return 0;

    if(p_fifo->p_read_addr > p_fifo->p_end_addr)
        p_fifo->p_read_addr = p_fifo->p_start_addr;

    len = (len < p_fifo->used_num) ? len : p_fifo->used_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_read_addr + 1;

    if(len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        p_fifo->p_read_addr += len_to_end;
    }
    else  //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
        p_fifo->p_read_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num += len;
    p_fifo->used_num -= len;
    retval = len;

    return retval;
}


int fifo_prereads(fifo_t* p_fifo, char *p_dest, int offset, int len)
{
    int retval;
    char *tmp_read_addr;
    int len_to_end;
    int len_from_start;


    if (NULL == p_dest)
        return -1;

    if (0 == p_fifo->used_num)
        return -1;

    if (offset >= p_fifo->used_num)
        return -1;


    tmp_read_addr = p_fifo->p_read_addr + offset;
    if(tmp_read_addr > p_fifo->p_end_addr)
        tmp_read_addr = tmp_read_addr - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;

    len = (len < (p_fifo->used_num - offset)) ? len : (p_fifo->used_num - offset);
    len_to_end = p_fifo->p_end_addr - tmp_read_addr + 1;

    if(len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, tmp_read_addr, len_to_end);
    }
    else  //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, tmp_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
    }

    retval = len;

    return retval;
}


int fifo_used(fifo_t* p_fifo)
{
    return p_fifo->used_num;
}


int fifo_free(fifo_t* p_fifo)
{
    return p_fifo->free_num;
}
