#include <unistd.h>
#include "py/mpconfig.h"

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
#include "delay.h"


// Send the string of given length.
void mp_hal_stdout_tx_strn(const char *serBuf, size_t numBytesRead) {
  int i, j = 0;
  while (j < numBytesRead) {
    if (j+10 < numBytesRead)
      i = 10;
    else
      i = (numBytesRead - j);
    write_data_in(&serBuf[j], i);
    DELAY_ms(1);
    VCOM_Serial2Usb();
    j += 10;
  }
}


// Receive single character from USB
int mp_hal_stdin_rx_chr(void) {
  static char serBuf [32];
  int  numBytesToRead, numAvailByte;

	numAvailByte = 0;
	while (numAvailByte <= 0)  {
		CDC_OutBufAvailChar (&numAvailByte);
	}
	
  if (numAvailByte > 0) {
      numBytesToRead = 1;
      CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
  }
  return serBuf[0];
}
