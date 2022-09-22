/*
 * Copyright (C) 2022 DJI.
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-27     robomaster   first version
 */

#include "can.h"

#define TX_SIZE 512
#define RX_SIZE 1024
#define FILTER_NUM sizeof(can_rx) / sizeof(ST_CAN_RX)

#define RX_FIFO_MB_IDX          (5)
#define TX_MB_IDX               (9)
static struct
{
    uint8_t buf[TX_SIZE];
    uint16_t wr;
    uint16_t rd;
} can_tx;

typedef struct
{
    uint16_t can_id;
    uint16_t wr;
    uint16_t rd;
    uint8_t buf[RX_SIZE];
} ST_CAN_RX;

ST_CAN_RX can_rx[3] =
    {
        {0x301, 0, 0, {0}},
        {0x350, 0, 0, {0}},
        {0x351, 0, 0, {0}},
};

typedef struct
{
    uint16_t std_id;
    uint8_t  dlc;
    uint8_t  data[8];
}can_frame_t;

uint8_t can_busy = 0;
uint16_t can_tx_id = 0x310;

uint8_t can_test_flag = 0;

uint32_t rx_fifo_filter_m[] =
    {
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,
        0x00000000,

};

void *rx_fifo_filter = rx_fifo_filter_m;

static int can3_recv_frame_port(can_frame_t *frame);
static void can3_send_frame_port(can_frame_t *frame);

void CAN3_IRQHandler(void)
{
    if (CAN3_PERIPHERAL->IFLAG1 & (1 << TX_MB_IDX))
    {
        can3_tx_int_handle();
        CAN3_PERIPHERAL->IFLAG1 |= ~(1 << TX_MB_IDX);
    }

    if (CAN3_PERIPHERAL->IFLAG1 & (1 << RX_FIFO_MB_IDX))
    {
        can3_rx_int_handle();
        CAN3_PERIPHERAL->IFLAG1 |= ~(1 << RX_FIFO_MB_IDX);
    }
}

void can3_init(void)
{
    FLEXCAN_SetRxFifoGlobalMask(CAN3_PERIPHERAL, 0x00000000);
}

void can3_rx_int_handle(void)
{
    can_frame_t rx_frame;
    if(can3_recv_frame_port(&rx_frame) == 0)
    {
        for (uint8_t n = 0; n < (sizeof(can_rx) / sizeof(can_rx[0])); n++)
        {
            if (rx_frame.std_id == can_rx[n].can_id)
            {
                for (uint8_t i = 0; i < rx_frame.dlc; i++)
                {
                    can_rx[n].buf[can_rx[n].wr++] = rx_frame.data[i];
                    can_rx[n].wr &= (RX_SIZE - 1);
                }
                break;
            }
        }
    }
}

void can3_tx_int_handle(void)
{
    can_frame_t tx_frame;
    uint32_t len;

    if (can_tx.rd != can_tx.wr)
    {
        len = (can_tx.wr - can_tx.rd) & (TX_SIZE - 1);
        len = (len > 8) ? 8 : len;
        for (uint8_t i = 0; i < len; i++)
        {
            tx_frame.data[i] = can_tx.buf[can_tx.rd++];
            can_tx.rd &= (TX_SIZE - 1);
        }

        tx_frame.dlc = len;
        tx_frame.std_id = can_tx_id;
        can3_send_frame_port(&tx_frame);
    }
    else
    {
        can_busy = 0;
    }
}

void can_send(uint8_t *buf, uint16_t len)
{
    uint16_t i;
    can_frame_t tx_frame;

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    for (i = 0; i < len; i++)
    {
        can_tx.buf[can_tx.wr++] = buf[i];
        can_tx.wr &= (TX_SIZE - 1);
    }

    if ((can_busy == 0) && (len > 0) && (can_tx.rd != can_tx.wr))
    {
        can_busy = 1;
        len = (can_tx.wr - can_tx.rd) & (TX_SIZE - 1);
        len = (len > 8) ? 8 : len;
        for (i = 0; i < len; i++)
        {
            tx_frame.data[i] = can_tx.buf[can_tx.rd++];
            can_tx.rd &= (TX_SIZE - 1);
        }
        tx_frame.dlc = len;
        tx_frame.std_id = can_tx_id;

        can3_send_frame_port(&tx_frame);
    }

    __set_PRIMASK(cpu_sr);
}

uint8_t get_can_state(void)
{
    return (can_busy == 0) && (can_tx.rd == can_tx.wr);
}

static __inline uint16_t can_receive_index(uint8_t n, uint8_t *buf, uint16_t buf_size)
{
    uint16_t i, len;

    unsigned long cpu_sr = __get_PRIMASK();
    __disable_irq();

    len = (can_rx[n].wr - can_rx[n].rd) & (RX_SIZE - 1);
    len = (len > buf_size) ? buf_size : len;

    for (i = 0; i < len; i++)
    {
        buf[i] = can_rx[n].buf[can_rx[n].rd++];
        can_rx[n].rd &= (RX_SIZE - 1);
    }

    __set_PRIMASK(cpu_sr);

    return len;
}

uint16_t can_receive(uint8_t *buf, uint16_t buf_size)
{
    return can_receive_index(0, buf, buf_size);
}

uint16_t can_receive0(uint8_t *buf, uint16_t buf_size)
{
    return can_receive_index(0, buf, buf_size);
}

uint16_t can_receive1(uint8_t *buf, uint16_t buf_size)
{
    return can_receive_index(1, buf, buf_size);
}

uint16_t can_receive2(uint8_t *buf, uint16_t buf_size)
{
    return can_receive_index(2, buf, buf_size);
}

void can_tx_id_set(uint16_t id)
{
    can_tx_id = id;
}

int can_test(void)
{
    if(can_test_flag == 2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static int can3_recv_frame_port(can_frame_t *frame)
{
    flexcan_frame_t rx_frame;

    if (FLEXCAN_GetMbStatusFlags(CAN3_PERIPHERAL, (uint32_t)kFLEXCAN_RxFifoFrameAvlFlag))
    {
        FLEXCAN_ReadRxFifo(CAN3_PERIPHERAL, &rx_frame);
        frame->std_id = rx_frame.id >> CAN_ID_STD_SHIFT;

        frame->dlc = rx_frame.length;

        switch(frame->dlc)
        {
            case 8:
                frame->data[7] = rx_frame.dataByte7;
            case 7:
                frame->data[6] = rx_frame.dataByte6;
            case 6:
                frame->data[5] = rx_frame.dataByte5;
            case 5:
                frame->data[4] = rx_frame.dataByte4;
            case 4:
                frame->data[3] = rx_frame.dataByte3;
            case 3:
                frame->data[2] = rx_frame.dataByte2;
            case 2:
                frame->data[1] = rx_frame.dataByte1;
            case 1:
                frame->data[0] = rx_frame.dataByte0;
            default:
                break;
        }

        /* 用于CAN工厂测试，检测到0x211包置位标志位 */
        if(can_test_flag == 0)
        {
            if(frame->std_id == 0x211)
            {
                can_test_flag = 1;
            }
        }
        else if(can_test_flag == 1)
        {
            if(frame->std_id == 0x212)
            {
                can_test_flag = 2;
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

static void can3_send_frame_port(can_frame_t *frame)
{
    flexcan_frame_t tx_frame;
    memset((uint8_t*)(&tx_frame), 0, sizeof(tx_frame));
    tx_frame.id = FLEXCAN_ID_STD(frame->std_id);
    tx_frame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    tx_frame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    tx_frame.length = frame->dlc;

    switch(frame->dlc)
    {
        case 8:
            tx_frame.dataByte7 = frame->data[7];
        case 7:
            tx_frame.dataByte6 = frame->data[6];
        case 6:
            tx_frame.dataByte5 = frame->data[5];
        case 5:
            tx_frame.dataByte4 = frame->data[4];
        case 4:
            tx_frame.dataByte3 = frame->data[3];
        case 3:
            tx_frame.dataByte2 = frame->data[2];
        case 2:
            tx_frame.dataByte1 = frame->data[1];
        case 1:
            tx_frame.dataByte0 = frame->data[0];
        default:
            break;
    }

    FLEXCAN_WriteTxMb(CAN3_PERIPHERAL, 9,  &tx_frame);
}
