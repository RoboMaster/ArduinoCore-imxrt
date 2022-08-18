#include "variant.h"
#include "fsl_lpuart.h"
/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[] = {
    //////////////////////////////////////////////////////////////
    ///            KEY config
    //////////////////////////////////////////////////////////////
    {
        //                               #0
        .GROUP = GPIO2,
        .PIN = 31U,
        .FUN_GPIO = {IOMUXC_GPIO_B1_15_GPIO2_IO31},
    },
    //////////////////////////////////////////////////////////////
    ///            LED config
    //////////////////////////////////////////////////////////////
    {
        //                               #1
        .GROUP = GPIO1,
        .PIN = 9U,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_09_GPIO1_IO09},
    },
    {
        //                               #2
        .GROUP = GPIO1,
        .PIN = 10U,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_10_GPIO1_IO10},
    },
    {
        //                               #3
        .GROUP = GPIO6,
        .PIN = 11U,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_11_GPIO1_IO11},
    },
    {
        //                               #4
        .GROUP = GPIO6,
        .PIN = 2U,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_02_GPIO1_IO02},
    },
    //////////////////////////////////////////////////////////////
    ///            DEBUG UART config
    //////////////////////////////////////////////////////////////
    {
        //                               #5
        .GROUP = GPIO1,
        .PIN = 13,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_13_GPIO1_IO13},
        .FUN_UART = {IOMUXC_GPIO_AD_B0_13_LPUART1_RX},
    },
    {
        //                               #6
        .GROUP = GPIO1,
        .PIN = 12,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B0_12_GPIO1_IO12},
        .FUN_UART = {IOMUXC_GPIO_AD_B0_12_LPUART1_TX},
    },
    //////////////////////////////////////////////////////////////
    ///            14pin connector config
    //////////////////////////////////////////////////////////////
    ///-------------------- UART3 --------------------
    {
        // UART3_TX                              #7
        .GROUP = GPIO6,
        .PIN = 22U,
        .ADC = ADC1,
        .adcChannel = 11,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_06_GPIO1_IO22},
        .FUN_UART = {IOMUXC_GPIO_AD_B1_06_LPUART3_TX},
    },
    {
        // UART3_RX                              #8
        .GROUP = GPIO6,
        .PIN = 23U,
        .ADC = ADC2,
        .adcChannel = 12,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_07_GPIO1_IO23},
        .FUN_UART = {IOMUXC_GPIO_AD_B1_07_LPUART3_RX},
    },
    ///-------------------- SPI3 --------------------
    {
        // SPI3_MISO                              #9
        .GROUP = GPIO1,
        .PIN = 29,
        .ADC = ADC2,
        .adcChannel = 2,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_13_GPIO1_IO29},
        .FUN_SPI = {IOMUXC_GPIO_AD_B1_13_LPSPI3_SDI},
    },
    {
        // SPI3_MOSI                              #10
        .GROUP = GPIO1,
        .PIN = 30,
        .ADC = ADC2,
        .adcChannel = 3,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_14_GPIO1_IO30},
        .FUN_SPI = {IOMUXC_GPIO_AD_B1_14_LPSPI3_SDO},
    },
    {
        // SPI3_CLK                              #11
        .GROUP = GPIO1,
        .PIN = 31,
        .ADC = ADC2,
        .adcChannel = 4,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_15_GPIO1_IO31},
        .FUN_SPI = {IOMUXC_GPIO_AD_B1_15_LPSPI3_SCK},
    },
    {
        // SPI3_CS0                              #12
        .GROUP = GPIO1,
        .PIN = 28,
        .ADC = ADC2,
        .adcChannel = 1,
        .FUN_GPIO = {IOMUXC_GPIO_AD_B1_12_GPIO1_IO28},
        .FUN_SPI = {IOMUXC_GPIO_AD_B1_12_LPSPI3_PCS0},
    },
    ///-------------------- I2C1 --------------------
    {
        // I2C1_SCL                              #13
        .GROUP = GPIO3,
        .PIN = 04,
        .FUN_GPIO = {IOMUXC_GPIO_SD_B1_04_GPIO3_IO04},
        .FUN_I2C = {IOMUXC_GPIO_SD_B1_04_LPI2C1_SCL},
    },
    {
        // I2C1_SDA                              #14
        .GROUP = GPIO3,
        .PIN = 05,
        .FUN_GPIO = {IOMUXC_GPIO_SD_B1_05_GPIO3_IO05},
        .FUN_I2C = {IOMUXC_GPIO_SD_B1_05_LPI2C1_SDA},
    },
    ///-------------------- EXT PWM --------------------
    {
        // EXT_INT1                              #15    !!!!Deprecated
        .GROUP = GPIO3,
        .PIN = 03,
        .FUN_GPIO = {IOMUXC_GPIO_SD_B1_03_GPIO3_IO03},
        .PWM = PWM2,
        .pwm_submodule = kPWM_Module_3,
        .pwm_channel = kPWM_PwmB,
        .FUN_PWM = {IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03},
    },
    {
        // EXT_INT0                              #16
        .GROUP = GPIO3,
        .PIN = 02,
        .FUN_GPIO = {IOMUXC_GPIO_SD_B1_02_GPIO3_IO02},
        .PWM = PWM2,
        .pwm_submodule = kPWM_Module_3,
        .pwm_channel = kPWM_PwmA,
        .FUN_PWM = {IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03},
    },
};
