/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
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