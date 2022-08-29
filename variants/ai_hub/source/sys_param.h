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

#ifndef _SYS_PARAM_H_
#define _SYS_PARAM_H_

#include "flash.h"
#include "stdint.h"

/* Exported define ------------------------------------------------------------*/
#define PARAM_MAGIC_NUM       0x12345601

/* Exported typedef -----------------------------------------------------------*/
typedef enum
{
    SYS_PARAM_OK = 0,
    SYS_PARAM_ERROR = -1,
    SYS_PARAM_CRC_ERROR = -2,
}sys_param_status_t;

#pragma pack(push, 1)

typedef struct
{
    uint32_t loader_ver;
    uint32_t app_ver;
    uint8_t  app_md5[16];
    uint32_t app_size;
    uint8_t  id;
    uint8_t  res[35];
}sys_param_t;

typedef struct
{
    uint32_t magic_num;

    sys_param_t param;

    uint16_t crc16;
}sys_param_save_t;



#pragma pack(pop)


/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
int sys_param_save(sys_param_t* save);
int sys_param_read(sys_param_t* read);


#endif
