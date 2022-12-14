/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK 0x10C00E04U /*!< GPIO1 and GPIO6 share same IO MUX function, GPIO_MUX1 selects one GPIO function: affected bits mask */
#define BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK 0xC0000000U /*!< GPIO2 and GPIO7 share same IO MUX function, GPIO_MUX2 selects one GPIO function: affected bits mask */
#define BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK 0x04U /*!< GPIO3 and GPIO8 share same IO MUX function, GPIO_MUX3 selects one GPIO function: affected bits mask */

/* GPIO_AD_B0_13 (coord L14), UART1_RXD */
/* Routed pin properties */
#define BOARD_INITPINS_UART1_RXD_PERIPHERAL                              LPUART1   /*!< Peripheral name */
#define BOARD_INITPINS_UART1_RXD_SIGNAL                                       RX   /*!< Signal name */

/* GPIO_AD_B0_12 (coord K14), UART1_TXD */
/* Routed pin properties */
#define BOARD_INITPINS_UART1_TXD_PERIPHERAL                              LPUART1   /*!< Peripheral name */
#define BOARD_INITPINS_UART1_TXD_SIGNAL                                       TX   /*!< Signal name */

/* GPIO_AD_B0_02 (coord M11), LED4 */
/* Routed pin properties */
#define BOARD_INITPINS_LED4_PERIPHERAL                                     GPIO6   /*!< Peripheral name */
#define BOARD_INITPINS_LED4_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED4_CHANNEL                                           2U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED4_GPIO                                           GPIO6   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED4_GPIO_PIN                                          2U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED4_GPIO_PIN_MASK                             (1U << 2U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_LED4_PORT                                           GPIO6   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_LED4_PIN                                               2U   /*!< PORT pin number */
#define BOARD_INITPINS_LED4_PIN_MASK                                  (1U << 2U)   /*!< PORT pin mask */

/* GPIO_AD_B0_09 (coord F14), LED1 */
/* Routed pin properties */
#define BOARD_INITPINS_LED1_PERIPHERAL                                     GPIO6   /*!< Peripheral name */
#define BOARD_INITPINS_LED1_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED1_CHANNEL                                           9U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED1_GPIO                                           GPIO6   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED1_GPIO_PIN                                          9U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED1_GPIO_PIN_MASK                             (1U << 9U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_LED1_PORT                                           GPIO6   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_LED1_PIN                                               9U   /*!< PORT pin number */
#define BOARD_INITPINS_LED1_PIN_MASK                                  (1U << 9U)   /*!< PORT pin mask */

/* GPIO_AD_B0_10 (coord G13), LED2 */
/* Routed pin properties */
#define BOARD_INITPINS_LED2_PERIPHERAL                                     GPIO6   /*!< Peripheral name */
#define BOARD_INITPINS_LED2_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED2_CHANNEL                                          10U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED2_GPIO                                           GPIO6   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED2_GPIO_PIN                                         10U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED2_GPIO_PIN_MASK                            (1U << 10U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_LED2_PORT                                           GPIO6   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_LED2_PIN                                              10U   /*!< PORT pin number */
#define BOARD_INITPINS_LED2_PIN_MASK                                 (1U << 10U)   /*!< PORT pin mask */

/* GPIO_AD_B0_11 (coord G10), LED3 */
/* Routed pin properties */
#define BOARD_INITPINS_LED3_PERIPHERAL                                     GPIO6   /*!< Peripheral name */
#define BOARD_INITPINS_LED3_SIGNAL                                       gpio_io   /*!< Signal name */
#define BOARD_INITPINS_LED3_CHANNEL                                          11U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED3_GPIO                                           GPIO6   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_LED3_GPIO_PIN                                         11U   /*!< GPIO pin number */
#define BOARD_INITPINS_LED3_GPIO_PIN_MASK                            (1U << 11U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_LED3_PORT                                           GPIO6   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_LED3_PIN                                              11U   /*!< PORT pin number */
#define BOARD_INITPINS_LED3_PIN_MASK                                 (1U << 11U)   /*!< PORT pin mask */

/* USB_OTG1_DN (coord M8), OTG1_DN */
/* Routed pin properties */
#define BOARD_INITPINS_OTG1_DN_PERIPHERAL                                   USB1   /*!< Peripheral name */
#define BOARD_INITPINS_OTG1_DN_SIGNAL                                usb_otg1_dn   /*!< Signal name */

/* USB_OTG1_DP (coord L8), OTG1_DP */
/* Routed pin properties */
#define BOARD_INITPINS_OTG1_DP_PERIPHERAL                                   USB1   /*!< Peripheral name */
#define BOARD_INITPINS_OTG1_DP_SIGNAL                                usb_otg1_dp   /*!< Signal name */

/* GPIO_EMC_36 (coord C3), SEMC_D14 */
/* Routed pin properties */
#define BOARD_INITPINS_SEMC_D14_PERIPHERAL                                  CAN3   /*!< Peripheral name */
#define BOARD_INITPINS_SEMC_D14_SIGNAL                                        TX   /*!< Signal name */

/* GPIO_EMC_37 (coord E4), SEMC_D15 */
/* Routed pin properties */
#define BOARD_INITPINS_SEMC_D15_PERIPHERAL                                  CAN3   /*!< Peripheral name */
#define BOARD_INITPINS_SEMC_D15_SIGNAL                                        RX   /*!< Signal name */

/* GPIO_SD_B0_00 (coord J4), SD1_CMD/J24[6] */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_CMD_PERIPHERAL                                 USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_CMD_SIGNAL                                  usdhc_cmd   /*!< Signal name */

/* GPIO_SD_B0_01 (coord J3), SD1_CLK/J24[3] */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_CLK_PERIPHERAL                                 USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_CLK_SIGNAL                                  usdhc_clk   /*!< Signal name */

/* GPIO_SD_B0_02 (coord J1), SD1_D0/J24[4]/SPI_MOSI/PWM */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_D0_PERIPHERAL                                  USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_D0_SIGNAL                                  usdhc_data   /*!< Signal name */
#define BOARD_INITPINS_SD1_D0_CHANNEL                                         0U   /*!< Signal channel */

/* GPIO_SD_B0_03 (coord K1), SD1_D1/J24[5]/SPI_MISO */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_D1_PERIPHERAL                                  USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_D1_SIGNAL                                  usdhc_data   /*!< Signal name */
#define BOARD_INITPINS_SD1_D1_CHANNEL                                         1U   /*!< Signal channel */

/* GPIO_SD_B0_04 (coord H2), SD1_D2 */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_D2_PERIPHERAL                                  USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_D2_SIGNAL                                  usdhc_data   /*!< Signal name */
#define BOARD_INITPINS_SD1_D2_CHANNEL                                         2U   /*!< Signal channel */

/* GPIO_SD_B0_05 (coord J2), SD1_D3 */
/* Routed pin properties */
#define BOARD_INITPINS_SD1_D3_PERIPHERAL                                  USDHC1   /*!< Peripheral name */
#define BOARD_INITPINS_SD1_D3_SIGNAL                                  usdhc_data   /*!< Signal name */
#define BOARD_INITPINS_SD1_D3_CHANNEL                                         3U   /*!< Signal channel */

/* GPIO_AD_B1_14 (coord G12), SPI3_MOSI */
/* Routed pin properties */
#define BOARD_INITPINS_SPI3_MOSI_PERIPHERAL                               LPSPI3   /*!< Peripheral name */
#define BOARD_INITPINS_SPI3_MOSI_SIGNAL                                      SDO   /*!< Signal name */

/* GPIO_AD_B1_15 (coord J14), SPI3_CLK */
/* Routed pin properties */
#define BOARD_INITPINS_SPI3_CLK_PERIPHERAL                                LPSPI3   /*!< Peripheral name */
#define BOARD_INITPINS_SPI3_CLK_SIGNAL                                       SCK   /*!< Signal name */

/* GPIO_AD_B1_13 (coord H11), SPI3_MISO */
/* Routed pin properties */
#define BOARD_INITPINS_SPI3_MISO_PERIPHERAL                               LPSPI3   /*!< Peripheral name */
#define BOARD_INITPINS_SPI3_MISO_SIGNAL                                      SDI   /*!< Signal name */

/* GPIO_AD_B1_07 (coord K10), CSI_HSYNC/J35[16]/J22[1]/UART_RX */
/* Routed pin properties */
#define BOARD_INITPINS_CSI_HSYNC_PERIPHERAL                              LPUART3   /*!< Peripheral name */
#define BOARD_INITPINS_CSI_HSYNC_SIGNAL                                       RX   /*!< Signal name */

/* GPIO_AD_B1_06 (coord J12), CSI_VSYNC/J35[18]/J22[2]/UART_TX */
/* Routed pin properties */
#define BOARD_INITPINS_CSI_VSYNC_PERIPHERAL                              LPUART3   /*!< Peripheral name */
#define BOARD_INITPINS_CSI_VSYNC_SIGNAL                                       TX   /*!< Signal name */

/* GPIO_B1_15 (coord B14), USER_KEY */
/* Routed pin properties */
#define BOARD_INITPINS_USER_KEY_PERIPHERAL                                 GPIO2   /*!< Peripheral name */
#define BOARD_INITPINS_USER_KEY_SIGNAL                                   gpio_io   /*!< Signal name */
#define BOARD_INITPINS_USER_KEY_CHANNEL                                      31U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_USER_KEY_GPIO                                       GPIO2   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_USER_KEY_GPIO_PIN                                     31U   /*!< GPIO pin number */
#define BOARD_INITPINS_USER_KEY_GPIO_PIN_MASK                        (1U << 31U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_USER_KEY_PORT                                       GPIO2   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_USER_KEY_PIN                                          31U   /*!< PORT pin number */
#define BOARD_INITPINS_USER_KEY_PIN_MASK                             (1U << 31U)   /*!< PORT pin mask */

/* GPIO_B1_14 (coord C14), SD_CD_SW */
/* Routed pin properties */
#define BOARD_INITPINS_SD_CD_SW_PERIPHERAL                                 GPIO2   /*!< Peripheral name */
#define BOARD_INITPINS_SD_CD_SW_SIGNAL                                   gpio_io   /*!< Signal name */
#define BOARD_INITPINS_SD_CD_SW_CHANNEL                                      30U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_SD_CD_SW_GPIO                                       GPIO2   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_SD_CD_SW_GPIO_PIN                                     30U   /*!< GPIO pin number */
#define BOARD_INITPINS_SD_CD_SW_GPIO_PIN_MASK                        (1U << 30U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_SD_CD_SW_PORT                                       GPIO2   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_SD_CD_SW_PIN                                          30U   /*!< PORT pin number */
#define BOARD_INITPINS_SD_CD_SW_PIN_MASK                             (1U << 30U)   /*!< PORT pin mask */

/* GPIO_AD_B1_12 (coord H12), SPI3_CS0 */
/* Routed pin properties */
#define BOARD_INITPINS_SPI3_CS0_PERIPHERAL                                 GPIO1   /*!< Peripheral name */
#define BOARD_INITPINS_SPI3_CS0_SIGNAL                                   gpio_io   /*!< Signal name */
#define BOARD_INITPINS_SPI3_CS0_CHANNEL                                      28U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_SPI3_CS0_GPIO                                       GPIO1   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_SPI3_CS0_GPIO_PIN                                     28U   /*!< GPIO pin number */
#define BOARD_INITPINS_SPI3_CS0_GPIO_PIN_MASK                        (1U << 28U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_SPI3_CS0_PORT                                       GPIO1   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_SPI3_CS0_PIN                                          28U   /*!< PORT pin number */
#define BOARD_INITPINS_SPI3_CS0_PIN_MASK                             (1U << 28U)   /*!< PORT pin mask */

/* GPIO_SD_B1_02 (coord M3), EXT_INT0 */
/* Routed pin properties */
#define BOARD_INITPINS_EXT_INT0_PERIPHERAL                                 GPIO3   /*!< Peripheral name */
#define BOARD_INITPINS_EXT_INT0_SIGNAL                                   gpio_io   /*!< Signal name */
#define BOARD_INITPINS_EXT_INT0_CHANNEL                                       2U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_EXT_INT0_GPIO                                       GPIO3   /*!< GPIO peripheral base pointer */
#define BOARD_INITPINS_EXT_INT0_GPIO_PIN                                      2U   /*!< GPIO pin number */
#define BOARD_INITPINS_EXT_INT0_GPIO_PIN_MASK                         (1U << 2U)   /*!< GPIO pin mask */
#define BOARD_INITPINS_EXT_INT0_PORT                                       GPIO3   /*!< PORT peripheral base pointer */
#define BOARD_INITPINS_EXT_INT0_PIN                                           2U   /*!< PORT pin number */
#define BOARD_INITPINS_EXT_INT0_PIN_MASK                              (1U << 2U)   /*!< PORT pin mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
