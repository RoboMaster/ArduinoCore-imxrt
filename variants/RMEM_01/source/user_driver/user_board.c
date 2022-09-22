/**
 * @file user_board.c
 * @author Zhenling Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2021-12-05
 *
 * @copyright Copyright (c) 2021, DJI
 *
 */


#include <string.h>

#include "pin_mux.h"
#include "peripherals.h"
#include "board.h"
#include "fsl_ocotp.h"
#include "fsl_clock.h"
#include "fsl_romapi.h"
#include "fsl_iomuxc.h"
#include "user_board.h"
#include "cmsis_os.h"
#include "crc.h"
#include "fsl_wdog.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
static gpio_int_f gpio_spi3_cs0_int;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



/* Exported functions --------------------------------------------------------*/
/**
 * @brief User board initialization.
 *
 * @return int
 */
int user_board_int(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitPeripherals();

    OCOTP_Init(OCOTP, CLOCK_GetFreq(kCLOCK_IpgClk));
}

/**
 * @brief Set board LED
 *
 * @param mask bit0~4 represent LED0~4 Select enable bit
 * @param value
 */
void board_led_set(uint8_t mask, uint8_t value)
{
    value = value > 0 ? 1 : 0;
    if(mask & 0x01)
    {
        GPIO_WritePinOutput(BOARD_INITPINS_LED1_PERIPHERAL, BOARD_INITPINS_LED1_CHANNEL, value);
    }

    if(mask & 0x02)
    {
        GPIO_WritePinOutput(BOARD_INITPINS_LED2_PERIPHERAL, BOARD_INITPINS_LED2_CHANNEL, value);
    }

    if(mask & 0x04)
    {
        GPIO_WritePinOutput(BOARD_INITPINS_LED3_PERIPHERAL, BOARD_INITPINS_LED3_CHANNEL, value);
    }

    if(mask & 0x08)
    {
        GPIO_WritePinOutput(BOARD_INITPINS_LED4_PERIPHERAL, BOARD_INITPINS_LED4_CHANNEL, value);
    }
}

/**
 * @brief Toggle board LED
 *
 * @param mask
 */
void board_led_togle(uint8_t mask)
{
    if(mask & 0x01)
    {
        GPIO_PortToggle(BOARD_INITPINS_LED1_PERIPHERAL, 0x01 << BOARD_INITPINS_LED1_CHANNEL);
    }

    if(mask & 0x02)
    {
        GPIO_PortToggle(BOARD_INITPINS_LED2_PERIPHERAL, 0x01 << BOARD_INITPINS_LED2_CHANNEL);
    }

    if(mask & 0x04)
    {
        GPIO_PortToggle(BOARD_INITPINS_LED3_PERIPHERAL, 0x01 << BOARD_INITPINS_LED3_CHANNEL);
    }

    if(mask & 0x08)
    {
        GPIO_PortToggle(BOARD_INITPINS_LED4_PERIPHERAL, 0x01 << BOARD_INITPINS_LED4_CHANNEL);
    }
}

/**
 * @brief Board serial number write
 *
 * @param sn
 * @return int
 */
int board_sn_write(uint8_t* sn)
{
    int ret;
    uint32_t write_buff[BOARD_SN_WORDS];
    uint32_t read_buff[BOARD_SN_WORDS];

    board_sn_read((uint8_t*)(read_buff));

    /* 读取LOCK区域(0x00), 检查OTP的GP3区域是否已经被锁住 */
    uint32_t otp_lock= 0xFFFFFFFF;
    OCOTP_ReadFuseShadowRegisterExt(OCOTP, 0x00, &otp_lock, 1);
    otp_lock = otp_lock & 0x0C000000;

    if(memcmp(read_buff, sn, BOARD_SN_BYTES) == 0)
    {
        if(otp_lock)
        {
            return 0;
        }
    }
    else
    {
        if(otp_lock)
        {
            return -1;
        }
    }

    /* 往OTP GP3区域写入 */
    memcpy(write_buff, sn, BOARD_SN_BYTES);
    uint8_t retry = 4;
    do
    {
        ret = 0;
        for (int i = 0; i < BOARD_SN_WORDS; i++)
        {
            __DSB();
            ret |= OCOTP_WriteFuseShadowRegister(OCOTP,
                    BOARD_SN_FUSE_MAP_ADDRESS_START + i,
                    write_buff[i]);
        }

        uint32_t read_buff[BOARD_SN_WORDS];
        board_sn_read((uint8_t*)read_buff);
        if(memcmp(read_buff, write_buff, BOARD_SN_BYTES) != 0)
        {
            ret = -1;
        }

    }while(ret != kStatus_Success && retry--);

    /* 往OTP_LOCK区（0x00)的Bit26~27都写入1锁住GP3 */
    retry = 4;
    do
    {
        __DSB();
        ret = OCOTP_WriteFuseShadowRegister(OCOTP, 0x00, 0x0C000000);
    }while(ret != kStatus_Success && retry--);

    return 0;
}

/**
 * @brief Board serial number read
 *
 * @param sn
 * @return int
 */
int board_sn_read(uint8_t* sn)
{
    uint32_t read_buff[BOARD_SN_WORDS];

    for (int i = 0; i < BOARD_SN_WORDS; i++)
    {
        __DSB();
        int ret = OCOTP_ReadFuseShadowRegisterExt(OCOTP,
            BOARD_SN_FUSE_MAP_ADDRESS_START + i,
            read_buff + i,
            1);

        if(ret != kStatus_Success)
        {
            return -1;
        }
    }

    memcpy(sn, read_buff, BOARD_SN_BYTES);
    return 0;
}

/**
 * @brief Board serial number CRC16
 *
 * @param sn
 * @return int
 */
uint16_t board_sn_crc16(void)
{
    uint8_t sn[BOARD_SN_BYTES];
    uint16_t sn_crc16;
    board_sn_read(sn);
    sn[BOARD_SN_BYTES - 1] = '\0';
    sn_crc16 = crc16_checksum_get(sn, strlen(sn), 0x4F19);
    return sn_crc16;
}

/**
 * @brief Board user key read
 *
 * @return uint8_t
 */
uint8_t board_key_read(void)
{
    return GPIO_ReadPinInput(BOARD_INITPINS_USER_KEY_PORT, BOARD_INITPINS_USER_KEY_PIN) == 0;
}

/**
 * @brief System reboot
 *
 */
void board_sys_reboot(void)
{
    WDOG_TriggerSystemSoftwareReset(WDOG1);
    while(1);
}

/**
 * @brief Enter ISP Module
 *
 */
void board_enter_mcu_isp(void)
{
    static uint32_t arg = 0xEB110000;
    ROM_RunBootloader(&arg);
}

/**
 * @brief Enter ISP Module
 *
 */
void board_boot_1rd_pattern(void)
{
    static uint32_t arg = 0xEB000000;
    ROM_RunBootloader(&arg);
}

/**
 * @brief Enter ISP Module
 *
 */
void board_boot_2rd_pattern(void)
{
    static uint32_t arg = 0xEB000001;
    ROM_RunBootloader(&arg);
}

/**
 * @brief Initialize the fuse
 *
 */
void board_misc_fuse_write(void)
{
    uint8_t retry = 4;
    int ret = 0;

    /* 读取MISC_CONFIG1区域(0x2E), 检查Secondary_Image_Offset(Bit23:16)是否为0 */
    uint32_t misc_config1= 0xFFFFFFFF;
    __DSB();
    OCOTP_ReadFuseShadowRegisterExt(OCOTP, 0x2E, &misc_config1, 1);

    /* 设置Secondary_Image_Offset(Bit23:16)为4, Offset = 4 * 256KB = 1MB */
    if((misc_config1 & 0x00FF0000) == 0)
    {
        uint32_t write_word = (4 << 16);
        do{
            __DSB();
            ret = OCOTP_WriteFuseShadowRegister(OCOTP, 0x2E, write_word);
        }while(ret != kStatus_Success && retry--);
    }
}

/**
 * @brief 初始化看门狗
 *
 * @param ms
 */
void board_wdog_init(int ms)
{
#ifndef USE_DEBUG
    wdog_config_t config;
    WDOG_GetDefaultConfig(&config);

    /* Timeout value is (config.timeoutValue + 1)/2s */
    config.timeoutValue = ms / 500;
    WDOG_Init(WDOG1, &config);
#endif
}

/**
 * @brief 刷新看门狗
 *
 */
void board_wdog_refresh(void)
{
#ifndef USE_DEBUG
    WDOG_Refresh(WDOG1);
#endif
}

/**
 * @brief 重新设置看门狗超时时间
 *
 * @param ms
 */
void board_wdog_set_timeout(int ms)
{
#ifndef USE_DEBUG
    WDOG1->WCR = (WDOG1->WCR & (~WDOG_WCR_WT_MASK)) | WDOG_WCR_WT(ms / 500);
    WDOG_Refresh(WDOG1);
#endif
}

/**
 * @brief 检测系统是否为看门狗超时重启
 *
 * @param ms
 */
int board_wdog_is_timeout_reset(void)
{
    int ret = 0;
#ifndef USE_DEBUG
    if(WDOG_GetStatusFlags(WDOG1) & kWDOG_TimeoutResetFlag)
    {
        ret = 1;
    }
#endif
    return ret;
}

/**
 * @brief 注册GPIO SPI CS回调函数
 *
 * @param fun
 */
void board_gpio_spi3_cs0_int_reg(gpio_int_f fun)
{
    gpio_spi3_cs0_int = fun;
}

/**
 * @brief 注册GPIO SPI CS回调函数
 *
 * @param fun
 */
void board_gpio_spi3_cs0_int_enable(uint8_t enable)
{
    if(enable)
    {
        GPIO_PortEnableInterrupts(BOARD_INITPINS_SPI3_CS0_GPIO, BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK);
        EnableIRQ(GPIO1_GPIO_COMB_16_31_IRQN);
    }
    else
    {
        GPIO_PortDisableInterrupts(BOARD_INITPINS_SPI3_CS0_GPIO, BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK);
    }
}

#if defined(CONNECT_AI_CAMERA)
/**
 * @brief GPIO1 16~31中断入口
 *
 */
// 放在Winterrupt中，统一接口
// void Compatible_GPIO1_Combined_16_31_IRQHandler(void)
// {
//     if( GPIO_PortGetInterruptFlags(BOARD_INITPINS_SPI3_CS0_GPIO) & BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK)
//     {
//         GPIO_PortClearInterruptFlags(BOARD_INITPINS_SPI3_CS0_GPIO, BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK);
//         if(gpio_spi3_cs0_int)
//         {
//             gpio_spi3_cs0_int();
//         }
//     }
// }
#endif

void GPIO1_Combined_16_31_IRQHandler(void)
{
    if( GPIO_PortGetInterruptFlags(BOARD_INITPINS_SPI3_CS0_GPIO) & BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK)
    {
        GPIO_PortClearInterruptFlags(BOARD_INITPINS_SPI3_CS0_GPIO, BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK);
        if(gpio_spi3_cs0_int)
        {
            gpio_spi3_cs0_int();
        }
    }
}