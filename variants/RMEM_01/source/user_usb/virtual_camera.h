/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_VIDEO_VIRTUAL_CAMERA_H__
#define __USB_VIDEO_VIRTUAL_CAMERA_H__

#include "usb_device_video.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

typedef enum
{
    UVC_SOURCE_COLORBAR     = 0,    //输入源为测试用色彩条
    UVC_SOURCE_GRAYSCREEN   = 1,    //输入源为灰色屏幕
    UVC_SOURCE_INPUT        = 2,    //输入源为外部输入
}uvc_source_e;

typedef struct _usb_video_virtual_camera_struct
{
    usb_device_handle deviceHandle;
    class_handle_t videoHandle;
    uint32_t currentMaxPacketSize;
    uint8_t *imageBuffer;
    uint32_t imageBufferLength;
    usb_device_video_probe_and_commit_controls_struct_t *probeStruct;
    usb_device_video_probe_and_commit_controls_struct_t *commitStruct;
    usb_device_video_still_probe_and_commit_controls_struct_t *stillProbeStruct;
    usb_device_video_still_probe_and_commit_controls_struct_t *stillCommitStruct;
	uint32_t imageIndex;
    uint32_t currentTime;
    uint32_t *classRequestBuffer;
    uint8_t currentFrameId;
    uint8_t waitForNewInterval;
    uint8_t currentStreamInterfaceAlternateSetting;
    uint8_t probeLength;
    uint8_t commitLength;
    uint8_t probeInfo;
    uint8_t commitInfo;
    uint8_t stillProbeLength;
    uint8_t stillCommitLength;
    uint8_t stillProbeInfo;
    uint8_t stillCommitInfo;
    uint8_t stillImageTransmission;
    uint8_t stillImageTriggerControl;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;

    uint8_t header[2];
    uint16_t frame_cnt;
    uint32_t cur_line;
    uint8_t  play_status;
} usb_video_virtual_camera_struct_t;

typedef uint8_t*(*usb_video_data_req_f)(void);  /*!< 请求数据回调函数  */

/*******************************************************************************
 * API
 ******************************************************************************/
void USB_DeviceVideoApplicationSetDefault(void);
void USB_DeviceVideoPrepareVideoData(void);
usb_status_t USB_DeviceVideoCamInit(usb_video_virtual_camera_struct_t *usbDeviceVideoVirtualCamera);
usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param);

void usb_video_data_req_handle_reg(usb_video_data_req_f fun);
void usb_video_set_source(uvc_source_e s);
uint32_t usb_video_get_last_req_timer(void);

#endif /* __USB_VIDEO_VIRTUAL_CAMERA_H__ */
