/*----------------------------------------------------------------------------
 *      Name:    vcomdemo.c
 *      Purpose: USB virtual COM port Demo
 *      Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else 
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "LPC17xx.h"    
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"
#include "serial.h"
#include "vcomdemo.h"


/*----------------------------------------------------------------------------
 Initialises the VCOM port.
 Call this function before using VCOM_putchar or VCOM_getchar
 *---------------------------------------------------------------------------*/
void VCOM_Init(void) {
#if PORT_NUM
  CDC_Init (1);
#else
  CDC_Init (0);
#endif
}


/*----------------------------------------------------------------------------
  Reads character from serial port buffer and writes to USB buffer
 *---------------------------------------------------------------------------*/
void VCOM_Serial2Usb(void) {
  static char serBuf [USB_CDC_BUFSIZE];
         int  numBytesRead, numAvailByte;
	
  ser_AvailChar (&numAvailByte);
  if (numAvailByte > 0) {
	  numBytesRead = ser_Read (&serBuf[0], &numAvailByte);

	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)&serBuf[0], numBytesRead);
  }
  
}

/*----------------------------------------------------------------------------
  Reads character from USB buffer and writes to serial port buffer
 *---------------------------------------------------------------------------*/
void VCOM_Usb2Serial(void) {
  static char serBuf [USB_CDC_BUFSIZE];
  int  numBytesRead, numAvailByte;
	
  ser_AvailChar (&numAvailByte);
  if (numAvailByte > 0) {
    numBytesRead = ser_Read (&serBuf[0], &numAvailByte);

	  USB_WriteEP (CDC_DEP_IN, (unsigned char *)&serBuf[0], numBytesRead);
  }

}


/*----------------------------------------------------------------------------
  checks the serial state and initiates notification
 *---------------------------------------------------------------------------*/
void VCOM_CheckSerialState (void) {
         unsigned short temp;
  static unsigned short serialState;

  temp = CDC_GetSerialState();
  if (serialState != temp) {
     serialState = temp;
     CDC_NotificationIn();                  // send SERIAL_STATE notification
  }
}


