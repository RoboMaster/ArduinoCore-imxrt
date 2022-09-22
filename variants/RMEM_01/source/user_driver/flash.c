/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "flash.h"
#include "string.h"

/* Private define ------------------------------------------------------------*/
#define ROM_API_SPI_INSTANCE          (1)


/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static flexspi_nor_config_t config;
static serial_nor_config_option_t option;
static uint8_t flash_init_flag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief 初始化Flash
 *
 * @param addr 擦除起始地址，必须为ERASE_SECTOR_SIZE的整数倍
 * @param erase_size
 * @return uint32_t
 */
uint32_t flash_init(void)
{
    if(flash_init_flag)
    {
        return FLASH_OK;
    }
    option.option0.U = 0xC0000008;      //QuadSPI NOR; Frq: 133MHz;

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    if(kStatus_Success != ROM_FLEXSPI_NorFlash_GetConfig(ROM_API_SPI_INSTANCE, &config, &option))
    {
        __enable_irq();
        return FLASH_ERROR;
    }

    if(kStatus_Success !=  ROM_FLEXSPI_NorFlash_Init(ROM_API_SPI_INSTANCE, &config))
    {
        __enable_irq();
        return FLASH_ERROR;
    }

    __enable_irq();
    return FLASH_OK;
}

/**
 * @brief 对片内Flash进行擦除
 *
 * @param addr 擦除起始地址，必须为ERASE_SECTOR_SIZE的整数倍
 * @param erase_size
 * @return uint32_t
 */
uint32_t flash_erase(uint32_t addr, uint32_t erase_size)
{
    if((addr < FLASH_ADDR_START) || (erase_size + addr > FLASH_ADDR_END + 1) ||
       (addr % MIN_ERASE_SIZE) || (erase_size % MIN_ERASE_SIZE))
    {
        return FLASH_PARAMETER_ERROR;
    }

    ROM_FLEXSPI_NorFlash_ClearCache(ROM_API_SPI_INSTANCE);
    SCB_DisableICache();
    SCB_DisableDCache();
    __disable_irq();

    /* 每个扇区单独进行擦除以节省时间 */
    for(int i = 0; i < erase_size / MIN_ERASE_SIZE; i++)
    {
        /* 检查扇区是否为空白 */
        uint8_t is_blank = 1;
        uint32_t* check_blank_ptr = (uint32_t*)(addr);
        if(addr < FLASH_2RD_IAMGE_START)
        {
            // RT1064直接使用地址从第二Image启动访问起始地址(0x70100000)之前的Flash存在异常，故使用ROM API但效率低
            while((uint32_t)check_blank_ptr < addr + MIN_ERASE_SIZE)
            {
                uint32_t read_buff;
                ROM_FLEXSPI_NorFlash_Read(ROM_API_SPI_INSTANCE, &config, &read_buff, (uint32_t)(check_blank_ptr) - FLASH_ADDR_OFFSET, 4);
                if(read_buff != 0xFFFFFFFF)
                {
                    is_blank = 0;
                    break;
                }
                check_blank_ptr++;
            }
        }
        else
        {
            while((uint32_t)check_blank_ptr < addr + MIN_ERASE_SIZE)
            {
                if(*check_blank_ptr != 0xFFFFFFFF)
                {
                    is_blank = 0;
                    break;
                }
                check_blank_ptr++;
            }
        }

        /* 如果不为空白则进行擦除 */
        if(!is_blank)
        {
            if(kStatus_Success !=  ROM_FLEXSPI_NorFlash_Erase(ROM_API_SPI_INSTANCE, &config, addr - FLASH_ADDR_OFFSET, MIN_ERASE_SIZE))
            {
                SCB_EnableICache();
                SCB_EnableDCache();
                __enable_irq();
                return FLASH_ERROR;
            }
        }
        addr += MIN_ERASE_SIZE;
    }

    // if(kStatus_Success !=  ROM_FLEXSPI_NorFlash_Erase(ROM_API_SPI_INSTANCE, &config, addr - FLASH_ADDR_OFFSET, erase_size))
    // {
    //     SCB_EnableICache();
    //     __enable_irq();
    //     return FLASH_ERROR;
    // }

    SCB_EnableICache();
    SCB_EnableDCache();
    __enable_irq();
    ROM_FLEXSPI_NorFlash_ClearCache(ROM_API_SPI_INSTANCE);
    return FLASH_OK;
}

/**
 * @brief 写入Flash操作
 *
 * @param addr 写入地址，需为MIN_PROGRAM_SIZE的整数倍
 * @param buf  写入缓存
 * @param num  写入数量
 * @return uint32_t
 */
uint32_t flash_write(uint32_t addr, uint8_t *buf, uint32_t num)
{
    static uint32_t program_buff[MIN_PROGRAM_SIZE / 4];

    if((addr % MIN_PROGRAM_SIZE) || (addr + num > FLASH_ADDR_END + 1) ||
        (addr < FLASH_ADDR_START))
    {
        return FLASH_PARAMETER_ERROR;
    }

    ROM_FLEXSPI_NorFlash_ClearCache(ROM_API_SPI_INSTANCE);
    SCB_DisableICache();
    SCB_DisableDCache();
    __disable_irq();

    uint32_t program_page_num = num / MIN_PROGRAM_SIZE;
    uint32_t remain_bytes = num % MIN_PROGRAM_SIZE;

    uint32_t w_addr = addr - FLASH_ADDR_OFFSET;

    for(int i = 0; i < program_page_num; i++)
    {
        memcpy(program_buff, buf, MIN_PROGRAM_SIZE);
        if(kStatus_Success != ROM_FLEXSPI_NorFlash_ProgramPage(ROM_API_SPI_INSTANCE, &config, w_addr, program_buff))
        {
            SCB_EnableICache();
            SCB_EnableDCache();
            __enable_irq();
            return FLASH_ERROR;
        }
        w_addr += MIN_PROGRAM_SIZE;
        buf += MIN_PROGRAM_SIZE;
    }

    if(remain_bytes > 0)
    {
        memset(program_buff, 0xFF, MIN_PROGRAM_SIZE);
        memcpy(program_buff, buf, remain_bytes);
        if(kStatus_Success != ROM_FLEXSPI_NorFlash_ProgramPage(ROM_API_SPI_INSTANCE, &config, w_addr, program_buff))
        {
            SCB_EnableICache();
            SCB_EnableDCache();
            __enable_irq();
            return FLASH_ERROR;
        }
    }

    ROM_FLEXSPI_NorFlash_ClearCache(ROM_API_SPI_INSTANCE);
    SCB_EnableICache();
    SCB_EnableDCache();
    __enable_irq();
    return FLASH_OK;
}

/**
 * @brief 读取Flash操作
 *
 * @param addr
 * @param buf
 * @param num
 */
uint32_t flash_read(uint32_t addr, uint8_t *buf, uint32_t num)
{
    if((addr + num > FLASH_ADDR_END + 1) || (addr < FLASH_ADDR_START))
    {
        return FLASH_PARAMETER_ERROR;
    }

    ROM_FLEXSPI_NorFlash_ClearCache(ROM_API_SPI_INSTANCE);
    __DSB();

    // RT1064直接使用地址从第二Image启动访问起始地址(0x70100000)之前的Flash存在异常，故使用ROM API但效率低
    if(addr < FLASH_2RD_IAMGE_START)
    {
        //TODO:注意Buf的类型强制转换是否可能在后续引发BUG
        ROM_FLEXSPI_NorFlash_Read(ROM_API_SPI_INSTANCE, &config, (uint32_t*)(buf), addr - FLASH_ADDR_OFFSET, num);
    }
    else
    {
        memcpy(buf, (uint8_t*)(addr), num);
    }

    return FLASH_OK;
}

/* Private functions ---------------------------------------------------------*/
