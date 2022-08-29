/*
 * task_cdc_uvc.h
 *
 *  Created on: 2021年12月3日
 *      Author: ryan.pan
 */

#ifndef _TASK_CDC_UVC_H_
#define _TASK_CDC_UVC_H_

#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "stdbool.h"

#include "fsl_debug_console.h"
#include "fsl_device_registers.h"

#include "usb_device.h"
#include "usb_device_class.h"

#include "usb.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_config.h"
#include "usb_device_descriptor.h"
#include "usb_phy.h"

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
void task_cdc_uvc(void *argument);
void cdc_vcp_send(uint8_t *data, uint16_t size);
uint16_t cdc_vcp_recv(uint8_t *buff, uint16_t buff_size);
uint8_t cdc_vcp_is_attach(void);

#endif
