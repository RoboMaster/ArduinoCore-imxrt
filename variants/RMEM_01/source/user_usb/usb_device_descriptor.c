/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_video.h"

#include "usb_device_descriptor.h"

#include "user_board.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
/* Define endpoint for communication class */
usb_device_endpoint_struct_t g_UsbDeviceCdcVcomCicEndpoints[USB_CDC_VCOM_ENDPOINT_CIC_COUNT] = {
    {
        USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT | (USB_IN << 7U),
        USB_ENDPOINT_INTERRUPT,
        FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
        FS_CDC_VCOM_INTERRUPT_IN_INTERVAL,
    },
};
#endif

/* Define endpoint for data class */
usb_device_endpoint_struct_t g_UsbDeviceCdcVcomDicEndpoints[USB_CDC_VCOM_ENDPOINT_DIC_COUNT] = {
    {
        USB_CDC_VCOM_BULK_IN_ENDPOINT | (USB_IN << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_IN_PACKET_SIZE,
        0U,
    },
    {
        USB_CDC_VCOM_BULK_OUT_ENDPOINT | (USB_OUT << 7U),
        USB_ENDPOINT_BULK,
        FS_CDC_VCOM_BULK_OUT_PACKET_SIZE,
        0U,
    }};

/* Define interface for communication class */
usb_device_interface_struct_t g_UsbDeviceCdcVcomCommunicationInterface[] = {{
    USB_CDC_VCOM_COMM_INTERFACE_ALTERNATE_0,
    {
        USB_CDC_VCOM_ENDPOINT_CIC_COUNT,
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
        NULL,
#else
        g_UsbDeviceCdcVcomCicEndpoints,
#endif
    },
}};

/* Define interface for data class */
usb_device_interface_struct_t g_UsbDeviceCdcVcomDataInterface[] = {{
    USB_CDC_VCOM_DATA_INTERFACE_ALTERNATE_0,
    {
        USB_CDC_VCOM_ENDPOINT_DIC_COUNT,
        g_UsbDeviceCdcVcomDicEndpoints,
    },
}};

/* Define interfaces for virtual com */
usb_device_interfaces_struct_t g_UsbDeviceCdcVcomInterfaces[USB_CDC_VCOM_INTERFACE_COUNT] = {
    {USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL, USB_CDC_VCOM_COMM_INTERFACE_INDEX,
     g_UsbDeviceCdcVcomCommunicationInterface,
     sizeof(g_UsbDeviceCdcVcomCommunicationInterface) / sizeof(usb_device_interface_struct_t)},
    {USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL, USB_CDC_VCOM_DATA_INTERFACE_INDEX,
     g_UsbDeviceCdcVcomDataInterface, sizeof(g_UsbDeviceCdcVcomDataInterface) / sizeof(usb_device_interface_struct_t)},
};

/* Define configurations for virtual com */
usb_device_interface_list_t g_UsbDeviceCdcVcomInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_CDC_VCOM_INTERFACE_COUNT,
        g_UsbDeviceCdcVcomInterfaces,
    },
};

/* Define class information for virtual com */
usb_device_class_struct_t g_UsbDeviceCdcVcomConfig = {
    g_UsbDeviceCdcVcomInterfaceList,
    kUSB_DeviceClassTypeCdc,
    USB_DEVICE_CONFIGURATION_COUNT,
};

usb_device_endpoint_struct_t g_UsbDeviceVideoControlEndpoints[USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_INTERRUPT,
        FS_INTERRUPT_IN_PACKET_SIZE,
        FS_INTERRUPT_IN_INTERVAL,
    },
};

/* Video device entities */
usb_device_video_entity_struct_t g_UsbDeviceVideoEntity[] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,   /* The ID of input terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_INPUT_TERMINAL,  /* Entity sub type type is VC input terminal */
        USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE, /* VC intput terminal type */
    },
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,  /* The ID of output terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_OUTPUT_TERMINAL, /* Entity sub type type is VC output terminal */
        0U,
    },
    {
        USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_ID,  /* The ID of processing terminal */
        USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_PROCESSING_UNIT, /* Entity sub type type is VC processing terminal */
        0U,
    },
};

/* Video device enetitis list */
usb_device_video_entities_struct_t g_UsbDeviceVideoEntities = {
    g_UsbDeviceVideoEntity,
    sizeof(g_UsbDeviceVideoEntity) / sizeof(usb_device_video_entity_struct_t),
};

/* Video device control interface */
usb_device_interface_struct_t g_UsbDeviceVideoControlInterface[] = {{
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_ALTERNATE_0, /* Alternate setting value */
    {
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT, /* control endpoint count */
        g_UsbDeviceVideoControlEndpoints,                /* control endpoint list */
    },
    &g_UsbDeviceVideoEntities, /* video device entitis list pointer */
}};

/* Video stream endpoint information */
usb_device_endpoint_struct_t g_UsbDeviceVideoStreamEndpoints[USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
        USB_ENDPOINT_ISOCHRONOUS,
        FS_STREAM_IN_PACKET_SIZE,
        FS_STREAM_IN_INTERVAL,
    },
};

/* Video device stream interface */
usb_device_interface_struct_t g_UsbDeviceVideoStreamInterface[] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_ALTERNATE_0, /* Alternate setting value is zero*/
        {
            0U, /* endpoint count is zero for this alternate setting */
            NULL,
        },
        NULL,
    },
    {
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_ALTERNATE_1, /* Alternate setting value is one */
        {
            USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT, /* endpoint count is zero for this alternate setting */
            g_UsbDeviceVideoStreamEndpoints,                /* stream endpoint list */
        },
        NULL,
    },
};

/* The video device interfaces */
usb_device_interfaces_struct_t g_UsbDeviceVideoInterfaces[USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT] = {
    {
        USB_DEVICE_VIDEO_CC_VIDEO,                        /* Class code */
        USB_DEVICE_VIDEO_SC_VIDEOCONTROL,                 /* Subclass code */
        USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,           /* Protocol code */
        USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* The control interface index */
        g_UsbDeviceVideoControlInterface,                 /* control interface list */
        sizeof(g_UsbDeviceVideoControlInterface) / sizeof(usb_device_interface_struct_t),
    },
    {
        USB_DEVICE_VIDEO_CC_VIDEO,                       /* Class code */
        USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* Subclass code */
        USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,          /* Protocol code */
        USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* The stream interface index */
        g_UsbDeviceVideoStreamInterface,                 /* stream interface list */
        sizeof(g_UsbDeviceVideoStreamInterface) / sizeof(usb_device_interface_struct_t),
    },
};

/* The video device interface list */
usb_device_interface_list_t g_UsbDeviceVideoInterfaceList[USB_DEVICE_CONFIGURATION_COUNT] = {
    {
        USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT, /* the interface count */
        g_UsbDeviceVideoInterfaces,               /* the control and stream interfaces pointer */
    },
};

/* The video device all interface list */
usb_device_class_struct_t g_UsbDeviceVideoVirtualCameraConfig = {
    g_UsbDeviceVideoInterfaceList,
    kUSB_DeviceClassTypeVideo,
    USB_DEVICE_CONFIGURATION_COUNT,
};

/* Define device descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceDescriptor[] = {
    /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_LENGTH_DEVICE,
    /* DEVICE Descriptor Type */
    USB_DESCRIPTOR_TYPE_DEVICE,
    /* USB Specification Release Number in Binary-Coded Decimal (i.e., 2.10 is 210H). */
    USB_SHORT_GET_LOW(USB_DEVICE_SPECIFIC_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_SPECIFIC_BCD_VERSION),
    /* Class code (assigned by the USB-IF). */
    USB_DEVICE_CLASS,
    /* Subclass code (assigned by the USB-IF). */
    USB_DEVICE_SUBCLASS,
    /* Protocol code (assigned by the USB-IF). */
    USB_DEVICE_PROTOCOL,
    /* Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) */
    USB_CONTROL_MAX_PACKET_SIZE,
    USB_SHORT_GET_LOW(USB_DEVICE_VID),
    USB_SHORT_GET_HIGH(USB_DEVICE_VID), /* Vendor ID (assigned by the USB-IF) */
    USB_SHORT_GET_LOW(USB_DEVICE_PID),
    USB_SHORT_GET_HIGH(USB_DEVICE_PID), /* Product ID (assigned by the manufacturer) */
    /* Device release number in binary-coded decimal */
    USB_SHORT_GET_LOW(USB_DEVICE_DEMO_BCD_VERSION),
    USB_SHORT_GET_HIGH(USB_DEVICE_DEMO_BCD_VERSION),
    /* Index of string descriptor describing manufacturer */
    0x01,
    /* Index of string descriptor describing product */
    0x02,
    /* Index of string descriptor describing the device's serial number */
    0x03,
    /* Number of possible configurations */
    USB_DEVICE_CONFIGURATION_COUNT,
};

/* Define configuration descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceConfigurationDescriptor[] = {
    /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_LENGTH_CONFIGURE,
    /* CONFIGURATION Descriptor Type */
    USB_DESCRIPTOR_TYPE_CONFIGURE,
    /* Total length of data returned for this configuration. */
    USB_SHORT_GET_LOW(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                      /* ----------------cdc-------------------*/
                      USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION +  USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC + USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG +
                      USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT + USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC +
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
#endif
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      /* ----------------uvc-------------------*/
                      USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION +
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_HEADER_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_LENGTH + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_HEADER_LENGTH +
					  USB_VIDEO_UNCOMPRESSED_FORMAT_DESCRIPTOR_LENGTH + USB_VIDEO_UNCOMPRESSED_FRAME_DESCRIPTOR_LENGTH +
					  USB_VIDEO_COLOR_MATCH_DESCRIPTOR_LENGTH + USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT),

    USB_SHORT_GET_HIGH(USB_DESCRIPTOR_LENGTH_CONFIGURE +
                      /* ----------------cdc-------------------*/
                      USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION +  USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC + USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG +
                      USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT + USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC +
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
#endif
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      /* ----------------uvc-------------------*/
                      USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION +
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_HEADER_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_LENGTH +
                      USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_LENGTH + USB_DESCRIPTOR_LENGTH_ENDPOINT +
                      USB_DESCRIPTOR_LENGTH_INTERFACE + USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_HEADER_LENGTH +
					  USB_VIDEO_UNCOMPRESSED_FORMAT_DESCRIPTOR_LENGTH + USB_VIDEO_UNCOMPRESSED_FRAME_DESCRIPTOR_LENGTH +
					  USB_VIDEO_COLOR_MATCH_DESCRIPTOR_LENGTH + USB_DESCRIPTOR_LENGTH_INTERFACE +
                      USB_DESCRIPTOR_LENGTH_ENDPOINT),
    /* Number of interfaces supported by this configuration */
    USB_INTERFACE_COUNT,
    /* Value to use as an argument to the SetConfiguration() request to select this configuration */
    USB_COMPOSITE_CONFIGURE_INDEX,
    /* Index of string descriptor describing this configuration */
    0,
    /* Configuration characteristics D7: Reserved (set to one) D6: Self-powered D5: Remote Wakeup D4...0: Reserved
       (reset to zero) */
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
        (USB_DEVICE_CONFIG_SELF_POWER << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_SELF_POWERED_SHIFT) |
        (USB_DEVICE_CONFIG_REMOTE_WAKEUP << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT),
    /* Maximum power consumption of the USB * device from the bus in this specific * configuration when the device is
       fully * operational. Expressed in 2 mA units *  (i.e., 50 = 100 mA).  */
    USB_DEVICE_CONFIG_POWER_MA(100),

    /* cdc_iad---------------------------------------------------------------------------------------------------------*/
    /* Interface Association Descriptor */
    /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION,
    /* INTERFACE_ASSOCIATION Descriptor Type  */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
    /* The first interface number associated with this function */
    0x00,
    /* The number of contiguous interfaces associated with this function */
    0x02,
    /* The function belongs to the Communication Device/Interface Class  */
    USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS,
    /* The function uses the No class specific protocol required Protocol  */
    0x00,
    /* The Function string descriptor index */
    0x04,

    /* cdc_itf---------------------------------------------------------------------------------------------------------*/
    /* Communication Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_COMM_INTERFACE_INDEX, USB_CDC_VCOM_COMM_INTERFACE_ALTERNATE_0,
    USB_CDC_VCOM_ENDPOINT_CIC_COUNT, USB_CDC_VCOM_CIC_CLASS, USB_CDC_VCOM_CIC_SUBCLASS, USB_CDC_VCOM_CIC_PROTOCOL,
    0x00, /* Interface Description String Index*/

    /* CDC Class-Specific descriptor */
    /* cdc_header------------------------------------------------------------------------------------------------------*/
    USB_DESCRIPTOR_LENGTH_CDC_HEADER_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE,  /* CS_INTERFACE Descriptor Type */
    USB_CDC_HEADER_FUNC_DESC, 0x10,
    0x01, /* USB Class Definitions for Communications the Communication specification version 1.10 */

    /* cdc_call_manag--------------------------------------------------------------------------------------------------*/
    USB_DESCRIPTOR_LENGTH_CDC_CALL_MANAG, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_CALL_MANAGEMENT_FUNC_DESC,
    0x01, /*Bit 0: Whether device handle call management itself 1, Bit 1: Whether device can send/receive call
             management information over a Data Class Interface 0 */
    0x01, /* Indicates multiplexed commands are handled via data interface */

    /* cdc_acm--------------------------------------------------------------------------------------------------------*/
    USB_DESCRIPTOR_LENGTH_CDC_ABSTRACT,   /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_ABSTRACT_CONTROL_FUNC_DESC,
    0x06, /* Bit 0: Whether device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and
             Get_Comm_Feature 0, Bit 1: Whether device supports the request combination of Set_Line_Coding,
             Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State 1, Bit ...  */

    /* cdc_union------------------------------------------------------------------------------------------------------*/
    USB_DESCRIPTOR_LENGTH_CDC_UNION_FUNC, /* Size of this descriptor in bytes */
    USB_DESCRIPTOR_TYPE_CDC_CS_INTERFACE, /* CS_INTERFACE Descriptor Type */
    USB_CDC_UNION_FUNC_DESC, 0x00,        /* The interface number of the Communications or Data Class interface  */
    0x01,                                 /* Interface number of subordinate interface in the Union  */

    /* cdc_ep--------------------------------------------------------------------------------------------------------*/
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
    /*Notification Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT | (USB_IN << 7U),
    USB_ENDPOINT_INTERRUPT, USB_SHORT_GET_LOW(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE), FS_CDC_VCOM_INTERRUPT_IN_INTERVAL,
#endif

    /* cdc_itf-------------------------------------------------------------------------------------------------------*/
    /* Data Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE, USB_DESCRIPTOR_TYPE_INTERFACE, USB_CDC_VCOM_DATA_INTERFACE_INDEX, USB_CDC_VCOM_DATA_INTERFACE_ALTERNATE_0,
    USB_CDC_VCOM_ENDPOINT_DIC_COUNT, USB_CDC_VCOM_DIC_CLASS, USB_CDC_VCOM_DIC_SUBCLASS, USB_CDC_VCOM_DIC_PROTOCOL,
    0x00, /* Interface Description String Index*/

    /* cdc_ep--------------------------------------------------------------------------------------------------------*/
    /*Bulk IN Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_BULK_IN_ENDPOINT | (USB_IN << 7U),
    USB_ENDPOINT_BULK, USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_IN_PACKET_SIZE), 0x00, /* The polling interval value is every 0 Frames */

    /* cdc_ep--------------------------------------------------------------------------------------------------------*/
    /*Bulk OUT Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, USB_DESCRIPTOR_TYPE_ENDPOINT, USB_CDC_VCOM_BULK_OUT_ENDPOINT | (USB_OUT << 7U),
    USB_ENDPOINT_BULK, USB_SHORT_GET_LOW(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE),
    USB_SHORT_GET_HIGH(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE), 0x00, /* The polling interval value is every 0 Frames */


    /* uvc_iad-------------------------------------------------------------------------------------------------------*/
    /* Interface Association Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE_ASSOCIATION,      /* size of the IAD */
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,        /* INTERFACE ASSOCIATION Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* Interface number of the first VideoControl
                                                         interface that is associated with this function. */
    USB_VIDEO_VIRTUAL_CAMERA_INTERFACE_COUNT,         /* Number of contiguous VideoStreaming interfaces that are
                                                         associated with this function.
                                                         The count includes the first VideoControl interface and all
                                                         its associated VideoStreaming interfaces.
                                                      */
    USB_DEVICE_VIDEO_CC_VIDEO,                        /* CC_VIDEO, Video Interface Class code */
    USB_DEVICE_VIDEO_SC_VIDEO_INTERFACE_COLLECTION,   /* SC_VIDEO_INTERFACE_COLLECTION. Video Interface
                                                         Subclass code */
    USB_DEVICE_VIDEO_PC_PROTOCOL_UNDEFINED,           /* Not used, Must be set to PC_PROTOCOL_UNDEFINED */
    0x05U,                                            /* Index of a string descriptor */

    /* uvc_itf-------------------------------------------------------------------------------------------------------*/
    /* Standard VC Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                  /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                    /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_INDEX, /* Index of control interface */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_INTERFACE_ALTERNATE_0, /* Index of the interface setting */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT_COUNT,  /* One endpoint of control pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                        /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOCONTROL,                 /* SC_VIDEOCONTROL */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,         /* Protocol */
    0x00U,                                            /* Index of this string descriptor */

    /* Class-Specific VC Interface header Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_HEADER_LENGTH, /* Size of this descriptor, 12+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,              /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_HEADER,              /* VC_HEADER, descriptor subtype */
    USB_SHORT_GET_LOW(USB_DEVICE_VIDEO_SPECIFIC_BCD_VERSION), USB_SHORT_GET_HIGH(USB_DEVICE_VIDEO_SPECIFIC_BCD_VERSION),
    /* bcdUVC */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_TOTAL_LENGTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VC_INTERFACE_TOTAL_LENGTH),
    /* Total number of bytes returned for the class-specific VideoControl interface descriptor.
       Includes the combined length of this descriptor header and all Unit and Terminal descriptors. */
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_CLOCK_FREQUENCY),
    /* Use of this field has been deprecated. */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_COUNT, /* The number of VideoStreaming interfaces in the Video
                                                        Interface Collection to which this VideoControl
                                                        interface belongs: n */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Interface number of the first VideoStreaming
                                                        interface in the Collection */

    /* Input Terminal Descriptor (Camera) */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_LENGTH, /* Size of this descriptor, 15+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,            /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_INPUT_TERMINAL,    /* VC_INPUT_TERMINAL, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,     /* A non-zero constant that uniquely identifies the
                                                          Terminal within the video function. This value is used
                                                          in all requests to address this Terminal. */
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_TYPE),
    /* ITT_CAMERA, Constant that characterizes the type of Terminal */
    0x00U,               /* ID of the Output Terminal to which this Input Terminal is associated */
    0x00U,               /* Index of a string descriptor */
    0x00U, 0x00U,        /* The value of Lmin, unsupported */
    0x00U, 0x00U,        /* The value of Lmax, unsupported */
    0x00U, 0x00U,        /* The value of Locular, unsupported */
    0x03U,               /* Size in bytes of the bmControls field: n */
    0x00U, 0x00U, 0x00U, /* Not control supported */

    /* Output Terminal Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_LENGTH, /* Size of this descriptor, 9+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,             /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_OUTPUT_TERMINAL,    /* VC_OUTPUT_TERMINAL, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,     /* A non-zero constant that uniquely identifies the
                                                           Terminal within the video function. This value is
                                                           used in all requests to address this Terminal. */
    USB_SHORT_GET_LOW(USB_DEVICE_VIDEO_TT_STREAMING), USB_SHORT_GET_HIGH(USB_DEVICE_VIDEO_TT_STREAMING),
    /* TT_STREAMING, Constant that characterizes the type of Terminal */
    0x00U, /* ID of the Input Terminal to which this Output Terminal is associated */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID, /* ID of the Unit or Terminal to which this Terminal is
                                                      connected */
    0x00U,                                         /* Index of a string descriptor */

    /* Processing Unit Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_LENGTH, /* Size of this descriptor, 10+n */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,             /* CS_INTERFACE, descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VC_PROCESSING_UNIT,    /* VC_PROCESSING_UNIT, descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_VC_PROCESSING_UNIT_ID,     /* A non-zero constant that uniquely identifies the
                                                           Terminal within the video function. This value is
                                                           used in all requests to address this Terminal. */
    USB_VIDEO_VIRTUAL_CAMERA_VC_INPUT_TERMINAL_ID,      /* ID of the Unit or Terminal to which this Terminal is
                                                           connected */
    0x00U, 0x00U, /* This field indicates the maximum digital magnification, multiplied by 100U */
#if defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_5) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_5 > 0U)
    0x03U,               /* Size of the bmControls field, in bytes: n */
    0x00U, 0x00U, 0x00U, /* Not control supported */
#else
    0x02U,        /* Size of the bmControls field, in bytes: n */
    0x00U, 0x00U, /* Not control supported */
#endif     /* USB_DEVICE_VIDEO_CLASS_VERSION_1_5 */
    0x00U, /* Index of a string descriptor */
#if defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_1) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_1 > 0U) || \
    defined(USB_DEVICE_VIDEO_CLASS_VERSION_1_5) && (USB_DEVICE_VIDEO_CLASS_VERSION_1_5 > 0U)
    0x00U, /* A bitmap of all analog video standards supported by the Processing Unit */
#endif     /* USB_DEVICE_VIDEO_CLASS_VERSION_1_1 || USB_DEVICE_VIDEO_CLASS_VERSION_1_5 */
    /* Standard VC Interrupt Endpoint Descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor, in bytes: 7U */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT descriptor type */
    USB_VIDEO_VIRTUAL_CAMERA_CONTROL_ENDPOINT |
        (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT), /* Endpoint address */
    USB_ENDPOINT_INTERRUPT,                                          /* Transfer type */
    USB_SHORT_GET_LOW(FS_INTERRUPT_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_INTERRUPT_IN_PACKET_SIZE),
    /* Max Packet Size */
    FS_INTERRUPT_IN_INTERVAL, /* Interval */

    /* Standard VS Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                 /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                   /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Index of stream interface */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_ALTERNATE_0, /* Index of the interface setting */
    0U,                                              /* No endpoint of stream pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                       /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* SC_VIDEOSTREAMING */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,        /* Protocol */
    0x00U,                                           /* Index of this string descriptor */

    /* VS Specific Input Header Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_HEADER_LENGTH, /* Size of this descriptor, in bytes: 13+(p*n) */
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,              /* CS_INTERFACE descriptor type */
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_INPUT_HEADER,        /* VS_INPUT_HEADER descriptor subtype */
    USB_VIDEO_VIRTUAL_CAMERA_FORMAT_COUNT,               /* One format (p = 1U)*/
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_TOTAL_LENGTH),
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_VS_INTERFACE_TOTAL_LENGTH),
    /* Total number of bytes returned for the class-specific VideoStreaming interface descriptors including
       this header descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << 0x07U),
    /* The address of the isochronous or bulk endpoint used for video data */
    0x00U,                                                /* Dynamic Format Change unsupported */
    USB_VIDEO_VIRTUAL_CAMERA_VC_OUTPUT_TERMINAL_ID,       /* The terminal ID of the Output Terminal to which the
                                                             video endpoint of this interface is connected */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_STILL_CAPTURE_METHOD, /* Method 2U, still image capture supported */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_STILL_CAPTURE_TRIGGER_SUPPOTED,
    /* Unsupported hardware trigger */
    0x00U, /* Specifies how the host software shall respond to a hardware trigger interrupt event from this
             interface */
    0x01U, /* Size of each bmaControl(x) field */
    0x00U, /* Not used */

    /* Uncompressed Video Format Descriptor */
    USB_VIDEO_UNCOMPRESSED_FORMAT_DESCRIPTOR_LENGTH,
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,
    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_FORMAT_UNCOMPRESSED,
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FORMAT_INDEX,
    0x01,	//bNumFrameDescriptors

    'N','V','1','2',				/* NV12:*/
    0x00,0x00,
    0x10,0x00,
    0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71,

    12,		//bBitsPerPixel
    0x01,	//bDefaultFrameIndex
    0x00,	//bAspectRatioX
    0x00,	//bAspectRatioY
    0x00,	//bmInterlaceFlags
    0x00,	//bCopyProtect

    /* Uncompressed Video Frame Descriptor */
    USB_VIDEO_UNCOMPRESSED_FRAME_DESCRIPTOR_LENGTH,
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,

    USB_DESCRIPTOR_SUBTYPE_VIDEO_VS_FRAME_UNCOMPRESSED,		//bDescriptorSubtype
    USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_INDEX, 		//bFrameIndex
    2,		//bmCapabilities
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),		//wWidth
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_WIDTH),
    USB_SHORT_GET_LOW(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),		//wHeight
    USB_SHORT_GET_HIGH(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_HEIGHT),
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MIN_BIT_RATE),		//dwMinBitRate
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MIN_BIT_RATE),
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_BIT_RATE),		//dwMaxBitRate
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_BIT_RATE),
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE),		//dwMaxVideoFrameBufferSize
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_YUYV_FRAME_MAX_FRAME_SIZE),
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),		//dwDefaultFrameInterval
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL),
    0x01,																					//bFrameIntervalType
    USB_LONG_GET_BYTE0(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL),			//dwMinFrameInterval
    USB_LONG_GET_BYTE1(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL),
    USB_LONG_GET_BYTE2(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL),
    USB_LONG_GET_BYTE3(USB_VIDEO_VIRTUAL_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL),

    /* Color Matching Descriptor */
    USB_VIDEO_COLOR_MATCH_DESCRIPTOR_LENGTH,
    USB_DESCRIPTOR_TYPE_VIDEO_CS_INTERFACE,
    0x0DU,				//bDescriptorSubtype
    0x01U,				//bColorPrimaries
    0x01U,				//bTransferCharacteristics
    0x04U,				//bMatrixCoefficients

    /* Standard VS Interface Descriptor */
    USB_DESCRIPTOR_LENGTH_INTERFACE,                 /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_INTERFACE,                   /* INTERFACE Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_INDEX, /* Index of stream interface */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_INTERFACE_ALTERNATE_1, /* Index of the interface setting */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_COUNT,  /* One endpoint of stream pipe */
    USB_DEVICE_VIDEO_CC_VIDEO,                       /* CC_VIDEO */
    USB_DEVICE_VIDEO_SC_VIDEOSTREAMING,              /* SC_VIDEOSTREAMING */
    USB_DEVICE_VIDEO_VIRTUAL_CAMERA_PROTOCOL,        /* Protocol */
    0x00U,                                           /* Index of this string descriptor */

    /*Endpoint descriptor */
    USB_DESCRIPTOR_LENGTH_ENDPOINT, /* Size of this descriptor */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* ENDPOINT Descriptor */
    USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_ISOCHRONOUS | USB_DESCRIPTOR_ENDPOINT_ATTRIBUTE_SYNC_TYPE_ASYNC,
    USB_SHORT_GET_LOW(FS_STREAM_IN_PACKET_SIZE), USB_SHORT_GET_HIGH(FS_STREAM_IN_PACKET_SIZE),
    /* Max Packet Size */
    FS_STREAM_IN_INTERVAL, /* Interval */
};

/* Define string descriptor */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString0[] = {
    2U + 2U,
    USB_DESCRIPTOR_TYPE_STRING,
    0x09U,
    0x04U,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString1[] = {
    2U + 2U * 14U, USB_DESCRIPTOR_TYPE_STRING,
    'D',           0x00U,
    'J',           0x00U,
    'I',           0x00U,
    ' ',           0x00U,
    'R',           0x00U,
    'o',           0x00U,
    'b',           0x00U,
    'o',           0x00U,
    'M',           0x00U,
    'a',           0x00U,
    's',           0x00U,
    't',           0x00U,
    'e',           0x00U,
    'r',           0x00U,
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString2[] = {2U + 2U * 16U, USB_DESCRIPTOR_TYPE_STRING,
                                'E',           0,
                                'x',           0,
                                'p',           0,
                                'a',           0,
                                'n',           0,
                                's',           0,
                                'i',           0,
                                'o',           0,
                                'n',           0,
                                ' ',           0,
                                'M',           0,
                                'o',           0,
                                'd',           0,
                                'u',           0,
                                'l',           0,
                                'e',           0
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString3[] = {2U + 2U * 15U, USB_DESCRIPTOR_TYPE_STRING,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U,
                                '0',           0x00U
};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString4[] = {2U + 2U * 6U, USB_DESCRIPTOR_TYPE_STRING,
                                'A',           0,
                                'I',           0,
                                ' ',           0,
                                'C',           0,
                                'O',           0,
                                'M',           0,};

USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t g_UsbDeviceString5[] = {2U + 2U * 9U, USB_DESCRIPTOR_TYPE_STRING,
                                'A',           0,
                                'I',           0,
                                ' ',           0,
                                'C',           0,
                                'a',           0,
                                'm',           0,
                                'e',           0,
                                'r',           0,
                                'a',           0};

/* Define string descriptor size */
uint32_t g_UsbDeviceStringDescriptorLength[USB_DEVICE_STRING_COUNT] = {
    sizeof(g_UsbDeviceString0), sizeof(g_UsbDeviceString1), sizeof(g_UsbDeviceString2), sizeof(g_UsbDeviceString3),
    sizeof(g_UsbDeviceString4), sizeof(g_UsbDeviceString5)};

uint8_t *g_UsbDeviceStringDescriptorArray[USB_DEVICE_STRING_COUNT] = {
    g_UsbDeviceString0, g_UsbDeviceString1, g_UsbDeviceString2, g_UsbDeviceString3, g_UsbDeviceString4, g_UsbDeviceString5};

usb_language_t g_UsbDeviceLanguage[USB_DEVICE_LANGUAGE_COUNT] = {{
    g_UsbDeviceStringDescriptorArray,
    g_UsbDeviceStringDescriptorLength,
    (uint16_t)0x0409,
}};

usb_language_list_t g_UsbDeviceLanguageList = {
    g_UsbDeviceString0,
    sizeof(g_UsbDeviceString0),
    g_UsbDeviceLanguage,
    USB_DEVICE_LANGUAGE_COUNT,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief USB device get device descriptor function.
 *
 * This function gets the device descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param deviceDescriptor The pointer to the device descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetDeviceDescriptor(usb_device_handle handle,
                                           usb_device_get_device_descriptor_struct_t *deviceDescriptor)
{
    deviceDescriptor->buffer = g_UsbDeviceDescriptor;
    deviceDescriptor->length = USB_DESCRIPTOR_LENGTH_DEVICE;
    return kStatus_USB_Success;
}

/*!
 * @brief USB device get configuration descriptor function.
 *
 * This function gets the configuration descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param configurationDescriptor The pointer to the configuration descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetConfigurationDescriptor(
    usb_device_handle handle, usb_device_get_configuration_descriptor_struct_t *configurationDescriptor)
{
    if (USB_CDC_VCOM_CONFIGURE_INDEX > configurationDescriptor->configuration)
    {
        configurationDescriptor->buffer = g_UsbDeviceConfigurationDescriptor;
        configurationDescriptor->length = USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL;
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief USB device get string descriptor function.
 *
 * This function gets the string descriptor of the USB device.
 *
 * @param handle The USB device handle.
 * @param stringDescriptor Pointer to the string descriptor structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetStringDescriptor(usb_device_handle handle,
                                           usb_device_get_string_descriptor_struct_t *stringDescriptor)
{
    if (stringDescriptor->stringIndex == 0U)
    {
        stringDescriptor->buffer = (uint8_t *)g_UsbDeviceLanguageList.languageString;
        stringDescriptor->length = g_UsbDeviceLanguageList.stringLength;
    }
    else
    {
        uint8_t languageId    = 0U;
        uint8_t languageIndex = USB_DEVICE_STRING_COUNT;

        for (; languageId < USB_DEVICE_LANGUAGE_COUNT; languageId++)
        {
            if (stringDescriptor->languageId == g_UsbDeviceLanguageList.languageList[languageId].languageId)
            {
                if (stringDescriptor->stringIndex < USB_DEVICE_STRING_COUNT)
                {
                    languageIndex = stringDescriptor->stringIndex;
                }
                break;
            }
        }

        if (USB_DEVICE_STRING_COUNT == languageIndex)
        {
            return kStatus_USB_InvalidRequest;
        }
        stringDescriptor->buffer = (uint8_t *)g_UsbDeviceLanguageList.languageList[languageId].string[languageIndex];
        stringDescriptor->length = g_UsbDeviceLanguageList.languageList[languageId].length[languageIndex];
    }
    return kStatus_USB_Success;
}

/*!
 * @brief USB device set speed function.
 *
 * This function sets the speed of the USB device.
 *
 * Due to the difference of HS and FS descriptors, the device descriptors and configurations need to be updated to match
 * current speed.
 * As the default, the device descriptors and configurations are configured by using FS parameters for both EHCI and
 * KHCI.
 * When the EHCI is enabled, the application needs to call this function to update device by using current speed.
 * The updated information includes endpoint max packet size, endpoint interval, etc.
 *
 * @param handle The USB device handle.
 * @param speed Speed type. USB_SPEED_HIGH/USB_SPEED_FULL/USB_SPEED_LOW.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceSetSpeed(usb_device_handle handle, uint8_t speed)
{
    usb_descriptor_union_t *ptr1;
    usb_descriptor_union_t *ptr2;

    ptr1 = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[0]);
    ptr2 = (usb_descriptor_union_t *)(&g_UsbDeviceConfigurationDescriptor[USB_DESCRIPTOR_LENGTH_CONFIGURATION_ALL - 1]);

    while (ptr1 < ptr2)
    {
        if (ptr1->common.bDescriptorType == USB_DESCRIPTOR_TYPE_ENDPOINT)
        {
            if (USB_SPEED_HIGH == speed)
            {
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
                if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN) &&
                    (USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT ==
                     (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    ptr1->endpoint.bInterval = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
#endif
                if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN) &&
                    (USB_CDC_VCOM_BULK_IN_ENDPOINT == (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_IN_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT) &&
                         (USB_CDC_VCOM_BULK_OUT_ENDPOINT ==
                          (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_CDC_VCOM_BULK_OUT_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN ==
                     (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                    ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = HS_STREAM_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_STREAM_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else
                {
                    ptr1->endpoint.bInterval = HS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(HS_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
            }
            else
            {
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
                if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN) &&
                    (USB_CDC_VCOM_INTERRUPT_IN_ENDPOINT ==
                     (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    ptr1->endpoint.bInterval = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
#endif
                if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN) &&
                    (USB_CDC_VCOM_BULK_IN_ENDPOINT == (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_IN_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if (((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                          USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT) &&
                         (USB_CDC_VCOM_BULK_OUT_ENDPOINT ==
                          (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)))
                {
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_CDC_VCOM_BULK_OUT_PACKET_SIZE, ptr1->endpoint.wMaxPacketSize);
                }
                else if ((USB_VIDEO_VIRTUAL_CAMERA_STREAM_ENDPOINT_IN ==
                     (ptr1->endpoint.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK)) &&
                    ((ptr1->endpoint.bEndpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK) ==
                     USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN))
                {
                    ptr1->endpoint.bInterval = FS_STREAM_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_STREAM_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
                else
                {
                    ptr1->endpoint.bInterval = FS_INTERRUPT_IN_INTERVAL;
                    USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(FS_INTERRUPT_IN_PACKET_SIZE,
                                                       ptr1->endpoint.wMaxPacketSize);
                }
            }
        }
        ptr1 = (usb_descriptor_union_t *)((uint8_t *)ptr1 + ptr1->common.bLength);
    }
#if ((defined USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE) && (USB_DEVICE_CONFIG_CDC_CIC_EP_DISABLE > 0U))
#else
    for (int i = 0; i < USB_CDC_VCOM_ENDPOINT_CIC_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            g_UsbDeviceCdcVcomCicEndpoints[i].maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            g_UsbDeviceCdcVcomCicEndpoints[i].interval      = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }
        else
        {
            g_UsbDeviceCdcVcomCicEndpoints[i].maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            g_UsbDeviceCdcVcomCicEndpoints[i].interval      = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }
    }
#endif
    for (int i = 0; i < USB_CDC_VCOM_ENDPOINT_DIC_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            if (g_UsbDeviceCdcVcomDicEndpoints[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_UsbDeviceCdcVcomDicEndpoints[i].maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE;
            }
            else
            {
                g_UsbDeviceCdcVcomDicEndpoints[i].maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
            }
        }
        else
        {
            if (g_UsbDeviceCdcVcomDicEndpoints[i].endpointAddress & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)
            {
                g_UsbDeviceCdcVcomDicEndpoints[i].maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE;
            }
            else
            {
                g_UsbDeviceCdcVcomDicEndpoints[i].maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
            }
        }
    }

    if (USB_SPEED_HIGH == speed)
    {
        g_UsbDeviceVideoControlEndpoints[0].maxPacketSize = HS_INTERRUPT_IN_PACKET_SIZE;
        g_UsbDeviceVideoStreamEndpoints[0].maxPacketSize  = HS_STREAM_IN_PACKET_SIZE;

        g_UsbDeviceVideoControlEndpoints[0].interval = HS_INTERRUPT_IN_INTERVAL;
        g_UsbDeviceVideoStreamEndpoints[0].interval  = HS_STREAM_IN_INTERVAL;
    }
    else
    {
        g_UsbDeviceVideoControlEndpoints[0].maxPacketSize = FS_INTERRUPT_IN_PACKET_SIZE;
        g_UsbDeviceVideoStreamEndpoints[0].maxPacketSize  = FS_STREAM_IN_PACKET_SIZE;

        g_UsbDeviceVideoControlEndpoints[0].interval = FS_INTERRUPT_IN_INTERVAL;
        g_UsbDeviceVideoStreamEndpoints[0].interval  = FS_STREAM_IN_INTERVAL;
    }

    return kStatus_USB_Success;
}

/**
 * @brief 设置USB序列号
 *
 * @param sn
 */
void usb_set_serial_num(char* sn)
{
    int i;
    for(i = 0; i < 15; i++)
    {
        if(sn[i] == 0)
        {
            break;
        }
        g_UsbDeviceString3[2 + 2 * i] = sn[i];
    }

    if(i == 0)
    {
       g_UsbDeviceString3[0] = 4;
       g_UsbDeviceString3[2] = '0';
       g_UsbDeviceStringDescriptorLength[3] = 4;
    }
    else
    {
        g_UsbDeviceString3[0] = 2 + 2 * i;
        g_UsbDeviceStringDescriptorLength[3] = 2 + 2 * i;
    }
}
