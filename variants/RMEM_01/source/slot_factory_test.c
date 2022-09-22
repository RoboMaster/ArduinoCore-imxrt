/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "slot_factory_test.h"
#include "fsl_iomuxc.h"
#include "MIMXRT1064.h"
#include "fsl_gpio.h"
#include "cmsis_os.h"

/* Private define ------------------------------------------------------------*/
#define SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0x083CF008U /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */
#define SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK 0x0FFFFFFFU /*!< GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: affected bits mask */
#define SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK 0x0D3C0FC0U /*!< GPIO3 and GPIO8 share same IO MUX function, GPIO_MUX3 selects one GPIO function: affected bits mask */
#define SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR29_GPIO_MUX4_GPIO_SEL_MASK 0xFFFFFFFFU /*!< GPIO4 and GPIO9 share same IO MUX function, GPIO_MUX4 selects one GPIO function: affected bits mask */

#define SLOT14_BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0xF0C00000U  /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */
#define SLOT14_BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK 0x34U        /*!< GPIO3 and GPIO8 share same IO MUX function, GPIO_MUX3 selects one GPIO function: affected bits mask */

#define SLOT14_T1_PORT                                        COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T1_PIN                                         28U   /*!< PORT pin number */

#define SLOT14_T2_PORT                                        COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT14_T2_PIN                                         2U   /*!< PORT pin number */

#define SLOT14_T3_PORT                                        COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T3_PIN                                         30U   /*!< PORT pin number */

#define SLOT14_T4_PORT                                        COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT14_T4_PIN                                         5U   /*!< PORT pin number */

#define SLOT14_T5_PORT                                        COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT14_T5_PIN                                         4U   /*!< PORT pin number */

#define SLOT14_T6_PORT                                        COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T6_PIN                                         31U   /*!< PORT pin number */

#define SLOT14_T8_PORT                                        COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T8_PIN                                         29U   /*!< PORT pin number */

#define SLOT14_T9_PORT                                        COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T9_PIN                                         23U   /*!< PORT pin number */

#define SLOT14_T10_PORT                                       COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT14_T10_PIN                                        22U   /*!< PORT pin number */

#define SLOT100_T5_PORT                                       COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T5_PIN                                        12U   /*!< PORT pin number */

#define SLOT100_T6_PORT                                       COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T6_PIN                                        13U   /*!< PORT pin number */

#define SLOT100_T7_PORT                                       COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T7_PIN                                        14U   /*!< PORT pin number */

#define SLOT100_T8_PORT                                       COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T8_PIN                                        15U   /*!< PORT pin number */

#define SLOT100_T10_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T10_PIN                                       0U   /*!< PORT pin number */

#define SLOT100_T11_PORT                                      COMPRESS_GPIO7   /*!< PORT peripheral base pointer */
#define SLOT100_T11_PIN                                       1U   /*!< PORT pin number */

#define SLOT100_T12_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T12_PIN                                       2U   /*!< PORT pin number */

#define SLOT100_T13_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T13_PIN                                       3U   /*!< PORT pin number */

#define SLOT100_T15_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T15_PIN                                       4U   /*!< PORT pin number */

#define SLOT100_T16_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T16_PIN                                       5U   /*!< PORT pin number */

#define SLOT100_T17_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T17_PIN                                       6U   /*!< PORT pin number */

#define SLOT100_T18_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T18_PIN                                       7U   /*!< PORT pin number */

#define SLOT100_T19_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T19_PIN                                       8U   /*!< PORT pin number */

#define SLOT100_T20_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T20_PIN                                       9U   /*!< PORT pin number */

#define SLOT100_T21_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T21_PIN                                       10U   /*!< PORT pin number */

#define SLOT100_T22_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T22_PIN                                       11U   /*!< PORT pin number */

#define SLOT100_T23_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T23_PIN                                       12U   /*!< PORT pin number */

#define SLOT100_T24_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T24_PIN                                       13U   /*!< PORT pin number */

#define SLOT100_T25_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T25_PIN                                       14U   /*!< PORT pin number */

#define SLOT100_T26_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T26_PIN                                       15U   /*!< PORT pin number */

#define SLOT100_T27_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T27_PIN                                       16U   /*!< PORT pin number */

#define SLOT100_T28_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T28_PIN                                       17U   /*!< PORT pin number */

#define SLOT100_T29_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T29_PIN                                       18U   /*!< PORT pin number */

#define SLOT100_T30_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T30_PIN                                       19U   /*!< PORT pin number */

#define SLOT100_T32_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T32_PIN                                       21U   /*!< PORT pin number */

#define SLOT100_T33_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T33_PIN                                       8U   /*!< PORT pin number */

#define SLOT100_T34_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T34_PIN                                       19U   /*!< PORT pin number */

#define SLOT100_T35_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T35_PIN                                       20U   /*!< PORT pin number */

#define SLOT100_T36_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T36_PIN                                       24U   /*!< PORT pin number */

#define SLOT100_T37_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T37_PIN                                       27U   /*!< PORT pin number */

#define SLOT100_T38_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T38_PIN                                       23U   /*!< PORT pin number */

#define SLOT100_T39_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T39_PIN                                       24U   /*!< PORT pin number */

#define SLOT100_T40_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T40_PIN                                       25U   /*!< PORT pin number */

#define SLOT100_T41_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T41_PIN                                       3U   /*!< PORT pin number */

#define SLOT100_T42_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T42_PIN                                       11U   /*!< PORT pin number */

#define SLOT100_T43_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T43_PIN                                       12U   /*!< PORT pin number */

#define SLOT100_T44_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T44_PIN                                       13U   /*!< PORT pin number */

#define SLOT100_T46_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T46_PIN                                       18U   /*!< PORT pin number */

#define SLOT100_T47_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T47_PIN                                       19U   /*!< PORT pin number */

#define SLOT100_T48_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T48_PIN                                       20U   /*!< PORT pin number */

#define SLOT100_T49_PORT                                      COMPRESS_GPIO1   /*!< PORT peripheral base pointer */
#define SLOT100_T49_PIN                                       21U   /*!< PORT pin number */

#define SLOT100_T53_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T53_PIN                                       6U   /*!< PORT pin number */

#define SLOT100_T54_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T54_PIN                                       7U   /*!< PORT pin number */

#define SLOT100_T56_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T56_PIN                                       8U   /*!< PORT pin number */

#define SLOT100_T57_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T57_PIN                                       26U   /*!< PORT pin number */

#define SLOT100_T59_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T59_PIN                                       0U   /*!< PORT pin number */

#define SLOT100_T60_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T60_PIN                                       1U   /*!< PORT pin number */

#define SLOT100_T61_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T61_PIN                                       2U   /*!< PORT pin number */

#define SLOT100_T62_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T62_PIN                                       3U   /*!< PORT pin number */

#define SLOT100_T63_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T63_PIN                                       4U   /*!< PORT pin number */

#define SLOT100_T64_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T64_PIN                                       5U   /*!< PORT pin number */

#define SLOT100_T65_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T65_PIN                                       6U   /*!< PORT pin number */

#define SLOT100_T66_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T66_PIN                                       7U   /*!< PORT pin number */

#define SLOT100_T67_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T67_PIN                                       9U   /*!< PORT pin number */

#define SLOT100_T68_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T68_PIN                                       11U   /*!< PORT pin number */

#define SLOT100_T69_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T69_PIN                                       29U   /*!< PORT pin number */

#define SLOT100_T70_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T70_PIN                                       10U   /*!< PORT pin number */

#define SLOT100_T71_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T71_PIN                                       9U   /*!< PORT pin number */

#define SLOT100_T73_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T73_PIN                                       26U   /*!< PORT pin number */

#define SLOT100_T74_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T74_PIN                                       25U   /*!< PORT pin number */

#define SLOT100_T75_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T75_PIN                                       27U   /*!< PORT pin number */

#define SLOT100_T76_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T76_PIN                                       20U   /*!< PORT pin number */

#define SLOT100_T77_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T77_PIN                                       21U   /*!< PORT pin number */

#define SLOT100_T78_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T78_PIN                                       22U   /*!< PORT pin number */

#define SLOT100_T79_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T79_PIN                                       23U   /*!< PORT pin number */

#define SLOT100_T80_PORT                                      COMPRESS_GPIO2   /*!< PORT peripheral base pointer */
#define SLOT100_T80_PIN                                       24U   /*!< PORT pin number */

#define SLOT100_T82_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T82_PIN                                       26U   /*!< PORT pin number */

#define SLOT100_T83_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T83_PIN                                       27U   /*!< PORT pin number */

#define SLOT100_T85_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T85_PIN                                       22U   /*!< PORT pin number */

#define SLOT100_T86_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T86_PIN                                       21U   /*!< PORT pin number */

#define SLOT100_T87_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T87_PIN                                       19U   /*!< PORT pin number */

#define SLOT100_T88_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T88_PIN                                       31U   /*!< PORT pin number */

#define SLOT100_T89_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T89_PIN                                       18U   /*!< PORT pin number */

#define SLOT100_T90_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T90_PIN                                       17U   /*!< PORT pin number */

#define SLOT100_T91_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T91_PIN                                       18U   /*!< PORT pin number */

#define SLOT100_T92_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T92_PIN                                       10U   /*!< PORT pin number */

#define SLOT100_T93_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T93_PIN                                       14U   /*!< PORT pin number */

#define SLOT100_T94_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T94_PIN                                       15U   /*!< PORT pin number */

#define SLOT100_T95_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T95_PIN                                       16U   /*!< PORT pin number */

#define SLOT100_T96_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T96_PIN                                       30U   /*!< PORT pin number */

#define SLOT100_T97_PORT                                      COMPRESS_GPIO3   /*!< PORT peripheral base pointer */
#define SLOT100_T97_PIN                                       20U   /*!< PORT pin number */

#define SLOT100_T98_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T98_PIN                                       27U   /*!< PORT pin number */

#define SLOT100_T99_PORT                                      COMPRESS_GPIO4   /*!< PORT peripheral base pointer */
#define SLOT100_T99_PIN                                       28U   /*!< PORT pin number */


/* Private typedef -----------------------------------------------------------*/
/**
 * @brief RT1064官方库提供的GPIO索引是int32类型，使用int8替代压缩
 *
 */
typedef enum
{
    COMPRESS_GPIO1,
    COMPRESS_GPIO2,
    COMPRESS_GPIO3,
    COMPRESS_GPIO4,
    //COMPRESS_GPIO5,
    //COMPRESS_GPIO6,
    COMPRESS_GPIO7,
}compress_gpio_e;

#pragma pack(push, 1)

/**
 * @brief 测试GPIO对
 *
 */
typedef struct
{
    compress_gpio_e input_compress_gpio;
    uint8_t         input_pin;

    compress_gpio_e output_compress_gpio;
    uint8_t         output_pin;
}gpio_test_pair;

#pragma pack(pop)
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* 14Pin插槽GPIO 测试对 */
gpio_test_pair slot_14_test_pair[] =
{
    {SLOT14_T1_PORT, SLOT14_T1_PIN, SLOT14_T6_PORT,  SLOT14_T6_PIN},
    {SLOT14_T2_PORT, SLOT14_T2_PIN, SLOT14_T6_PORT,  SLOT14_T6_PIN},
    {SLOT14_T3_PORT, SLOT14_T3_PIN, SLOT14_T8_PORT,  SLOT14_T8_PIN},
    {SLOT14_T4_PORT, SLOT14_T4_PIN, SLOT14_T9_PORT,  SLOT14_T9_PIN},
    {SLOT14_T5_PORT, SLOT14_T5_PIN, SLOT14_T10_PORT, SLOT14_T10_PIN}
};

/* 100Pin插槽GPIO 测试对 */
gpio_test_pair slot_100_test_pair[] =
{
    {SLOT100_T5_PORT,  SLOT100_T5_PIN,  SLOT100_T6_PORT,   SLOT100_T6_PIN},
    {SLOT100_T7_PORT,  SLOT100_T7_PIN,  SLOT100_T56_PORT,  SLOT100_T56_PIN},
    {SLOT100_T8_PORT,  SLOT100_T8_PIN,  SLOT100_T57_PORT,  SLOT100_T57_PIN},

    {SLOT100_T59_PORT, SLOT100_T59_PIN, SLOT100_T10_PORT,  SLOT100_T10_PIN}, /* 陪测板中T59,T60,T10三个IO接在了一起*/
    {SLOT100_T60_PORT, SLOT100_T60_PIN, SLOT100_T10_PORT,  SLOT100_T10_PIN},

    {SLOT100_T11_PORT, SLOT100_T11_PIN, SLOT100_T61_PORT,  SLOT100_T61_PIN},
    {SLOT100_T12_PORT, SLOT100_T12_PIN, SLOT100_T62_PORT,  SLOT100_T62_PIN},
    {SLOT100_T13_PORT, SLOT100_T13_PIN, SLOT100_T63_PORT,  SLOT100_T63_PIN},
    {SLOT100_T15_PORT, SLOT100_T15_PIN, SLOT100_T64_PORT,  SLOT100_T64_PIN},
    {SLOT100_T16_PORT, SLOT100_T16_PIN, SLOT100_T65_PORT,  SLOT100_T65_PIN},
    {SLOT100_T17_PORT, SLOT100_T17_PIN, SLOT100_T66_PORT,  SLOT100_T66_PIN},
    {SLOT100_T18_PORT, SLOT100_T18_PIN, SLOT100_T67_PORT,  SLOT100_T67_PIN},
    {SLOT100_T19_PORT, SLOT100_T19_PIN, SLOT100_T68_PORT,  SLOT100_T68_PIN},
    {SLOT100_T20_PORT, SLOT100_T20_PIN, SLOT100_T69_PORT,  SLOT100_T69_PIN},
    {SLOT100_T21_PORT, SLOT100_T21_PIN, SLOT100_T70_PORT,  SLOT100_T70_PIN},
    {SLOT100_T22_PORT, SLOT100_T22_PIN, SLOT100_T71_PORT,  SLOT100_T71_PIN},
    {SLOT100_T23_PORT, SLOT100_T23_PIN, SLOT100_T73_PORT,  SLOT100_T73_PIN},
    {SLOT100_T24_PORT, SLOT100_T24_PIN, SLOT100_T74_PORT,  SLOT100_T74_PIN},
    {SLOT100_T25_PORT, SLOT100_T25_PIN, SLOT100_T75_PORT,  SLOT100_T75_PIN},
    {SLOT100_T26_PORT, SLOT100_T26_PIN, SLOT100_T76_PORT,  SLOT100_T76_PIN},
    {SLOT100_T27_PORT, SLOT100_T27_PIN, SLOT100_T77_PORT,  SLOT100_T77_PIN},
    {SLOT100_T28_PORT, SLOT100_T28_PIN, SLOT100_T78_PORT,  SLOT100_T78_PIN},
    {SLOT100_T29_PORT, SLOT100_T29_PIN, SLOT100_T79_PORT,  SLOT100_T79_PIN},
    {SLOT100_T30_PORT, SLOT100_T30_PIN, SLOT100_T80_PORT,  SLOT100_T80_PIN},
    {SLOT100_T32_PORT, SLOT100_T32_PIN, SLOT100_T82_PORT,  SLOT100_T82_PIN},
    {SLOT100_T33_PORT, SLOT100_T33_PIN, SLOT100_T83_PORT,  SLOT100_T83_PIN},
    {SLOT100_T34_PORT, SLOT100_T34_PIN, SLOT100_T85_PORT,  SLOT100_T85_PIN},
    {SLOT100_T35_PORT, SLOT100_T35_PIN, SLOT100_T86_PORT,  SLOT100_T86_PIN},
    {SLOT100_T36_PORT, SLOT100_T36_PIN, SLOT100_T87_PORT,  SLOT100_T87_PIN},
    {SLOT100_T37_PORT, SLOT100_T37_PIN, SLOT100_T88_PORT,  SLOT100_T88_PIN},
    {SLOT100_T38_PORT, SLOT100_T38_PIN, SLOT100_T89_PORT,  SLOT100_T89_PIN},
    {SLOT100_T39_PORT, SLOT100_T39_PIN, SLOT100_T90_PORT,  SLOT100_T90_PIN},
    {SLOT100_T40_PORT, SLOT100_T40_PIN, SLOT100_T91_PORT,  SLOT100_T91_PIN},
    {SLOT100_T41_PORT, SLOT100_T41_PIN, SLOT100_T92_PORT,  SLOT100_T92_PIN},
    {SLOT100_T42_PORT, SLOT100_T42_PIN, SLOT100_T93_PORT,  SLOT100_T93_PIN},
    {SLOT100_T43_PORT, SLOT100_T43_PIN, SLOT100_T94_PORT,  SLOT100_T94_PIN},
    {SLOT100_T44_PORT, SLOT100_T44_PIN, SLOT100_T95_PORT,  SLOT100_T95_PIN},
    {SLOT100_T46_PORT, SLOT100_T46_PIN, SLOT100_T96_PORT,  SLOT100_T96_PIN},
    {SLOT100_T47_PORT, SLOT100_T47_PIN, SLOT100_T97_PORT,  SLOT100_T97_PIN},
    {SLOT100_T48_PORT, SLOT100_T48_PIN, SLOT100_T98_PORT,  SLOT100_T98_PIN},
    {SLOT100_T49_PORT, SLOT100_T49_PIN, SLOT100_T99_PORT,  SLOT100_T99_PIN},
    {SLOT100_T53_PORT, SLOT100_T53_PIN, SLOT100_T54_PORT,  SLOT100_T54_PIN}
};

/* Private function prototypes -----------------------------------------------*/
static GPIO_Type* gpio_uncompass(compress_gpio_e compress_gpio);
static void gpio_init_group(gpio_test_pair *group, uint16_t unit_num);
static int gpio_test_group(gpio_test_pair *group, uint16_t unit_num);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief 测试PCBA上14pin的插槽，需要接入14Pin配测板.
 *
 * @return int 返回测试结果 0:测试成功 -1:测试失败
 */
int slot_14pin_test(void)
{
    uint8_t pair_num = sizeof(slot_14_test_pair) / sizeof(gpio_test_pair);

    gpio_init_group(slot_14_test_pair, pair_num);

    /* IO Mux设置, 由MCUXpresso Config Tools自动生成 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO3_IO02, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO3_IO05, 0U);
    IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(SLOT14_BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK)))
      | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0x00U)
    );
  IOMUXC_GPR->GPR28 = ((IOMUXC_GPR->GPR28 &
    (~(SLOT14_BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK)))
      | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(0x00U)
    );

    /* 开始测试 */
    return gpio_test_group(slot_14_test_pair, pair_num);
}

/**
 * @brief 测试PCBA上100pin的插槽，需要接入100Pin配测板.
 *
 * @return int 返回测试结果 0:测试成功 -1:测试失败
 */
int slot_100pin_test(void)
{
    uint8_t pair_num = sizeof(slot_100_test_pair) / sizeof(gpio_test_pair);

    /* 初始化GPIO */
    gpio_init_group(slot_100_test_pair, pair_num);

    /* IO Mux设置, 由MCUXpresso Config Tools自动生成 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_00_GPIO1_IO00, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_03_GPIO1_IO03, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_GPIO1_IO12, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_GPIO1_IO13, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_14_GPIO1_IO14, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_GPIO1_IO18, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_GPIO1_IO19, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_GPIO1_IO21, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_GPIO2_IO00, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_GPIO2_IO01, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_GPIO2_IO02, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_GPIO2_IO03, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_GPIO2_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_GPIO2_IO05, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_GPIO2_IO06, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_GPIO2_IO07, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_GPIO2_IO08, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_GPIO2_IO09, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_GPIO2_IO10, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_GPIO2_IO11, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_GPIO2_IO12, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_GPIO2_IO13, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_GPIO2_IO14, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_GPIO2_IO15, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_GPIO2_IO16, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_GPIO2_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_GPIO2_IO18, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_GPIO2_IO19, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_GPIO2_IO20, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_GPIO2_IO21, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_GPIO2_IO22, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_GPIO2_IO23, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_GPIO2_IO24, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_GPIO2_IO25, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_GPIO2_IO26, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_GPIO2_IO28, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_GPIO2_IO29, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_GPIO4_IO00, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_GPIO4_IO01, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_GPIO4_IO02, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_GPIO4_IO03, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_GPIO4_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_GPIO4_IO05, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_GPIO4_IO06, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_GPIO4_IO07, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_GPIO4_IO09, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_GPIO4_IO11, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_GPIO4_IO12, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_13_GPIO4_IO13, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_17_GPIO4_IO17, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_GPIO4_IO18, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_23_GPIO4_IO23, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_24_GPIO4_IO24, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_25_GPIO4_IO25, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_26_GPIO4_IO26, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_27_GPIO4_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_29_GPIO4_IO29, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_30_GPIO4_IO30, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_GPIO4_IO31, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_GPIO3_IO18, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_38_GPIO3_IO24, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPIO3_IO26, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_GPIO3_IO27, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_GPIO3_IO00, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_GPIO3_IO05, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_GPIO3_IO06, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_GPIO3_IO07, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_GPIO3_IO08, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_GPIO3_IO09, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_GPIO3_IO10, 0U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_GPIO3_IO11, 0U);

    IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
        (~(SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK)))
        | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0x00U)
        );
    IOMUXC_GPR->GPR27 = ((IOMUXC_GPR->GPR27 &
        (~(SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK)))
        | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(0x02U)
        );
    IOMUXC_GPR->GPR28 = ((IOMUXC_GPR->GPR28 &
        (~(SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK)))
        | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(0x00U)
        );
    IOMUXC_GPR->GPR29 = ((IOMUXC_GPR->GPR29 &
        (~(SLOT100_BOARD_INITPINS_IOMUXC_GPR_GPR29_GPIO_MUX4_GPIO_SEL_MASK)))
        | IOMUXC_GPR_GPR29_GPIO_MUX4_GPIO_SEL(0x00U)
        );

    /* 开始测试 */
    return gpio_test_group(slot_100_test_pair, pair_num);
}


/* Private functions ---------------------------------------------------------*/
/**
 * @brief 将8位被压缩的GPIO转换回官方库32位的GPIO结构体
 *
 * @param compress_gpio
 * @return GPIO_Type*
 */
static GPIO_Type* gpio_uncompass(compress_gpio_e compress_gpio)
{
    switch(compress_gpio)
    {
        case COMPRESS_GPIO1: return GPIO1;
        case COMPRESS_GPIO2: return GPIO2;
        case COMPRESS_GPIO3: return GPIO3;
        case COMPRESS_GPIO4: return GPIO4;
        //case COMPRESS_GPIO5: return GPIO5;
        //case COMPRESS_GPIO6: return GPIO6;
        case COMPRESS_GPIO7: return GPIO7;
    }
}

/**
 * @brief 初始化GPIO
 *
 * @param group 需要初始化IO测试对列表
 * @param unit_num IO测试对的数量
 */
static void gpio_init_group(gpio_test_pair *group, uint16_t unit_num)
{
    /* 初始化GPIO */
    gpio_pin_config_t gpio_config;
    gpio_config.outputLogic = 0U;
    gpio_config.interruptMode = kGPIO_NoIntmode;

    for(int i = 0; i < unit_num; i++)
    {
        gpio_config.direction = kGPIO_DigitalInput;
        GPIO_PinInit(gpio_uncompass(group[i].input_compress_gpio),
                     group[i].input_pin,
                     &gpio_config);
        gpio_config.direction = kGPIO_DigitalOutput;
        GPIO_PinInit(gpio_uncompass(group[i].output_compress_gpio),
                     group[i].output_pin,
                     &gpio_config);
    }
}

/**
 * @brief 测试IO对
 *
 * @param group 需要初始化IO测试对列表
 * @param unit_num IO测试对的数量
 * @return int 返回测试结果 0:测试成功 -1:测试失败
 */
static int gpio_test_group(gpio_test_pair *group, uint16_t unit_num)
{
    for(int i = 0; i < unit_num; i++)
    {
        GPIO_WritePinOutput(gpio_uncompass(group[i].output_compress_gpio), group[i].output_pin, 1);
    }

    osDelay(2);

    for(int i = 0; i < unit_num; i++)
    {
        if(!GPIO_ReadPinInput(gpio_uncompass(group[i].input_compress_gpio), group[i].input_pin))
        {
            return -1;
        }
    }

    for(int i = 0; i < unit_num; i++)
    {
        GPIO_WritePinOutput(gpio_uncompass(group[i].output_compress_gpio), group[i].output_pin, 0);
    }

    osDelay(2);

    for(int i = 0; i < unit_num; i++)
    {
        if(GPIO_ReadPinInput(gpio_uncompass(group[i].input_compress_gpio), group[i].input_pin))
        {
            return -1;
        }
    }

    return 0;
}
