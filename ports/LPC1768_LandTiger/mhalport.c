#include <string.h>

#include "py/runtime.h"
#include "py/stream.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "extmod/misc.h"

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


// Send the string of given length.
void mp_hal_stdout_tx_strn(const char *serBuf, size_t numBytesRead) {
  riceive_Data(serBuf, (int) numBytesRead);
  VCOM_Serial2Usb();
}


// Receive single character from USB
int mp_hal_stdin_rx_chr(void) {
  static char serBuf [32];
  int  numBytesToRead, numBytesRead, numAvailByte;

	numAvailByte = 0;
	while (numAvailByte <= 0)  {
		CDC_OutBufAvailChar (&numAvailByte);
	}
	
  if (numAvailByte > 0) {
      numBytesToRead = 1;
      numBytesRead = CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
  }
  return serBuf[0];
}