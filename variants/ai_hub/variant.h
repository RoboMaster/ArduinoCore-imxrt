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
#define IO8     7U           // UART3_TX
#define IO7     8U           // UART3_RX
#define IO6     9U           // SPI3_MISO
// #define P3   15U          // EXT_INT1 !!!!Deprecated
#define IO5     11U          // SPI3_CLK

#define IO4     13U          // I2C1_SCL
#define IO3     14U          // I2C1_SDA
#define IO2     10U          // SPI3_MOSI
#define IO_RES  16U          // EXT_INT0
#define IO1     12U          // SPI3_CS0

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

#define PIN_UART1_RX      IO7
#define PIN_UART1_TX      IO8
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

// #define PIN_UART3_RX      IO7
// #define PIN_UART3_TX      IO8
// #define UART3_NUM         LPUART3
// #define UART3_IRQn        LPUART3_IRQn
// #define UART3_HANDLER     LPUART3_IRQHandler

/////////////////////////////////////////////////////
///                 I2C
/////////////////////////////////////////////////////
#define I2C1        LPI2C1
#define I2C1_SCL    IO4
#define I2C1_SDA    IO3
// default I2C PORT
#define SCL         I2C1_SCL
#define SDA         I2C1_SDA

/////////////////////////////////////////////////////
///                 SPI
/////////////////////////////////////////////////////
#define SPI3_MOSI   IO2
#define SPI3_MISO   IO6
#define SPI3_SCK    IO5
#define SPI3_CS     IO1

#define MOSI        SPI3_MOSI
#define MISO        SPI3_MISO
#define SCK         SPI3_SCK

/////////////////////////////////////////////////////
///                 OTHER
/////////////////////////////////////////////////////

#define A0  IO8
#define A1  IO7
#define A2  IO6
#define A3  IO5
#define A4  IO2
#define A5  IO1

#define PWM0 IO_RES

#ifdef __cplusplus
}
#endif


#endif /*end of  PINS_ARDUINO_ARCH_MIX*/