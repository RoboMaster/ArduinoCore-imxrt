/**
 * @file flash.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @version 0.1
 * @date 2021-12-29
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef FLASH_H
#define FLASH_H

#include "stdint.h"
#include "fsl_romapi.h"

/* Exported define ------------------------------------------------------------*/
#define SYS_PARAM_FLASH_ADDR          (0x700FF000)
#define SYS_PARAM_BACKUP_FLASH_ADDR   (0x700FE000)

#define FLASH_ADDR_START              (0x70000000)  /* FLASH起始地址 */
#define FLASH_2RD_IAMGE_START         (0x70100000)
#define FLASH_ADDR_OFFSET             (0x70000000)
#define FLASH_ADDR_END                (0x703FFFFF)  /* FLASH末尾地址 */
#define MIN_ERASE_SIZE                (0x1000)      /* 最小擦除单位 */
#define MIN_PROGRAM_SIZE              (0x100)       /* 最小编程单位 */
/* Exported typedef -----------------------------------------------------------*/

typedef enum
{
  FLASH_OK = 0,

  FLASH_BUSY = 1,
  FLASH_ERROR,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_COMPLETE,
  FLASH_TIMEOUT,

  FLASH_PARAMETER_ERROR,
}flash_status;

/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/

uint32_t flash_init(void);
uint32_t flash_erase(uint32_t addr, uint32_t erase_size);
uint32_t flash_write(uint32_t addr, uint8_t *buf, uint32_t num);
uint32_t flash_read(uint32_t addr, uint8_t *buf, uint32_t num);

#endif
