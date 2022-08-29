/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
/*${standard_header_anchor}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_camera.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "cmsis_os.h"

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif


static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param);
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern const unsigned char g_UsbDeviceVideoMjpegData[];
extern const uint32_t g_UsbDeviceVideoMjpegLength;

extern usb_device_class_struct_t g_UsbDeviceVideoVirtualCameraConfig;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_ProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_CommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillCommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint32_t s_ClassRequestBuffer[(sizeof(usb_device_video_probe_and_commit_controls_struct_t) >> 2U) + 1U];
/* this buffer is used to do transfer */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_ImageBuffer[HS_STREAM_IN_PACKET_SIZE];
usb_video_virtual_camera_struct_t *g_UsbDeviceVideoVirtualCamera;

static usb_video_data_req_f usb_video_data_req;
uvc_source_e uvc_source = UVC_SOURCE_COLORBAR;
uvc_source_e last_uvc_source = UVC_SOURCE_COLORBAR;
uint32_t last_data_req_time = 0;
uint8_t* uvc_data_buff;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
}
#endif

#define IMAGE_WIDTH             (320)
#define IMAGE_HEIGHT            (240)
#define VIDEO_PACKET_SIZE       FS_STREAM_IN_PACKET_SIZE

#define IMAGE_Y_LINE_NUM        (IMAGE_HEIGHT)
#define IMAGE_UV_LINE_NUM       (IMAGE_HEIGHT / 2)
#define IMAGE_LINE_NUM          (IMAGE_Y_LINE_NUM + IMAGE_UV_LINE_NUM)
#define VIDEO_PACKET_LINE_NUM   (3)
#define IMAGE_LINE_SIZE         (320)

#define GRAYSCREEN_Y                  (33)
#define GRAYSCREEN_UV                 (128)     //灰色U和V都是128

uint8_t colorbar_nv12_y_data[320] =
{
    0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x65, 0x67, 0x69, 0x6b, 0x6d, 0x6f, 0x71, 0x74,
    0x78, 0x7a, 0x7d, 0x7f, 0x81, 0x83, 0x85, 0x87, 0x8b, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a,
    0x9f, 0xa1, 0xa3, 0xa5, 0xa7, 0xa9, 0xab, 0xad, 0xb2, 0xb4, 0xb6, 0xb9, 0xbb, 0xbd, 0xbf, 0xc1,
    0xc5, 0xc7, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd1, 0xcf, 0xce, 0xcd, 0xcc, 0xcb, 0xc9, 0xc8, 0xc7,
    0xc5, 0xc4, 0xc3, 0xc2, 0xc1, 0xbf, 0xbe, 0xbd, 0xbb, 0xba, 0xb9, 0xb8, 0xb7, 0xb6, 0xb5, 0xb4,
    0xb1, 0xb0, 0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xa7, 0xa6, 0xa5, 0xa4, 0xa3, 0xa2, 0xa1, 0xa0,
    0x9e, 0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96, 0x94, 0x93, 0x92, 0x90, 0x91, 0x91, 0x92, 0x92,
    0x93, 0x93, 0x94, 0x94, 0x95, 0x95, 0x95, 0x96, 0x97, 0x97, 0x98, 0x98, 0x98, 0x99, 0x99, 0x9a,
    0x9a, 0x9b, 0x9b, 0x9c, 0x9c, 0x9d, 0x9d, 0x9d, 0x9e, 0x9f, 0x9f, 0x9f, 0xa0, 0xa0, 0xa1, 0xa1,
    0xa2, 0xa2, 0xa3, 0xa3, 0xa4, 0xa4, 0xa4, 0xa5, 0xa6, 0xa6, 0xa6, 0xa7, 0xa7, 0xa8, 0xa8, 0xa9,
    0xa9, 0xa7, 0xa5, 0xa3, 0xa1, 0x9e, 0x9c, 0x9a, 0x96, 0x94, 0x92, 0x90, 0x8d, 0x8b, 0x89, 0x87,
    0x83, 0x81, 0x7f, 0x7c, 0x7a, 0x78, 0x76, 0x73, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65, 0x62, 0x60,
    0x5c, 0x5a, 0x58, 0x56, 0x54, 0x51, 0x4f, 0x4d, 0x49, 0x47, 0x45, 0x43, 0x40, 0x3e, 0x3c, 0x3a,
    0x36, 0x33, 0x31, 0x2f, 0x2d, 0x2b, 0x29, 0x2a, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x32, 0x33, 0x34,
    0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3e, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x48,
    0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x53, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b,
    0x5d, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67, 0x68, 0x69, 0x6b, 0x6a, 0x6a, 0x69, 0x69,
    0x68, 0x68, 0x67, 0x67, 0x66, 0x66, 0x66, 0x65, 0x64, 0x64, 0x64, 0x63, 0x63, 0x62, 0x62, 0x61,
    0x61, 0x60, 0x60, 0x5f, 0x5f, 0x5f, 0x5e, 0x5e, 0x5d, 0x5c, 0x5c, 0x5c, 0x5b, 0x5b, 0x5a, 0x5a,
    0x59, 0x59, 0x58, 0x58, 0x58, 0x57, 0x57, 0x56, 0x55, 0x55, 0x55, 0x54, 0x54, 0x53, 0x53, 0x53
};

uint8_t colorbar_nv12_uv_data[320] =
{
    0x59, 0xef, 0x57, 0xec, 0x54, 0xe8, 0x52, 0xe5, 0x4e, 0xe1, 0x4c, 0xde, 0x49, 0xda, 0x47, 0xd7,
    0x43, 0xd3, 0x41, 0xcf, 0x3e, 0xcc, 0x3c, 0xc9, 0x38, 0xc5, 0x36, 0xc1, 0x33, 0xbe, 0x31, 0xbb,
    0x2d, 0xb6, 0x2b, 0xb3, 0x28, 0xb0, 0x26, 0xad, 0x22, 0xa8, 0x20, 0xa5, 0x1d, 0xa2, 0x1b, 0x9f,
    0x17, 0x9a, 0x14, 0x97, 0x12, 0x94, 0x10, 0x91, 0x12, 0x8b, 0x14, 0x87, 0x15, 0x84, 0x16, 0x80,
    0x18, 0x7a, 0x19, 0x77, 0x1b, 0x73, 0x1c, 0x6f, 0x1e, 0x6a, 0x1f, 0x66, 0x20, 0x62, 0x22, 0x5f,
    0x23, 0x59, 0x25, 0x55, 0x26, 0x51, 0x27, 0x4e, 0x29, 0x48, 0x2a, 0x44, 0x2c, 0x41, 0x2d, 0x3d,
    0x2f, 0x37, 0x30, 0x34, 0x31, 0x30, 0x33, 0x2c, 0x35, 0x27, 0x36, 0x23, 0x39, 0x22, 0x3d, 0x21,
    0x42, 0x20, 0x46, 0x20, 0x49, 0x1f, 0x4d, 0x1f, 0x53, 0x1e, 0x57, 0x1d, 0x5a, 0x1c, 0x5e, 0x1c,
    0x64, 0x1b, 0x67, 0x1a, 0x6b, 0x1a, 0x6f, 0x19, 0x74, 0x18, 0x78, 0x18, 0x7c, 0x17, 0x7f, 0x16,
    0x85, 0x16, 0x89, 0x15, 0x8c, 0x14, 0x90, 0x14, 0x96, 0x13, 0x99, 0x12, 0x9d, 0x12, 0xa1, 0x11,
    0xa6, 0x11, 0xa9, 0x14, 0xab, 0x18, 0xae, 0x1a, 0xb1, 0x1f, 0xb4, 0x22, 0xb6, 0x25, 0xb9, 0x28,
    0xbc, 0x2d, 0xbf, 0x30, 0xc1, 0x33, 0xc4, 0x37, 0xc7, 0x3b, 0xca, 0x3e, 0xcc, 0x41, 0xcf, 0x45,
    0xd3, 0x49, 0xd5, 0x4c, 0xd7, 0x50, 0xda, 0x53, 0xde, 0x57, 0xe0, 0x5a, 0xe2, 0x5e, 0xe5, 0x61,
    0xe8, 0x65, 0xeb, 0x69, 0xed, 0x6c, 0xef, 0x6f, 0xed, 0x74, 0xec, 0x78, 0xeb, 0x7c, 0xe9, 0x7f,
    0xe7, 0x85, 0xe6, 0x89, 0xe5, 0x8d, 0xe4, 0x90, 0xe2, 0x96, 0xe0, 0x9a, 0xdf, 0x9d, 0xde, 0xa1,
    0xdc, 0xa6, 0xdb, 0xaa, 0xda, 0xae, 0xd8, 0xb2, 0xd6, 0xb7, 0xd5, 0xbb, 0xd4, 0xbf, 0xd3, 0xc2,
    0xd1, 0xc8, 0xd0, 0xcc, 0xce, 0xd0, 0xcd, 0xd3, 0xcb, 0xd9, 0xca, 0xdc, 0xc7, 0xde, 0xc3, 0xdf,
    0xbd, 0xdf, 0xba, 0xe0, 0xb6, 0xe1, 0xb2, 0xe1, 0xad, 0xe2, 0xa9, 0xe3, 0xa5, 0xe3, 0xa1, 0xe4,
    0x9c, 0xe5, 0x98, 0xe5, 0x94, 0xe6, 0x91, 0xe7, 0x8b, 0xe8, 0x88, 0xe8, 0x84, 0xe9, 0x80, 0xe9,
    0x7a, 0xea, 0x77, 0xeb, 0x73, 0xeb, 0x6f, 0xec, 0x6a, 0xed, 0x66, 0xee, 0x62, 0xee, 0x5e, 0xef
};

typedef struct {
    uint8_t color_y;                    //叠加图片显示颜色Y分量（目前未实现，无作用）
    uint8_t color_u;                    //叠加图片显示颜色U分量（目前未实现，无作用）
    uint8_t color_v;                    //叠加图片显示颜色V分量（目前未实现，无作用）
    uint16_t w;                         //宽度
    uint8_t h_div8;                     //高度 / 8
}uvc_overlap_t;

static const uvc_overlap_t nosingal_pic = {
    .color_y = 0,
    .color_u = 128,
    .color_v = 128,
    .w = 186,
    .h_div8 = 3,
};

static const uint8_t nosingal_pic_data[] = {
    0xff, 0xff, 0xff, 0xff, 0xf9, 0xf9, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xf9, 0xf9, 0x7e, 0x7e, 0x1e, 0x1e, 0x1e, 0x1e,
    0x7e, 0x7e, 0xf9, 0xf9, 0xe1, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe1, 0xe1, 0xf9, 0xf9, 0xff, 0xff, 0x1e, 0x1e, 0x1e, 0x1e, 0x7e, 0x7e, 0x78, 0x78, 0x60, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x1e, 0x1e, 0xff, 0xff, 0xff, 0xff, 0x1e,
    0x1e, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xf9, 0xf9, 0x7e, 0x7e,
    0x1e, 0x1e, 0x1e, 0x1e, 0x7e, 0x7e, 0x78, 0x78, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xf9, 0xf9, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0xe1, 0xe1, 0x78, 0x78, 0x1e, 0x1e, 0x1e, 0x1e, 0x78,
    0x78, 0xe1, 0xe1, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x06, 0x06,
    0x7e, 0x7e, 0xf9, 0xf9, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x1e, 0x1e,
    0x1e, 0x1e, 0x78, 0x78, 0xe1, 0xe1, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xe1, 0xe1,
    0xe1, 0xe1, 0xe1, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x06, 0x06, 0x7e,
    0x7e, 0xf9, 0xf9, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x7e, 0x7e,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x7e, 0x7e, 0xf9, 0xf9,
    0xe1, 0xe1, 0xe1, 0xe1, 0xf9, 0xf9, 0x7e, 0x7e, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x78, 0x78, 0xf9, 0xf9, 0xe1, 0xe1, 0xe1, 0xe1, 0xff, 0xff,
    0x7e, 0x7e, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xe1, 0xe1, 0xff,
    0xff, 0xff, 0xff, 0xe1, 0xe1, 0xe1, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e,
    0x7e, 0x7e, 0xf9, 0xf9, 0xe1, 0xe1, 0xe1, 0xe1, 0x78, 0x78, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x7e, 0x7e, 0xff,
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1
};

/* Prepare next transfer payload */
void USB_DeviceVideoPrepareVideoData(void)
{
    uint16_t need_send_size = 0;
    uint8_t* image_start;

    last_data_req_time = osKernelSysTick();

    /* 发送色彩测试条 */
    if (uvc_source == UVC_SOURCE_COLORBAR)
    {
        if (g_UsbDeviceVideoVirtualCamera->cur_line >= IMAGE_LINE_NUM || last_uvc_source != uvc_source || g_UsbDeviceVideoVirtualCamera->frame_cnt == 0)
        {
            last_uvc_source = uvc_source;

            //start of new frame
            g_UsbDeviceVideoVirtualCamera->cur_line = 0;
            g_UsbDeviceVideoVirtualCamera->header[1] ^= 1; //toggle bit0 every new frame
            g_UsbDeviceVideoVirtualCamera->frame_cnt++;
        }

        for (uint8_t i = 0; i < VIDEO_PACKET_LINE_NUM; i++)
        {
            uint16_t offset = (g_UsbDeviceVideoVirtualCamera->frame_cnt % 160) * 2;
            image_start = g_UsbDeviceVideoVirtualCamera->imageBuffer + 2 + IMAGE_LINE_SIZE * i;
            if(g_UsbDeviceVideoVirtualCamera->cur_line < IMAGE_Y_LINE_NUM)
            {
                memcpy(image_start, colorbar_nv12_y_data + offset, IMAGE_LINE_SIZE - offset);
                memcpy(image_start + IMAGE_LINE_SIZE - offset, colorbar_nv12_y_data, offset);
            }
            else
            {
                memcpy(image_start, colorbar_nv12_uv_data + offset, IMAGE_LINE_SIZE - offset);
                memcpy(image_start + IMAGE_LINE_SIZE - offset, colorbar_nv12_uv_data, offset);
            }

            /* 叠加无信号图片 */
            uint16_t start_x = (IMAGE_WIDTH - nosingal_pic.w) / 2;
            uint16_t end_x = start_x + nosingal_pic.w;
            end_x = end_x > IMAGE_WIDTH ? IMAGE_WIDTH : end_x;

            uint16_t start_y = (IMAGE_HEIGHT - nosingal_pic.h_div8 * 8) / 2;
            uint16_t end_y = start_y + nosingal_pic.h_div8 * 8;
            end_y = end_y > IMAGE_HEIGHT ? IMAGE_HEIGHT : end_y;

            /* 叠加无信号图片 */
            if(start_y <= g_UsbDeviceVideoVirtualCamera->cur_line && end_y > g_UsbDeviceVideoVirtualCamera->cur_line)
            {
                uint16_t c_line = g_UsbDeviceVideoVirtualCamera->cur_line + i;
                uint8_t bit_mask = 0x01 << ((c_line - start_y + 1) % 8);
                for (int j = start_x; j < end_x; j++)
                {
                    uint16_t pos = ((c_line - start_y) / 8 ) * nosingal_pic.w + j - start_x;
                    image_start[j] = 128;
                    if(nosingal_pic_data[pos] & bit_mask)
                    {
                        image_start[j] = nosingal_pic.color_y;
                    }
                }
            }
            else if(start_y / 2 + IMAGE_Y_LINE_NUM <= g_UsbDeviceVideoVirtualCamera->cur_line &&
                end_y / 2 + IMAGE_Y_LINE_NUM > g_UsbDeviceVideoVirtualCamera->cur_line)
            {
                uint16_t c_line = g_UsbDeviceVideoVirtualCamera->cur_line + i - IMAGE_Y_LINE_NUM;
                uint8_t bit_mask = 0x01 << (((c_line - start_y / 2) * 2 + 1) % 8);
                for (int j = start_x; j < end_x; j += 2)
                {
                    uint16_t pos = ((c_line - start_y / 2) / 4) * nosingal_pic.w + j - start_x;
                    if(nosingal_pic_data[pos] & bit_mask)
                    {
                        image_start[j] = nosingal_pic.color_u;
                        image_start[j+1] = nosingal_pic.color_v;
                    }
                }
            }
        }

        need_send_size = IMAGE_LINE_SIZE * VIDEO_PACKET_LINE_NUM + 2;
        g_UsbDeviceVideoVirtualCamera->cur_line += VIDEO_PACKET_LINE_NUM;
    }
    /* 发送灰色屏幕 */
    else if (uvc_source == UVC_SOURCE_GRAYSCREEN)
    {
        if (g_UsbDeviceVideoVirtualCamera->cur_line >= IMAGE_LINE_NUM || last_uvc_source != uvc_source || g_UsbDeviceVideoVirtualCamera->frame_cnt == 0)
        {
            last_uvc_source = uvc_source;

            //start of new frame
            g_UsbDeviceVideoVirtualCamera->cur_line = 0;
            g_UsbDeviceVideoVirtualCamera->header[1] ^= 1; //toggle bit0 every new frame
            g_UsbDeviceVideoVirtualCamera->frame_cnt++;
        }

        for (uint8_t i = 0; i < VIDEO_PACKET_LINE_NUM; i++)
        {
            image_start = g_UsbDeviceVideoVirtualCamera->imageBuffer + 2 + IMAGE_LINE_SIZE * i;
            if(g_UsbDeviceVideoVirtualCamera->cur_line < IMAGE_Y_LINE_NUM)
            {
                memset(image_start, GRAYSCREEN_Y, IMAGE_LINE_SIZE);
            }
            else
            {
                 memset(image_start, GRAYSCREEN_UV, IMAGE_LINE_SIZE);
            }
        }

        need_send_size = IMAGE_LINE_SIZE * VIDEO_PACKET_LINE_NUM + 2;
        g_UsbDeviceVideoVirtualCamera->cur_line += VIDEO_PACKET_LINE_NUM;
    }
    /* 发送输入的图像 */
    else
    {
        if (g_UsbDeviceVideoVirtualCamera->cur_line >= IMAGE_LINE_NUM || last_uvc_source != uvc_source || g_UsbDeviceVideoVirtualCamera->frame_cnt == 0 ||
            uvc_data_buff == NULL)
        {
            last_uvc_source = uvc_source;
            uvc_data_buff =  usb_video_data_req();
            if(uvc_data_buff)
            {
                //start of new frame
                g_UsbDeviceVideoVirtualCamera->cur_line = 0;
                g_UsbDeviceVideoVirtualCamera->header[1] ^= 1; //toggle bit0 every new frame
                g_UsbDeviceVideoVirtualCamera->frame_cnt++;
            }
        }

        if(uvc_data_buff != NULL)
        {
            for (uint8_t i = 0; i < VIDEO_PACKET_LINE_NUM; i++)
            {
                image_start = g_UsbDeviceVideoVirtualCamera->imageBuffer + 2 + (IMAGE_WIDTH * i);
                memcpy(image_start, uvc_data_buff + IMAGE_LINE_SIZE * (g_UsbDeviceVideoVirtualCamera->cur_line + i), IMAGE_LINE_SIZE);
            }
            need_send_size = IMAGE_LINE_SIZE * VIDEO_PACKET_LINE_NUM + 2;
            g_UsbDeviceVideoVirtualCamera->cur_line += VIDEO_PACKET_LINE_NUM;
        }
        else
        {
            need_send_size = 0;
        }
    }

    g_UsbDeviceVideoVirtualCamera->imageBuffer[0] = g_UsbDeviceVideoVirtualCamera->header[0];
    g_UsbDeviceVideoVirtualCamera->imageBuffer[1] = g_UsbDeviceVideoVirtualCamera->header[1];
    g_UsbDeviceVideoVirtualCamera->imageBufferLength = need_send_size;
}

static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_device_video_probe_and_commit_controls_struct_t *probe =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_probe_and_commit_controls_struct_t *commit =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_probe =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_commit =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    uint32_t temp32;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        /* probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoVirtualCamera->probeStruct->dwFrameInterval);
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoVirtualCamera->probeStruct->dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoVirtualCamera->probeStruct->bFormatIndex = probe->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera->probeStruct->bFrameIndex  = probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_PROBE_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoVirtualCamera->probeStruct;
            request->length = g_UsbDeviceVideoVirtualCamera->probeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->probeLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->probeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->probeInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->probeInfo);
            break;
        /* commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoVirtualCamera->commitStruct->dwFrameInterval);
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoVirtualCamera->commitStruct->dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoVirtualCamera->commitStruct->bFormatIndex = commit->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera->commitStruct->bFrameIndex  = commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_COMMIT_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoVirtualCamera->commitStruct;
            request->length = g_UsbDeviceVideoVirtualCamera->commitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->commitLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->commitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->commitInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->commitInfo);
            break;
        /* still probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, g_UsbDeviceVideoVirtualCamera->stillProbeStruct->dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoVirtualCamera->stillProbeStruct->bFormatIndex = still_probe->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera->stillProbeStruct->bFrameIndex  = still_probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_PROBE_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoVirtualCamera->stillProbeStruct;
            request->length = g_UsbDeviceVideoVirtualCamera->stillProbeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->stillProbeLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->stillProbeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->stillProbeInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->stillProbeInfo);
            break;
        /* still commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, g_UsbDeviceVideoVirtualCamera->stillCommitStruct->dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoVirtualCamera->stillCommitStruct->bFormatIndex = still_commit->bFormatIndex;
            g_UsbDeviceVideoVirtualCamera->stillCommitStruct->bFrameIndex  = still_commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_COMMIT_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoVirtualCamera->stillCommitStruct;
            request->length = g_UsbDeviceVideoVirtualCamera->stillCommitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->stillCommitLength;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->stillCommitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoVirtualCamera->stillCommitInfo;
            request->length = sizeof(g_UsbDeviceVideoVirtualCamera->stillCommitInfo);
            break;
        /* still image trigger request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_IMAGE_TRIGGER_CONTROL:
            g_UsbDeviceVideoVirtualCamera->stillImageTriggerControl = *(request->buffer);
            break;
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/* USB device Video class callback */
usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    switch (event)
    {
        case kUSB_DeviceVideoEventStreamSendResponse:
            /* Stream data dent */
            if (g_UsbDeviceVideoVirtualCamera->attach)
            {
                /* Prepare the next stream data */
                USB_DeviceVideoPrepareVideoData();
                error = USB_DeviceVideoSend(
                    g_UsbDeviceVideoVirtualCamera->videoHandle, USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN,
                    g_UsbDeviceVideoVirtualCamera->imageBuffer, g_UsbDeviceVideoVirtualCamera->imageBufferLength);
            }
            break;
        case kUSB_DeviceVideoEventClassRequestBuffer:
            if (param && (g_UsbDeviceVideoVirtualCamera->attach))
            {
                /* Get the class-specific OUT buffer */
                usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;

                if (request->length <= sizeof(usb_device_video_probe_and_commit_controls_struct_t))
                {
                    request->buffer = (uint8_t *)g_UsbDeviceVideoVirtualCamera->classRequestBuffer;
                    error           = kStatus_USB_Success;
                }
            }
            break;
        default:
            if (param && (event > 0xFFU))
            {
                /* If the event is the class-specific request(Event > 0xFFU), handle the class-specific request */
                error = USB_DeviceVideoRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/* Set to default state */
void USB_DeviceVideoApplicationSetDefault(void)
{
    g_UsbDeviceVideoVirtualCamera->speed                = USB_SPEED_FULL;
    g_UsbDeviceVideoVirtualCamera->attach               = 0U;
    g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
    g_UsbDeviceVideoVirtualCamera->imageBuffer          = s_ImageBuffer;
    g_UsbDeviceVideoVirtualCamera->probeStruct          = &s_ProbeStruct;
    g_UsbDeviceVideoVirtualCamera->commitStruct         = &s_CommitStruct;
    g_UsbDeviceVideoVirtualCamera->stillProbeStruct     = &s_StillProbeStruct;
    g_UsbDeviceVideoVirtualCamera->stillCommitStruct    = &s_StillCommitStruct;
    g_UsbDeviceVideoVirtualCamera->classRequestBuffer   = &s_ClassRequestBuffer[0];
    for (uint8_t i = 0; i < USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT; i++)
    {
        g_UsbDeviceVideoVirtualCamera->currentInterfaceAlternateSetting[i] = 0;
    }

    g_UsbDeviceVideoVirtualCamera->probeStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera->probeStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoVirtualCamera->probeStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize,
                                   g_UsbDeviceVideoVirtualCamera->probeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoVirtualCamera->probeStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoVirtualCamera->commitStruct->bFormatIndex = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera->commitStruct->bFrameIndex  = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoVirtualCamera->commitStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize,
                                   g_UsbDeviceVideoVirtualCamera->commitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoVirtualCamera->commitStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoVirtualCamera->probeInfo    = 0x03U;
    g_UsbDeviceVideoVirtualCamera->probeLength  = 26U;
    g_UsbDeviceVideoVirtualCamera->commitInfo   = 0x03U;
    g_UsbDeviceVideoVirtualCamera->commitLength = 26U;

    g_UsbDeviceVideoVirtualCamera->stillProbeStruct->bFormatIndex      = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera->stillProbeStruct->bFrameIndex       = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoVirtualCamera->stillProbeStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize,
                                   g_UsbDeviceVideoVirtualCamera->stillProbeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoVirtualCamera->stillProbeStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoVirtualCamera->stillCommitStruct->bFormatIndex      = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoVirtualCamera->stillCommitStruct->bFrameIndex       = USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoVirtualCamera->stillCommitStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoVirtualCamera->currentMaxPacketSize,
                                   g_UsbDeviceVideoVirtualCamera->stillCommitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoVirtualCamera->stillCommitStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoVirtualCamera->stillProbeInfo    = 0x03U;
    g_UsbDeviceVideoVirtualCamera->stillProbeLength  = sizeof(s_StillProbeStruct);
    g_UsbDeviceVideoVirtualCamera->stillCommitInfo   = 0x03U;
    g_UsbDeviceVideoVirtualCamera->stillCommitLength = sizeof(s_StillCommitStruct);

    g_UsbDeviceVideoVirtualCamera->currentTime                            = 0U;
    g_UsbDeviceVideoVirtualCamera->currentFrameId                         = 0U;
    g_UsbDeviceVideoVirtualCamera->currentStreamInterfaceAlternateSetting = 0U;
    g_UsbDeviceVideoVirtualCamera->imageBufferLength                      = 0U;
    g_UsbDeviceVideoVirtualCamera->imageIndex                             = 0U;
    g_UsbDeviceVideoVirtualCamera->waitForNewInterval                     = 0U;
    g_UsbDeviceVideoVirtualCamera->stillImageTransmission                 = 0U;
    g_UsbDeviceVideoVirtualCamera->stillImageTriggerControl = USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;

    g_UsbDeviceVideoVirtualCamera->cur_line    = 0xffff;
    g_UsbDeviceVideoVirtualCamera->header[0]   = 2;
    g_UsbDeviceVideoVirtualCamera->header[1]   = 0;
}

/*!
 * @brief Video device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceVideoCamInit(usb_video_virtual_camera_struct_t *usbDeviceVideoVirtualCamera)
{
    g_UsbDeviceVideoVirtualCamera = usbDeviceVideoVirtualCamera;
    return kStatus_USB_Success;
}

/**
 * @brief   注册数据请求回调
 *
 * @param fun
 */
void usb_video_data_req_handle_reg(usb_video_data_req_f fun)
{
    usb_video_data_req = fun;
}

/**
 * @brief 设置USB视频原
 *
 * @param source
 */
void usb_video_set_source(uvc_source_e s)
{
    uvc_source = s;
}

/**
 * @brief 获取USB上一次请求的时间
 *
 */
uint32_t usb_video_get_last_req_timer(void)
{
    return last_data_req_time;
}
