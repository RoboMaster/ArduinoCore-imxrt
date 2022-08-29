/**
 * @file user_board.h
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-12-5
 *
 * @copyright Copyright (c) 2021, DJI
 *
 */

#ifndef __USER_BOARD_H__
#define __USER_BOARD_H__

#include "stdint.h"

#define BOARD_SN_FUSE_MAP_ADDRESS_START     (0x38)
#define BOARD_SN_WORDS     (4)
#define BOARD_SN_BYTES     (BOARD_SN_WORDS * 4)

#define CPU_SR_T                unsigned long
#define ENTER_CRITICAL          __get_PRIMASK();__disable_irq
#define EXIT_CRITICAL(CPU_SR)   __set_PRIMASK(CPU_SR);

typedef void (*gpio_int_f)(void);          /*!< 解包回调函数  */

int user_board_int(void);

void board_led_set(uint8_t mask, uint8_t value);
void board_led_togle(uint8_t mask);

int board_sn_write(uint8_t* sn);
int board_sn_read(uint8_t* sn);
uint16_t board_sn_crc16(void);

uint8_t board_key_read(void);

void board_sys_reboot(void);

void board_enter_mcu_isp(void);
void board_boot_1rd_pattern(void);
void board_boot_2rd_pattern(void);
void board_misc_fuse_write(void);

void board_wdog_init(int ms);
void board_wdog_refresh(void);
void board_wdog_set_timeout(int ms);
int board_wdog_is_timeout_reset(void);

void board_gpio_spi3_cs0_int_reg(gpio_int_f fun);
void board_gpio_spi3_cs0_int_enable(uint8_t enable);

#endif
