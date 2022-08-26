/**
 * @file spi.h
 * @author Ryan Pan (ryan.pan@dji.com)
 * @brief
 * @version 0.1
 * @date 2022-03-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "user_driver_spi.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_edma.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern lpspi_slave_edma_handle_t LPSPI3_handle;

spi_rx_handler_t spi_rx_hander;

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void spi3_rx_dma_finish_irq(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData);
/* Exported functions --------------------------------------------------------*/

/**
 * @brief SPI 从机接受初始化
 *
 */
void spi3_slave_init(void)
{
    LPSPI3_handle.callback = &spi3_rx_dma_finish_irq;
    return;
}

/**
 * @brief SPI 从机开始接受
 *
 * @param buff
 * @param size
 * @return int
 */
int spi3_slave_start_receive(void* buff, uint32_t size)
{
    lpspi_transfer_t slaveXfer;

    slaveXfer.txData      = NULL;
    slaveXfer.rxData      = buff;
    slaveXfer.dataSize    = size;
    slaveXfer.configFlags = kLPSPI_SlavePcs0 | kLPSPI_SlaveByteSwap;

    /* 设置CS极性(设置CS信号) */
    uint32_t cfgr1Value = 0;
    cfgr1Value = LPSPI3->CFGR1 & ~(1UL << (LPSPI_CFGR1_PCSPOL_SHIFT));
    LPSPI3->CFGR1 = cfgr1Value | (kLPSPI_PcsActiveHigh << LPSPI_CFGR1_PCSPOL_SHIFT);

    LPSPI_SlaveTransferEDMA(LPSPI3, &LPSPI3_handle, &slaveXfer);
}


/**
 * @brief SPI 从机取消接收
 *
 * @param buff
 * @param size
 * @return int
 */
int spi3_slave_cancel_receive(void)
{
    /* 设置CS极性(设置CS信号) */
    uint32_t cfgr1Value = 0;
    cfgr1Value = LPSPI3->CFGR1 & ~(1UL << (LPSPI_CFGR1_PCSPOL_SHIFT));
    LPSPI3->CFGR1 = cfgr1Value | (kLPSPI_PcsActiveLow << LPSPI_CFGR1_PCSPOL_SHIFT);

    LPSPI_SlaveTransferAbortEDMA(LPSPI3, &LPSPI3_handle);
    return 0;
}

/**
 * @brief SPI 是否处于接收中
 *
 * @return int
 */
int spi3_slave_in_receive(void)
{
    return (LPSPI3_handle.state == kLPSPI_Busy);
}

/**
 * @brief SPI2从机接受完成处理函数注册
 *
 * @param handle
 */
void spi3_slave_receive_finish_handle_reg(spi_rx_handler_t handle)
{
    spi_rx_hander = handle;
}


/**
 * @brief SPI DMA接受完成中断
 *
 */
static void spi3_rx_dma_finish_irq(LPSPI_Type *base, lpspi_slave_edma_handle_t *handle, status_t status, void *userData)
{
    /* 设置极性 */
    uint32_t cfgr1Value = 0;
    cfgr1Value = LPSPI3->CFGR1 & ~(1UL << (LPSPI_CFGR1_PCSPOL_SHIFT));
    LPSPI3->CFGR1 = cfgr1Value | (kLPSPI_PcsActiveLow << LPSPI_CFGR1_PCSPOL_SHIFT);

    if(spi_rx_hander)
    {
        spi_rx_hander();
    }
}
/* Private functions ---------------------------------------------------------*/
