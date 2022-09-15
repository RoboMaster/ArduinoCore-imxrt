/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v10.0
processor: MIMXRT1064xxxxA
package_id: MIMXRT1064DVL6A
mcu_data: ksdk2_0
processor_version: 10.0.0
board: MIMXRT1064-EVK
pin_labels:
- {pin_num: H3, pin_signal: GPIO_EMC_08, label: USER_KEY, identifier: SEMC_DM0;evkmimxrt1064_freertos_hello2;USER_KEY}
- {pin_num: E5, pin_signal: GPIO_EMC_35, label: SD_CD_SW, identifier: SEMC_D13;SD_CD_SW}
- {pin_num: C14, pin_signal: GPIO_B1_14, label: SD_CD_SW, identifier: SD0_VSELECT;SD_CD_SW}
- {pin_num: B14, pin_signal: GPIO_B1_15, label: USER_KEY, identifier: BACKLIGHT_CTL;USER_KEY}
- {pin_num: M11, pin_signal: GPIO_AD_B0_02, label: LED4, identifier: LED4}
- {pin_num: F14, pin_signal: GPIO_AD_B0_09, label: LED1, identifier: LED1}
- {pin_num: G13, pin_signal: GPIO_AD_B0_10, label: LED2, identifier: INT1_COMBO;LED2}
- {pin_num: G10, pin_signal: GPIO_AD_B0_11, label: LED3, identifier: INT2_COMBO;LED3}
- {pin_num: H12, pin_signal: GPIO_AD_B1_12, label: SPI3_CS0, identifier: CSI_D5;SPI3_CS0}
- {pin_num: H11, pin_signal: GPIO_AD_B1_13, label: SPI3_MISO, identifier: CSI_D4;SPI3_MISO}
- {pin_num: G12, pin_signal: GPIO_AD_B1_14, label: SPI3_MOSI, identifier: CSI_D3;SPI3_MOSI}
- {pin_num: J14, pin_signal: GPIO_AD_B1_15, label: SPI3_CLK, identifier: CSI_D2;SPI3_CLK}
- {pin_num: M3, pin_signal: GPIO_SD_B1_02, label: EXT_INT0, identifier: FlexSPI_D1_B;EXT_INT0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L14, peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_13, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
  - {pin_num: K14, peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_12, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
  - {pin_num: M11, peripheral: GPIO6, signal: 'gpio_io, 02', pin_signal: GPIO_AD_B0_02, direction: OUTPUT}
  - {pin_num: F14, peripheral: GPIO6, signal: 'gpio_io, 09', pin_signal: GPIO_AD_B0_09, direction: OUTPUT}
  - {pin_num: G13, peripheral: GPIO6, signal: 'gpio_io, 10', pin_signal: GPIO_AD_B0_10, identifier: LED2, direction: OUTPUT}
  - {pin_num: G10, peripheral: GPIO6, signal: 'gpio_io, 11', pin_signal: GPIO_AD_B0_11, identifier: LED3, direction: OUTPUT}
  - {pin_num: M8, peripheral: USB1, signal: usb_otg1_dn, pin_signal: USB_OTG1_DN}
  - {pin_num: L8, peripheral: USB1, signal: usb_otg1_dp, pin_signal: USB_OTG1_DP}
  - {pin_num: C3, peripheral: CAN3, signal: TX, pin_signal: GPIO_EMC_36, drive_strength: R0, slew_rate: Fast}
  - {pin_num: E4, peripheral: CAN3, signal: RX, pin_signal: GPIO_EMC_37, drive_strength: R0, slew_rate: Fast}
  - {pin_num: J4, peripheral: USDHC1, signal: usdhc_cmd, pin_signal: GPIO_SD_B0_00, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: J3, peripheral: USDHC1, signal: usdhc_clk, pin_signal: GPIO_SD_B0_01, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: J1, peripheral: USDHC1, signal: 'usdhc_data, 0', pin_signal: GPIO_SD_B0_02, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: K1, peripheral: USDHC1, signal: 'usdhc_data, 1', pin_signal: GPIO_SD_B0_03, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: H2, peripheral: USDHC1, signal: 'usdhc_data, 2', pin_signal: GPIO_SD_B0_04, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: J2, peripheral: USDHC1, signal: 'usdhc_data, 3', pin_signal: GPIO_SD_B0_05, pull_up_down_config: Pull_Up_47K_Ohm, pull_keeper_select: Pull, drive_strength: R0,
    slew_rate: Fast}
  - {pin_num: G12, peripheral: LPSPI3, signal: SDO, pin_signal: GPIO_AD_B1_14, identifier: SPI3_MOSI, drive_strength: R0, slew_rate: Fast}
  - {pin_num: J14, peripheral: LPSPI3, signal: SCK, pin_signal: GPIO_AD_B1_15, identifier: SPI3_CLK, drive_strength: R0, slew_rate: Fast}
  - {pin_num: H11, peripheral: LPSPI3, signal: SDI, pin_signal: GPIO_AD_B1_13, identifier: SPI3_MISO, drive_strength: R0, slew_rate: Fast}
  - {pin_num: K10, peripheral: LPUART3, signal: RX, pin_signal: GPIO_AD_B1_07, speed: MHZ_200, slew_rate: Fast}
  - {pin_num: J12, peripheral: LPUART3, signal: TX, pin_signal: GPIO_AD_B1_06, speed: MHZ_200, slew_rate: Fast}
  - {pin_num: B14, peripheral: GPIO2, signal: 'gpio_io, 31', pin_signal: GPIO_B1_15, identifier: USER_KEY, direction: INPUT, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: no_init, slew_rate: Fast}
  - {pin_num: C14, peripheral: GPIO2, signal: 'gpio_io, 30', pin_signal: GPIO_B1_14, identifier: SD_CD_SW, direction: INPUT, pull_up_down_config: Pull_Up_100K_Ohm,
    pull_keeper_select: no_init, slew_rate: Fast}
  - {pin_num: H12, peripheral: GPIO1, signal: 'gpio_io, 28', pin_signal: GPIO_AD_B1_12, identifier: SPI3_CS0, direction: INPUT, gpio_interrupt: kGPIO_IntFallingEdge,
    pull_up_down_config: Pull_Up_100K_Ohm, slew_rate: Fast}
  - {pin_num: M3, peripheral: GPIO3, signal: 'gpio_io, 02', pin_signal: GPIO_SD_B1_02, identifier: EXT_INT0, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);

#if (EXT_FEATURE_MODE == 3) || (EXT_FEATURE_MODE == 4)
  /* GPIO configuration of SPI3_CS0 on GPIO_AD_B1_12 (pin H12) */
  gpio_pin_config_t SPI3_CS0_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_IntFallingEdge
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_12 (pin H12) */
  GPIO_PinInit(GPIO1, 28U, &SPI3_CS0_config);
  /* Enable GPIO pin interrupt on GPIO_AD_B1_12 (pin H12) */
  GPIO_PortEnableInterrupts(GPIO1, 1U << 28U);

  /* GPIO configuration of EXT_INT0 on GPIO_SD_B1_02 (pin M3) */
  gpio_pin_config_t EXT_INT0_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B1_02 (pin M3) */
  GPIO_PinInit(GPIO3, 2U, &EXT_INT0_config);
#endif

#if (EXT_FEATURE_MODE == 3) || (EXT_FEATURE_MODE == 4)
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_LPUART3_TX, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_LPUART3_RX, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0U); // SPI3_CS0
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_LPSPI3_SDI, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_LPSPI3_SDO, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_LPSPI3_SCK, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30, 0U); // 不知作用
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0U); // 不知作用
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_02_GPIO3_IO02, 0U);
#endif

#if (EXT_FEATURE_MODE == 2) || (EXT_FEATURE_MODE == 4)
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_36_FLEXCAN3_TX, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_37_FLEXCAN3_RX, 0U);
#endif

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0U);

  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL_MASK)))
      | IOMUXC_GPR_GPR26_GPIO_MUX1_GPIO_SEL(0x00U) //0E04U
    );
  IOMUXC_GPR->GPR27 = ((IOMUXC_GPR->GPR27 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL_MASK)))
      | IOMUXC_GPR_GPR27_GPIO_MUX2_GPIO_SEL(0x00U)
    );

  IOMUXC_GPR->GPR28 = ((IOMUXC_GPR->GPR28 &
    (~(BOARD_INITPINS_IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL_MASK)))
      | IOMUXC_GPR_GPR28_GPIO_MUX3_GPIO_SEL(0x00U)
    );

#if (EXT_FEATURE_MODE == 3) || (EXT_FEATURE_MODE == 4)
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_LPUART3_TX, 0x10F1U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_LPUART3_RX, 0x10F1U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0x90B1U);// SPI3_CS0
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_LPSPI3_SDI, 0x1089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_LPSPI3_SDO, 0x1089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_LPSPI3_SCK, 0x1089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30, 0x90B1U); // 不知作用
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0x90B1U); // 不知作用
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, 0x7089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, 0x7089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0x7089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0x7089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0x7089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0x7089U);
#endif

#if (EXT_FEATURE_MODE == 2) || (EXT_FEATURE_MODE == 4)
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_36_FLEXCAN3_TX, 0x1089U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_37_FLEXCAN3_RX, 0x1089U);
#endif

  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX, 0x10B0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX, 0x10B0U);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

  // /* GPIO configuration of SD_CD_SW on GPIO_B1_14 (pin C14) */
  // gpio_pin_config_t SD_CD_SW_config = {
  //     .direction = kGPIO_DigitalInput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_B1_14 (pin C14) */
  // GPIO_PinInit(GPIO2, 30U, &SD_CD_SW_config);

  // /* GPIO configuration of USER_KEY on GPIO_B1_15 (pin B14) */
  // gpio_pin_config_t USER_KEY_config = {
  //     .direction = kGPIO_DigitalInput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_B1_15 (pin B14) */
  // GPIO_PinInit(GPIO2, 31U, &USER_KEY_config);



  // /* GPIO configuration of LED4 on GPIO_AD_B0_02 (pin M11) */
  // gpio_pin_config_t LED4_config = {
  //     .direction = kGPIO_DigitalOutput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_AD_B0_02 (pin M11) */
  // GPIO_PinInit(GPIO6, 2U, &LED4_config);

  // /* GPIO configuration of LED1 on GPIO_AD_B0_09 (pin F14) */
  // gpio_pin_config_t LED1_config = {
  //     .direction = kGPIO_DigitalOutput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_AD_B0_09 (pin F14) */
  // GPIO_PinInit(GPIO6, 9U, &LED1_config);

  // /* GPIO configuration of LED2 on GPIO_AD_B0_10 (pin G13) */
  // gpio_pin_config_t LED2_config = {
  //     .direction = kGPIO_DigitalOutput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_AD_B0_10 (pin G13) */
  // GPIO_PinInit(GPIO6, 10U, &LED2_config);

  // /* GPIO configuration of LED3 on GPIO_AD_B0_11 (pin G10) */
  // gpio_pin_config_t LED3_config = {
  //     .direction = kGPIO_DigitalOutput,
  //     .outputLogic = 0U,
  //     .interruptMode = kGPIO_NoIntmode
  // };
  // /* Initialize GPIO functionality on GPIO_AD_B0_11 (pin G10) */
  // GPIO_PinInit(GPIO6, 11U, &LED3_config);

  // IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_GPIO1_IO02, 0U);
  // IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0U);
  // IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, 0U);
  // IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_11_GPIO1_IO11, 0U);