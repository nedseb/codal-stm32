/**
  ******************************************************************************
  * @file    usbd_WebUSB_if.c
  * @author  MCD Application Team
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifdef USBCON
#ifdef USBD_USE_CDC

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usbd_desc.h"
#include "usbd_webusb_if.h"
#include "usbd_ctlreq.h"
#include "bootloader.h"




// =====================================================================
// === WEB USB =======================================================
// =================================================================

USBD_DescriptorsTypeDef USBD_Desc_webusb;


#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/* USB Device Billboard BOS descriptor Template */
#if (USBD_CLASS_BOS_ENABLED == 1)

#define USB_WEBUSB_SIZ_BOS_DESC 0x1D

__ALIGN_BEGIN  uint8_t USBD_WebUSB_BOSDesc[USB_WEBUSB_SIZ_BOS_DESC] __ALIGN_END = {
    0x05,                                /* bLength */
    USB_DESC_TYPE_BOS,                   /* Device Descriptor Type */
    USB_WEBUSB_SIZ_BOS_DESC, 0x00,       /* Total length of BOS descriptor and all of its sub descs */
    0x01,                                /* The number of separate device capability descriptors in the BOS */

    /* ----------- Device Capability Descriptor: PLATFORM ---------- */
    0x18,       //.bLength
    0x10,       //.bDescriptorType
    0x05,       //.bDevCapabilityType
    0x00,       //.bReserved
    0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47, 0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,     //.PlatformCapabilityUUID
    0x01, 0x00, //.bcdVersion
    0x02,
    0x00,
};
#endif


#if (USBD_CLASS_USER_STRING_DESC == 1)
/**
  * @brief  Returns the Class User string descriptor.
  * @param  speed: Current device speed
  * @param  idx: index of string descriptor
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t *USBD_WebUSB_Class_UserStrDescriptor(USBD_SpeedTypeDef speed, uint8_t idx, uint16_t *length)
{
    UNUSED(speed);
    
    switch(idx){
        case 10:
            USBD_GetString((uint8_t *)"WebUSB Interface", USBD_StrDesc, length);
            break;

        default:
            USBD_GetString((uint8_t *)"UNKNOWN STRING", USBD_StrDesc, length);
            break;
    }

    return USBD_StrDesc;
}
#endif

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
/**
  * @brief  USBD_USR_BOSDescriptor
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_WebUSB_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_WebUSB_BOSDesc);
  return (uint8_t *)USBD_WebUSB_BOSDesc;
}
#endif





// =====================================================================
// === CDC ===========================================================
// =================================================================

#ifdef USE_USB_HS
  #define WebUSB_MAX_PACKET_SIZE USB_OTG_HS_MAX_PACKET_SIZE
#elif defined(USB_OTG_FS) || defined(USB_OTG_FS_MAX_PACKET_SIZE)
  #define WebUSB_MAX_PACKET_SIZE USB_OTG_FS_MAX_PACKET_SIZE
#else /* USB */
  #define WebUSB_MAX_PACKET_SIZE USB_MAX_EP0_SIZE
#endif

/*
 * The value USB_WebUSB_TRANSMIT_TIMEOUT is defined in terms of HAL_GetTick() units.
 * Typically it is 1ms value. The timeout determines when we would consider the
 * host "too slow" and threat the USB CDC port as disconnected.
 */
#ifndef USB_WebUSB_TRANSMIT_TIMEOUT
  #define USB_WebUSB_TRANSMIT_TIMEOUT 3
#endif

/* USBD_CDC Private Variables */
/* USB Device Core CDC handle declaration */
USBD_HandleTypeDef hUSBD_Device_WebUSB;

static bool WebUSB_initialized = false;

/* Received Data over USB are stored in this buffer       */
CDC_TransmitQueue_TypeDef WebUSB_TransmitQueue;
CDC_ReceiveQueue_TypeDef WebUSB_ReceiveQueue;
__IO uint32_t lineState_webusb = 0;
__IO bool receivePended_webusb = true;
static uint32_t transmitStart = 0;

#ifdef DTR_TOGGLING_SEQ
  /* DTR toggling sequence management */
  extern void dtr_togglingHook(uint8_t *buf, uint32_t *len);
  uint8_t dtr_toggling = 0;
#endif

/** USBD_CDC Private Function Prototypes */

static int8_t USBD_WebUSB_Init(void);
static int8_t USBD_WebUSB_DeInit(void);
static int8_t USBD_WebUSB_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t USBD_WebUSB_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t USBD_WebUSB_TransmitCplt(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

USBD_WebUSB_ItfTypeDef USBD_WebUSB_fops = {
  USBD_WebUSB_Init,
  USBD_WebUSB_DeInit,
  USBD_WebUSB_Control,
  USBD_WebUSB_Receive,
  USBD_WebUSB_TransmitCplt
};

USBD_WebUSB_LineCodingTypeDef linecoding_webusb = {
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};

void (*webusb_rx_callback)(void) = NULL;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  USBD_WebUSB_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t USBD_WebUSB_Init(void)
{
  /* Set Application Buffers */
  CDC_TransmitQueue_Init(&WebUSB_TransmitQueue);
  CDC_ReceiveQueue_Init(&WebUSB_ReceiveQueue);
  receivePended_webusb = true;
  USBD_WebUSB_SetRxBuffer(&hUSBD_Device_WebUSB, CDC_ReceiveQueue_ReserveBlock(&WebUSB_ReceiveQueue));

  return ((int8_t)USBD_OK);
}

/**
  * @brief  USBD_WebUSB_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t USBD_WebUSB_DeInit(void)
{
  return ((int8_t)USBD_OK);
}


/**
  * @brief  USBD_WebUSB_Control
  *         Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t USBD_WebUSB_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  UNUSED(length);
  switch (cmd) {
    case WebUSB_SEND_ENCAPSULATED_COMMAND:

      break;

    case WebUSB_GET_ENCAPSULATED_RESPONSE:

      break;

    case WebUSB_SET_COMM_FEATURE:

      break;

    case WebUSB_GET_COMM_FEATURE:

      break;

    case WebUSB_CLEAR_COMM_FEATURE:

      break;

    /*******************************************************************************/
    /* Line Coding Structure                                                       */
    /*-----------------------------------------------------------------------------*/
    /* Offset | Field       | Size | Value  | Description                          */
    /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
    /* 4      | bCharFormat |   1  | Number | Stop bits                            */
    /*                                        0 - 1 Stop bit                       */
    /*                                        1 - 1.5 Stop bits                    */
    /*                                        2 - 2 Stop bits                      */
    /* 5      | bParityType |  1   | Number | Parity                               */
    /*                                        0 - None                             */
    /*                                        1 - Odd                              */
    /*                                        2 - Even                             */
    /*                                        3 - Mark                             */
    /*                                        4 - Space                            */
    /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
    /*******************************************************************************/
    case WebUSB_SET_LINE_CODING:
      linecoding_webusb.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | \
                                         (pbuf[2] << 16) | (pbuf[3] << 24));
      linecoding_webusb.format     = pbuf[4];
      linecoding_webusb.paritytype = pbuf[5];
      linecoding_webusb.datatype   = pbuf[6];
      break;

    case WebUSB_GET_LINE_CODING:
      pbuf[0] = (uint8_t)(linecoding_webusb.bitrate);
      pbuf[1] = (uint8_t)(linecoding_webusb.bitrate >> 8);
      pbuf[2] = (uint8_t)(linecoding_webusb.bitrate >> 16);
      pbuf[3] = (uint8_t)(linecoding_webusb.bitrate >> 24);
      pbuf[4] = linecoding_webusb.format;
      pbuf[5] = linecoding_webusb.paritytype;
      pbuf[6] = linecoding_webusb.datatype;
      break;

    case WebUSB_SET_CONTROL_LINE_STATE:
      lineState_webusb =
        (((USBD_SetupReqTypedef *)pbuf)->wValue & 0x01) != 0; // Check DTR state
      if (lineState_webusb) { // Reset the transmit timeout when the port is connected
        transmitStart = 0;
      }
#ifdef DTR_TOGGLING_SEQ
      dtr_toggling++; /* Count DTR toggling */
#endif
      break;

    case WebUSB_SEND_BREAK:

      break;

    default:
      break;
  }

  return ((int8_t)USBD_OK);
}

/**
  * @brief  USBD_WebUSB_Receive
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t USBD_WebUSB_Receive(uint8_t *Buf, uint32_t *Len)
{
#ifdef DTR_TOGGLING_SEQ
  if (dtr_toggling > 3) {
    dtr_togglingHook(Buf, Len);
    dtr_toggling = 0;
  }
#else
  UNUSED(Buf);
#endif
  /* It always contains required amount of free space for writing */
  CDC_ReceiveQueue_CommitBlock(&WebUSB_ReceiveQueue, (uint16_t)(*Len));
  receivePended_webusb = false;
  /* If enough space in the queue for a full buffer then continue receive */
  if (!webUSB_resume_receive()) {
    USBD_WebUSB_ClearBuffer(&hUSBD_Device_WebUSB);
  }

  if(webusb_rx_callback != NULL){
    webusb_rx_callback();
  }

  return ((int8_t)USBD_OK);
}


/**
  * @brief  USBD_WebUSB_TransmitCplt
  *         Data transmited callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t USBD_WebUSB_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  transmitStart = 0;
  CDC_TransmitQueue_CommitRead(&WebUSB_TransmitQueue);
  webUSB_continue_transmit();
  return ((int8_t)USBD_OK);
}

void webUSB_init(void)
{
    USBD_Desc_webusb.GetDeviceDescriptor = USBD_Desc.GetDeviceDescriptor;
    USBD_Desc_webusb.GetLangIDStrDescriptor = USBD_Desc.GetLangIDStrDescriptor;
    USBD_Desc_webusb.GetManufacturerStrDescriptor = USBD_Desc.GetManufacturerStrDescriptor;
    USBD_Desc_webusb.GetProductStrDescriptor = USBD_Desc.GetProductStrDescriptor;
    USBD_Desc_webusb.GetSerialStrDescriptor = USBD_Desc.GetSerialStrDescriptor;
    USBD_Desc_webusb.GetConfigurationStrDescriptor = USBD_Desc.GetConfigurationStrDescriptor;
    USBD_Desc_webusb.GetInterfaceStrDescriptor = USBD_Desc.GetInterfaceStrDescriptor;
    
#if (USBD_CLASS_USER_STRING_DESC == 1)
    USBD_Desc_webusb.GetUserStrDescriptor = USBD_WebUSB_Class_UserStrDescriptor;
#endif

#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
    USBD_Desc_webusb.GetBOSDescriptor = USBD_WebUSB_USR_BOSDescriptor;
#endif

  if (!WebUSB_initialized) {
    /* Init Device Library */
    if (USBD_Init(&hUSBD_Device_WebUSB, &USBD_Desc_webusb, 0) == USBD_OK) {
      /* Add Supported Class */
      if (USBD_RegisterClass(&hUSBD_Device_WebUSB, USBD_WebUSB_CLASS) == USBD_OK) {
        /* Add CDC Interface Class */
        if (USBD_WebUSB_RegisterInterface(&hUSBD_Device_WebUSB, &USBD_WebUSB_fops) == USBD_OK) {
          /* Start Device Process */
          USBD_Start(&hUSBD_Device_WebUSB);
          WebUSB_initialized = true;
        }
      }
    }
  }
}

void webUSB_deInit(void)
{
  if (WebUSB_initialized) {
    USBD_Stop(&hUSBD_Device_WebUSB);
    USBD_WebUSB_DeInit();
    USBD_DeInit(&hUSBD_Device_WebUSB);
    WebUSB_initialized = false;
  }
}

bool webUSB_connected()
{
  /* Save the transmitStart value in a local variable to avoid twice reading - fix #478 */
  uint32_t transmitTime = transmitStart;
  if (transmitTime) {
    transmitTime = HAL_GetTick() - transmitTime;
  }
  return ((hUSBD_Device_WebUSB.dev_state == USBD_STATE_CONFIGURED)
          && (transmitTime < USB_WebUSB_TRANSMIT_TIMEOUT)
          && lineState_webusb);
}

void webUSB_continue_transmit(void)
{
  uint16_t size;
  uint8_t *buffer;
  USBD_WebUSB_HandleTypeDef *hcdc = (USBD_WebUSB_HandleTypeDef *) hUSBD_Device_WebUSB.pClassData;
  /*
   * TS: This method can be called both in the main thread
   * (via USBSerial::write) and in the IRQ stream (via USBD_WebUSB_TransmistCplt),
   * BUT the main thread cannot pass this condition while waiting for a IRQ!
   * This is not possible because TxState is not zero while waiting for data
   * transfer ending! The IRQ thread is uninterrupted, since its priority
   * is higher than that of the main thread. So this method is thread safe.
   */
  if (hcdc->TxState == 0U) {
    buffer = CDC_TransmitQueue_ReadBlock(&WebUSB_TransmitQueue, &size);
    if (size > 0) {
      transmitStart = HAL_GetTick();
      USBD_WebUSB_SetTxBuffer(&hUSBD_Device_WebUSB, buffer, size);
      /*
       * size never exceed PMA buffer and USBD_WebUSB_TransmitPacket make full
       * copy of block in PMA, so no need to worry about buffer damage
       */
      USBD_WebUSB_TransmitPacket(&hUSBD_Device_WebUSB);
    }
  }
}

bool webUSB_resume_receive(void)
{
  /*
   * TS: main and IRQ threads can't pass it at same time, because
   * IRQ may occur only if receivePended_webusb is true. So it is thread-safe!
   */
  if (!receivePended_webusb) {
    uint8_t *block = CDC_ReceiveQueue_ReserveBlock(&WebUSB_ReceiveQueue);
    if (block != NULL) {
      receivePended_webusb = true;
      /* Set new buffer */
      USBD_WebUSB_SetRxBuffer(&hUSBD_Device_WebUSB, block);
      USBD_WebUSB_ReceivePacket(&hUSBD_Device_WebUSB);
      return true;
    }
  }
  return false;
}


void webUSB_OnDataReceived( void (*callback)(void) ){
  webusb_rx_callback = callback;
}

#endif /* USBD_USE_CDC */
#endif /* USBCON */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

