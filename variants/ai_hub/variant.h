#ifndef PINS_ARDUINO_ARCH_MIX
#define PINS_ARDUINO_ARCH_MIX

#include "WVariant.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "MIMXRT1064.h"

#define USB_VID 0x2CA3U
#define USB_PID 0x6001U

#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////
///                 14PIN
/////////////////////////////////////////////////////
#define JP_0   7U           // UART3_TX
#define JP_1   8U           // UART3_RX
#define JP_2   9U           // SPI3_MISO
#define JP_3   15U          // EXT_INT1 !!!!Deprecated
#define JP_4   11U          // SPI3_CLK

#define JP_5   13U          // I2C1_SCL
#define JP_6   14U          // I2C1_SDA
#define JP_7   10U          // SPI3_MOSI
#define JP_8   16U          // EXT_INT0
#define JP_9   12U          // SPI3_CS0

/////////////////////////////////////////////////////
///                 KEY
/////////////////////////////////////////////////////
#define USER_KEY 0U

/////////////////////////////////////////////////////
///                 LED
/////////////////////////////////////////////////////
#define USER_LED_1 1U
#define USER_LED_2 2U
#define USER_LED_3 3U
#define USER_LED_4 4U
#define LED_BUILTIN USER_LED_1

/////////////////////////////////////////////////////
///                 UART
/////////////////////////////////////////////////////
#define UART_INTERFACES_COUNT 1

#define PIN_UART1_RX      JP_1
#define PIN_UART1_TX      JP_0
#define UART1_NUM         LPUART3
#define UART1_IRQn        LPUART3_IRQn
#define UART1_HANDLER     LPUART3_IRQHandler

// #define PIN_UART1_RX      5U
// #define PIN_UART1_TX      6U
// #define UART1_NUM         LPUART1
// #define UART1_IRQn        LPUART1_IRQn
// #define UART1_HANDLER     LPUART1_IRQHandler

// #define PIN_UART2_RX      0U
// #define PIN_UART2_TX      0U
// #define UART2_NUM         LPUART8
// #define UART2_IRQn        LPUART8_IRQn
// #define UART2_HANDLER     LPUART8_IRQHandler

// #define PIN_UART3_RX      JP_1
// #define PIN_UART3_TX      JP_0
// #define UART3_NUM         LPUART3
// #define UART3_IRQn        LPUART3_IRQn
// #define UART3_HANDLER     LPUART3_IRQHandler

/////////////////////////////////////////////////////
///                 I2C
/////////////////////////////////////////////////////
#define I2C1        LPI2C1
#define I2C1_SCL    JP_5
#define I2C1_SDA    JP_6
// default I2C PORT
#define SCL         I2C1_SCL
#define SDA         I2C1_SDA

/////////////////////////////////////////////////////
///                 SPI
/////////////////////////////////////////////////////
#define SPI3_MOSI   JP_7
#define SPI3_MISO   JP_2
#define SPI3_SCK    JP_4
#define SPI3_CS     JP_9

#define MOSI        SPI3_MOSI
#define MISO        SPI3_MISO
#define SCK         SPI3_SCK

/////////////////////////////////////////////////////
///                 OTHER
/////////////////////////////////////////////////////

#define A0  JP_0
#define A1  JP_1
#define A2  JP_2
#define A3  JP_4
#define A4  JP_7
#define A5  JP_9

#define PWM0 JP_8

#ifdef __cplusplus
}
#endif


#endif /*end of  PINS_ARDUINO_ARCH_MIX*/