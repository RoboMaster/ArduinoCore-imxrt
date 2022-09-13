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
#define P0   7U           // UART3_TX
#define P1   8U           // UART3_RX
#define P2   9U           // SPI3_MISO
#define P3   15U          // EXT_INT1 !!!!Deprecated
#define P4   11U          // SPI3_CLK

#define P5   13U          // I2C1_SCL
#define P6   14U          // I2C1_SDA
#define P7   10U          // SPI3_MOSI
#define P8   16U          // EXT_INT0
#define P9   12U          // SPI3_CS0

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
#define USART1

#define PIN_UART1_RX      P1
#define PIN_UART1_TX      P0
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

// #define PIN_UART3_RX      P1
// #define PIN_UART3_TX      P0
// #define UART3_NUM         LPUART3
// #define UART3_IRQn        LPUART3_IRQn
// #define UART3_HANDLER     LPUART3_IRQHandler

/////////////////////////////////////////////////////
///                 I2C
/////////////////////////////////////////////////////
#define I2C1        LPI2C1
#define I2C1_SCL    P5
#define I2C1_SDA    P6
// default I2C PORT
#define SCL         I2C1_SCL
#define SDA         I2C1_SDA

/////////////////////////////////////////////////////
///                 SPI
/////////////////////////////////////////////////////
#define SPI3_MOSI   P7
#define SPI3_MISO   P2
#define SPI3_SCK    P4
#define SPI3_CS     P9

#define MOSI        SPI3_MOSI
#define MISO        SPI3_MISO
#define SCK         SPI3_SCK

/////////////////////////////////////////////////////
///                 OTHER
/////////////////////////////////////////////////////

#define A0  P0
#define A1  P1
#define A2  P2
#define A3  P4
#define A4  P7
#define A5  P9

#define PWM0 P8

#ifdef __cplusplus
}
#endif


#endif /*end of  PINS_ARDUINO_ARCH_MIX*/