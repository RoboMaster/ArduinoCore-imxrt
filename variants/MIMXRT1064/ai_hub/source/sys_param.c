/**
 * @file sys_param.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief 系统参数操作逻辑，包含Loader和APP参数
 * @version 0.1
 * @date 2021-12-31
 *
 * @copyright Copyright (c) 2021 DJI
 *
 */
#include "sys_param.h"
#include "flash.h"
#include "crc.h"
#include "string.h"
#include "user_board.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static sys_param_t param_cache;
static uint8_t param_cache_valid;

/* Private function prototypes -----------------------------------------------*/
static int sys_param_read_with_addr(uint32_t flash_addr, sys_param_save_t* read);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 保存系统参数
 *
 * @param save 需要保存的系统参数
 * @return int 返回值 @sys_param_status_t
 */
int sys_param_save(sys_param_t* save)
{
    sys_param_save_t read_buf;
    sys_param_save_t write_buf;
    uint32_t write_addr = SYS_PARAM_FLASH_ADDR;
    uint32_t erase_addr = SYS_PARAM_BACKUP_FLASH_ADDR;

    /* 选择写入主区域还是备用区（两者交替写入） */
    if(SYS_PARAM_OK == sys_param_read_with_addr(SYS_PARAM_FLASH_ADDR, &read_buf))
    {
        write_addr = SYS_PARAM_BACKUP_FLASH_ADDR;
        erase_addr = SYS_PARAM_FLASH_ADDR;
        if(memcmp(&(read_buf.param), save, sizeof(sys_param_t)) == 0)
        {
            return SYS_PARAM_OK;
        }
    }
    else if(SYS_PARAM_OK == sys_param_read_with_addr(SYS_PARAM_BACKUP_FLASH_ADDR, &read_buf))
    {
        write_addr = SYS_PARAM_FLASH_ADDR;
        erase_addr = SYS_PARAM_BACKUP_FLASH_ADDR;
        if(memcmp(&(read_buf.param), save, sizeof(sys_param_t)) == 0)
        {
            return SYS_PARAM_OK;
        }
    }

    param_cache_valid = 0;

    /* 加上校验信息 */
    memcpy((uint8_t*)(&(write_buf.param)), (uint8_t*)(save), sizeof(sys_param_t));
    write_buf.magic_num = PARAM_MAGIC_NUM;
    write_buf.crc16 = crc16_checksum_get((uint8_t*)(&write_buf), sizeof(sys_param_save_t) - 2, 0);

    /* 开始写入 */
    uint32_t retry = 5;
    uint8_t is_write_ok = 0;
    while(retry--)
    {
        board_wdog_set_timeout(25000);
        if(FLASH_OK != flash_erase(write_addr, MIN_ERASE_SIZE))
        {
            board_wdog_set_timeout(2000);
            continue;
        }
        board_wdog_set_timeout(2000);

        if(FLASH_OK != flash_write(write_addr, (uint8_t*)(&write_buf), sizeof(sys_param_save_t)))
        {
            continue;
        }

        memset(&read_buf, 0, sizeof(sys_param_save_t));
        flash_read(write_addr, (uint8_t*)(&read_buf), sizeof(sys_param_save_t));
        if(0 != memcmp(&read_buf, &write_buf, sizeof(sys_param_save_t)))
        {
            continue;
        }

        /* 写入校验成功，擦掉另外一个区域 */
        if(FLASH_OK != flash_erase(erase_addr, MIN_ERASE_SIZE))
        {
            continue;
        }

        is_write_ok = 1;
        break;
    }

    if(!is_write_ok)
    {
        return SYS_PARAM_ERROR;
    }

    return SYS_PARAM_OK;
}

/**
 * @brief 读取保存的参数
 *
 * @param read 读取缓冲区
 * @return int 返回值 @sys_param_status_t
 */
int sys_param_read(sys_param_t* read)
{
    sys_param_save_t read_buf;
    uint8_t ret = SYS_PARAM_OK;

    if(param_cache_valid)
    {
        memcpy(read, &(param_cache), sizeof(sys_param_t));
        return SYS_PARAM_OK;
    }

    if(SYS_PARAM_OK == sys_param_read_with_addr(SYS_PARAM_FLASH_ADDR, &read_buf))
    {
        memcpy(read, &(read_buf.param), sizeof(sys_param_t));
        memcpy(&param_cache, &(read_buf.param), sizeof(sys_param_t));
        param_cache_valid = 1;
        return SYS_PARAM_OK;
    }
    else
    {
        if(SYS_PARAM_OK == sys_param_read_with_addr(SYS_PARAM_BACKUP_FLASH_ADDR, &read_buf))
        {
            memcpy(read, &(read_buf.param), sizeof(sys_param_t));
            memcpy(&param_cache, &(read_buf.param), sizeof(sys_param_t));
            param_cache_valid = 1;
            return SYS_PARAM_OK;
        }
    }
    return SYS_PARAM_ERROR;
}

/* Private functions ---------------------------------------------------------*/
static int sys_param_read_with_addr(uint32_t flash_addr, sys_param_save_t* read)
{
    flash_read(flash_addr, (uint8_t*)read, sizeof(sys_param_save_t));

    if(read->magic_num != PARAM_MAGIC_NUM)
    {
        return SYS_PARAM_ERROR;
    }

    if(read->crc16 != crc16_checksum_get((uint8_t*)(read), sizeof(sys_param_save_t) - 2, 0))
    {
        return SYS_PARAM_CRC_ERROR;
    }

    return SYS_PARAM_OK;
}
