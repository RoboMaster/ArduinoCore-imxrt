/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v10.0
processor: MIMXRT1064xxxxA
package_id: MIMXRT1064DVL6A
mcu_data: ksdk2_0
processor_version: 10.0.0
board: MIMXRT1064-EVK
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: 5cde0fa3-f380-4be3-b6d1-0e475d443615
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system_54b53072540eeeb8f8e9343e71f28176'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DMA0 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'DMA0'
- type: 'edma'
- mode: 'basic'
- custom_name_enabled: 'false'
- type_id: 'edma_6d0dd4e17e2f179c7d42d98767129ede'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'DMA0'
- config_sets:
  - fsl_edma:
    - common_settings:
      - enableContinuousLinkMode: 'false'
      - enableHaltOnError: 'true'
      - enableRoundRobinArbitration: 'false'
      - enableDebugMode: 'false'
    - dma_table:
      - 0: []
      - 1: []
      - 2: []
      - 3: []
      - 4: []
    - edma_channels: []
    - errInterruptConfig:
      - enableErrInterrupt: 'false'
      - errorInterrupt:
        - IRQn: 'DMA_ERROR_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'false'
        - priority: '0'
        - enable_custom_name: 'false'
    - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const edma_config_t DMA0_config = {
  .enableContinuousLinkMode = false,
  .enableHaltOnError = true,
  .enableRoundRobinArbitration = false,
  .enableDebugMode = false
};

/* Empty initialization function (commented out)
static void DMA0_init(void) {
} */

/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic_57b5eef3774cc60acaede6f5b8bddc67'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
      - 1: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void NVIC_init(void) {
} */

/***********************************************************************************************************************
 * LPUART3 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'LPUART3'
- type: 'lpuart'
- mode: 'edma'
- custom_name_enabled: 'false'
- type_id: 'lpuart_bf01db7d964092f3cf860852cba17f7e'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPUART3'
- config_sets:
  - lpuartConfig_t:
    - lpuartConfig:
      - clockSource: 'LpuartClock'
      - lpuartSrcClkFreq: 'BOARD_BootClockRUN'
      - baudRate_Bps: '1500000'
      - parityMode: 'kLPUART_ParityDisabled'
      - dataBitsCount: 'kLPUART_EightDataBits'
      - isMsb: 'false'
      - stopBitCount: 'kLPUART_OneStopBit'
      - txFifoWatermark: '0'
      - rxFifoWatermark: '1'
      - enableRxRTS: 'false'
      - enableTxCTS: 'false'
      - txCtsSource: 'kLPUART_CtsSourcePin'
      - txCtsConfig: 'kLPUART_CtsSampleAtStart'
      - rxIdleType: 'kLPUART_IdleTypeStartBit'
      - rxIdleConfig: 'kLPUART_IdleCharacter1'
      - enableTx: 'true'
      - enableRx: 'true'
  - edmaCfg:
    - edma_channels:
      - enable_rx_edma_channel: 'true'
      - edma_rx_channel:
        - eDMAn: '0'
        - eDMA_source: 'kDmaRequestMuxLPUART3Rx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
      - enable_tx_edma_channel: 'true'
      - edma_tx_channel:
        - eDMAn: '1'
        - eDMA_source: 'kDmaRequestMuxLPUART3Tx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
    - lpuart_edma_handle:
      - enable_custom_name: 'true'
      - handle_custom_name: 'LPUART3_LPUART_eDMA_Handle'
      - init_callback: 'false'
      - callback_fcn: ''
      - user_data: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const lpuart_config_t LPUART3_config = {
  .baudRate_Bps = 1500000UL,
  .parityMode = kLPUART_ParityDisabled,
  .dataBitsCount = kLPUART_EightDataBits,
  .isMsb = false,
  .stopBitCount = kLPUART_OneStopBit,
  .txFifoWatermark = 0U,
  .rxFifoWatermark = 1U,
  .enableRxRTS = false,
  .enableTxCTS = false,
  .txCtsSource = kLPUART_CtsSourcePin,
  .txCtsConfig = kLPUART_CtsSampleAtStart,
  .rxIdleType = kLPUART_IdleTypeStartBit,
  .rxIdleConfig = kLPUART_IdleCharacter1,
  .enableTx = true,
  .enableRx = true
};
edma_handle_t LPUART3_RX_Handle;
edma_handle_t LPUART3_TX_Handle;
lpuart_edma_handle_t LPUART3_LPUART_eDMA_Handle;

static void LPUART3_init(void) {
  LPUART_Init(LPUART3_PERIPHERAL, &LPUART3_config, LPUART3_CLOCK_SOURCE);
  /* Set the source kDmaRequestMuxLPUART3Rx request in the DMAMUX */
  DMAMUX_SetSource(LPUART3_RX_DMAMUX_BASEADDR, LPUART3_RX_DMA_CHANNEL, LPUART3_RX_DMA_REQUEST);
  /* Enable the channel 0 in the DMAMUX */
  DMAMUX_EnableChannel(LPUART3_RX_DMAMUX_BASEADDR, LPUART3_RX_DMA_CHANNEL);
  /* Set the source kDmaRequestMuxLPUART3Tx request in the DMAMUX */
  DMAMUX_SetSource(LPUART3_TX_DMAMUX_BASEADDR, LPUART3_TX_DMA_CHANNEL, LPUART3_TX_DMA_REQUEST);
  /* Enable the channel 1 in the DMAMUX */
  DMAMUX_EnableChannel(LPUART3_TX_DMAMUX_BASEADDR, LPUART3_TX_DMA_CHANNEL);
  /* Create the eDMA LPUART3_RX_Handle handle */
  EDMA_CreateHandle(&LPUART3_RX_Handle, LPUART3_RX_DMA_BASEADDR, LPUART3_RX_DMA_CHANNEL);
  /* Create the eDMA LPUART3_TX_Handle handle */
  EDMA_CreateHandle(&LPUART3_TX_Handle, LPUART3_TX_DMA_BASEADDR, LPUART3_TX_DMA_CHANNEL);
  /* Create the LPUART eDMA handle */
  LPUART_TransferCreateHandleEDMA(LPUART3_PERIPHERAL, &LPUART3_LPUART_eDMA_Handle, NULL, NULL, &LPUART3_TX_Handle, &LPUART3_RX_Handle);
}

/***********************************************************************************************************************
 * LPUART1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'LPUART1'
- type: 'lpuart'
- mode: 'edma'
- custom_name_enabled: 'false'
- type_id: 'lpuart_bf01db7d964092f3cf860852cba17f7e'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPUART1'
- config_sets:
  - lpuartConfig_t:
    - lpuartConfig:
      - clockSource: 'LpuartClock'
      - lpuartSrcClkFreq: 'BOARD_BootClockRUN'
      - baudRate_Bps: '115200'
      - parityMode: 'kLPUART_ParityDisabled'
      - dataBitsCount: 'kLPUART_EightDataBits'
      - isMsb: 'false'
      - stopBitCount: 'kLPUART_OneStopBit'
      - txFifoWatermark: '0'
      - rxFifoWatermark: '1'
      - enableRxRTS: 'false'
      - enableTxCTS: 'false'
      - txCtsSource: 'kLPUART_CtsSourcePin'
      - txCtsConfig: 'kLPUART_CtsSampleAtStart'
      - rxIdleType: 'kLPUART_IdleTypeStartBit'
      - rxIdleConfig: 'kLPUART_IdleCharacter1'
      - enableTx: 'true'
      - enableRx: 'true'
    - quick_selection: 'QuickSelection1'
  - edmaCfg:
    - edma_channels:
      - enable_rx_edma_channel: 'true'
      - edma_rx_channel:
        - eDMAn: '2'
        - eDMA_source: 'kDmaRequestMuxLPUART1Rx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
      - enable_tx_edma_channel: 'true'
      - edma_tx_channel:
        - eDMAn: '3'
        - eDMA_source: 'kDmaRequestMuxLPUART1Tx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
    - lpuart_edma_handle:
      - enable_custom_name: 'false'
      - init_callback: 'false'
      - callback_fcn: ''
      - user_data: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const lpuart_config_t LPUART1_config = {
  .baudRate_Bps = 921600UL,
  .parityMode = kLPUART_ParityDisabled,
  .dataBitsCount = kLPUART_EightDataBits,
  .isMsb = false,
  .stopBitCount = kLPUART_OneStopBit,
  .txFifoWatermark = 0U,
  .rxFifoWatermark = 1U,
  .enableRxRTS = false,
  .enableTxCTS = false,
  .txCtsSource = kLPUART_CtsSourcePin,
  .txCtsConfig = kLPUART_CtsSampleAtStart,
  .rxIdleType = kLPUART_IdleTypeStartBit,
  .rxIdleConfig = kLPUART_IdleCharacter1,
  .enableTx = true,
  .enableRx = true
};
edma_handle_t LPUART1_RX_Handle;
edma_handle_t LPUART1_TX_Handle;
lpuart_edma_handle_t LPUART1_LPUART_eDMA_Handle;

static void LPUART1_init(void) {
  LPUART_Init(LPUART1_PERIPHERAL, &LPUART1_config, LPUART1_CLOCK_SOURCE);
  /* Set the source kDmaRequestMuxLPUART1Rx request in the DMAMUX */
  DMAMUX_SetSource(LPUART1_RX_DMAMUX_BASEADDR, LPUART1_RX_DMA_CHANNEL, LPUART1_RX_DMA_REQUEST);
  /* Enable the channel 2 in the DMAMUX */
  DMAMUX_EnableChannel(LPUART1_RX_DMAMUX_BASEADDR, LPUART1_RX_DMA_CHANNEL);
  /* Set the source kDmaRequestMuxLPUART1Tx request in the DMAMUX */
  DMAMUX_SetSource(LPUART1_TX_DMAMUX_BASEADDR, LPUART1_TX_DMA_CHANNEL, LPUART1_TX_DMA_REQUEST);
  /* Enable the channel 3 in the DMAMUX */
  DMAMUX_EnableChannel(LPUART1_TX_DMAMUX_BASEADDR, LPUART1_TX_DMA_CHANNEL);
  /* Create the eDMA LPUART1_RX_Handle handle */
  EDMA_CreateHandle(&LPUART1_RX_Handle, LPUART1_RX_DMA_BASEADDR, LPUART1_RX_DMA_CHANNEL);
  /* Create the eDMA LPUART1_TX_Handle handle */
  EDMA_CreateHandle(&LPUART1_TX_Handle, LPUART1_TX_DMA_BASEADDR, LPUART1_TX_DMA_CHANNEL);
  /* Create the LPUART eDMA handle */
  LPUART_TransferCreateHandleEDMA(LPUART1_PERIPHERAL, &LPUART1_LPUART_eDMA_Handle, NULL, NULL, &LPUART1_TX_Handle, &LPUART1_RX_Handle);
}

/***********************************************************************************************************************
 * CAN3 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'CAN3'
- type: 'flexcan'
- mode: 'interrupts'
- custom_name_enabled: 'false'
- type_id: 'flexcan_b90c41c7a38dc0e2eb7f531827c85cfe'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'CAN3'
- config_sets:
  - interruptsCfg:
    - messageBufferIrqs: '0x3FF'
    - messageBufferIrqs2: '0'
    - interruptsEnable: ''
    - enable_irq: 'true'
    - interrupt_shared:
      - IRQn: 'CAN3_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
  - fsl_flexcan:
    - can_config:
      - clockSource: 'kFLEXCAN_ClkSrcOsc'
      - clockSourceFreq: 'BOARD_BootClockRUN'
      - wakeupSrc: 'kFLEXCAN_WakeupSrcUnfiltered'
      - flexibleDataRate: 'false'
      - baudRate: '1000000'
      - baudRateFD: '2000000'
      - enableBRS: 'false'
      - dataSize: 'kFLEXCAN_8BperMB'
      - maxMbNum: '16'
      - enableLoopBack: 'false'
      - enableTimerSync: 'true'
      - enableSelfWakeup: 'false'
      - enableIndividMask: 'false'
      - disableSelfReception: 'false'
      - enableListenOnlyMode: 'false'
      - enableDoze: 'false'
      - timingConfig:
        - propSeg: '2'
        - phaseSeg1: '4'
        - phaseSeg2: '3'
        - rJumpwidth: '2'
        - bitTime: []
        - fpropSeg: '2'
        - fphaseSeg1: '4'
        - fphaseSeg2: '4'
        - frJumpwidth: '2'
        - fbitTime: []
    - enableRxFIFO: 'true'
    - rxFIFO:
      - idFilterTable: 'rx_fifo_filter'
      - idFilterNum: 'num0'
      - idFilterType: 'kFLEXCAN_RxFifoFilterTypeC'
      - priority: 'kFLEXCAN_RxFifoPrioHigh'
    - channels:
      - 0:
        - mbID: '9'
        - mbType: 'mbTx'
        - rxMb:
          - id: '0'
          - format: 'kFLEXCAN_FrameFormatStandard'
          - type: 'kFLEXCAN_FrameTypeData'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const flexcan_config_t CAN3_config = {
  .clkSrc = kFLEXCAN_ClkSrc0,
  .wakeupSrc = kFLEXCAN_WakeupSrcUnfiltered,
  .bitRate = 1000000UL,
  .bitRateFD = 2000000UL,
  .maxMbNum = 16U,
  .enableLoopBack = false,
  .enableTimerSync = true,
  .enableSelfWakeup = false,
  .enableIndividMask = false,
  .disableSelfReception = false,
  .enableListenOnlyMode = false,
  .enableDoze = false,
  .timingConfig = {
    .preDivider = 3,
    .propSeg = 1,
    .phaseSeg1 = 3,
    .phaseSeg2 = 2,
    .rJumpwidth = 1
  }
};
flexcan_rx_fifo_config_t CAN3_rx_fifo_config = {
  .idFilterNum = 0,
  .idFilterType = kFLEXCAN_RxFifoFilterTypeC,
  .priority = kFLEXCAN_RxFifoPrioHigh
};

static void CAN3_init(void) {
  FLEXCAN_Init(CAN3_PERIPHERAL, &CAN3_config, CAN3_CLOCK_SOURCE);

  CAN3_rx_fifo_config.idFilterTable = rx_fifo_filter;
  FLEXCAN_SetRxFifoConfig(CAN3_PERIPHERAL, &CAN3_rx_fifo_config, true);
  /* Message buffer 9 initialization */
  FLEXCAN_SetTxMbConfig(CAN3_PERIPHERAL, 9, true);
  /* Enable FlexCAN interrupts of message buffers */
  FLEXCAN_EnableMbInterrupts(CAN3_PERIPHERAL, 0x3FFULL);
  /* Enable interrupt CAN3_IRQn request in the NVIC. */
  EnableIRQ(CAN3_FLEXCAN_IRQN);
}

/***********************************************************************************************************************
 * LPSPI3 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'LPSPI3'
- type: 'lpspi'
- mode: 'edma'
- custom_name_enabled: 'false'
- type_id: 'lpspi_6e21a1e0a09f0a012d683c4f91752db8'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPSPI3'
- config_sets:
  - main:
    - mode: 'kLPSPI_Slave'
    - clockSource: 'LpspiClock'
    - clockSourceFreq: 'BOARD_BootClockRUN'
    - slave:
      - bitsPerFrame: '8'
      - cpol: 'kLPSPI_ClockPolarityActiveHigh'
      - cpha: 'kLPSPI_ClockPhaseFirstEdge'
      - direction: 'kLPSPI_MsbFirst'
      - whichPcs: 'kLPSPI_Pcs0'
      - pcsActiveHighOrLow: 'kLPSPI_PcsActiveHigh'
      - pinCfg: 'kLPSPI_SdoInSdiOut'
      - dataOutConfig: 'kLpspiDataOutRetained'
    - allPcsPolarityEnable: 'false'
    - allPcsPolarity:
      - kLPSPI_Pcs1Active: 'kLPSPI_PcsActiveLow'
      - kLPSPI_Pcs2Active: 'kLPSPI_PcsActiveLow'
      - kLPSPI_Pcs3Active: 'kLPSPI_PcsActiveLow'
  - edma:
    - channels:
      - enableReceive: 'true'
      - receive:
        - eDMAn: '4'
        - eDMA_source: 'kDmaRequestMuxLPSPI3Rx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
      - enableTransmit: 'false'
      - transmit:
        - eDMAn: '5'
        - eDMA_source: 'kDmaRequestMuxLPSPI3Tx'
        - enableTriggerPIT: 'false'
        - init_channel_priority: 'false'
        - edma_channel_Preemption:
          - enableChannelPreemption: 'false'
          - enablePreemptAbility: 'false'
          - channelPriority: '0'
        - enable_custom_name: 'false'
    - transfer:
      - callback:
        - name: ''
        - userData: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const lpspi_slave_config_t LPSPI3_config = {
  .bitsPerFrame = 8UL,
  .cpol = kLPSPI_ClockPolarityActiveHigh,
  .cpha = kLPSPI_ClockPhaseFirstEdge,
  .direction = kLPSPI_MsbFirst,
  .whichPcs = kLPSPI_Pcs0,
  .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,
  .pinCfg = kLPSPI_SdoInSdiOut,
  .dataOutConfig = kLpspiDataOutRetained
};
edma_handle_t LPSPI3_RX_Handle;
lpspi_slave_edma_handle_t LPSPI3_handle;

static void LPSPI3_init(void) {
  LPSPI_SlaveInit(LPSPI3_PERIPHERAL, &LPSPI3_config);
  /* Set the source kDmaRequestMuxLPSPI3Rx request in the DMAMUX */
  DMAMUX_SetSource(LPSPI3_RX_DMAMUX_BASEADDR, LPSPI3_RX_DMA_CHANNEL, LPSPI3_RX_DMA_REQUEST);
  /* Enable the channel 4 in the DMAMUX */
  DMAMUX_EnableChannel(LPSPI3_RX_DMAMUX_BASEADDR, LPSPI3_RX_DMA_CHANNEL);
  /* Create the eDMA LPSPI3_RX_Handle handle */
  EDMA_CreateHandle(&LPSPI3_RX_Handle, LPSPI3_RX_DMA_BASEADDR, LPSPI3_RX_DMA_CHANNEL);
  LPSPI_SlaveTransferCreateHandleEDMA(LPSPI3_PERIPHERAL, &LPSPI3_handle, NULL, NULL, &LPSPI3_RX_Handle, NULL);
}

/***********************************************************************************************************************
 * GPIO1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'GPIO1'
- type: 'igpio'
- mode: 'GPIO'
- custom_name_enabled: 'false'
- type_id: 'igpio_b1c1fa279aa7069dca167502b8589cb7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'GPIO1'
- config_sets:
  - fsl_gpio:
    - enable_irq_comb_0_15: 'false'
    - gpio_interrupt_comb_0_15:
      - IRQn: 'GPIO1_Combined_0_15_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_comb_16_31: 'true'
    - gpio_interrupt_comb_16_31:
      - IRQn: 'GPIO1_Combined_16_31_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int0: 'false'
    - gpio_interrupt_int0:
      - IRQn: 'GPIO1_INT0_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int1: 'false'
    - gpio_interrupt_int1:
      - IRQn: 'GPIO1_INT1_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int2: 'false'
    - gpio_interrupt_int2:
      - IRQn: 'GPIO1_INT2_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int3: 'false'
    - gpio_interrupt_int3:
      - IRQn: 'GPIO1_INT3_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int4: 'false'
    - gpio_interrupt_int4:
      - IRQn: 'GPIO1_INT4_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int5: 'false'
    - gpio_interrupt_int5:
      - IRQn: 'GPIO1_INT5_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int6: 'false'
    - gpio_interrupt_int6:
      - IRQn: 'GPIO1_INT6_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - enable_irq_int7: 'false'
    - gpio_interrupt_int7:
      - IRQn: 'GPIO1_INT7_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

static void GPIO1_init(void) {
  /* Make sure, the clock gate for GPIO1 is enabled (e. g. in pin_mux.c) */
  /* Enable interrupt GPIO1_Combined_16_31_IRQn request in the NVIC. */
  EnableIRQ(GPIO1_GPIO_COMB_16_31_IRQN);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Global initialization */
  DMAMUX_Init(DMA0_DMAMUX_BASEADDR);
  EDMA_Init(DMA0_DMA_BASEADDR, &DMA0_config);

  /* Initialize components */

  LPUART1_init();
  CAN3_init();

#ifdef USE_AICAMERA
  LPUART3_init();
  LPSPI3_init();
  GPIO1_init();
#endif

}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
