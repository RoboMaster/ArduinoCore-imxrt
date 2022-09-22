/**
 * @file open_protocol_error.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2022-01-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _OPEN_PROTOCOL_ERROR_H_
#define _OPEN_PROTOCOL_ERROR_H_

typedef enum
{
    OPEN_PROTO_NORMAL           = 0,

    OPEN_PROTO_INVALID_PARAM    = 0xE3,
    OPEN_PROTO_IDX_ERROR        = 0xF0,
    OPEN_PROTO_OVERSIZE         = 0xF1,
    OPEN_PROTO_VERIFY_FAIL      = 0xF2,

    OPEN_PROTO_FLASH_ERROR      = 0xF4,

    OPEN_PROTO_WRONG_LENGTH     = 0xF7

}open_proto_error_e;

#endif