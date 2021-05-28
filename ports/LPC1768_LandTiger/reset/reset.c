#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "rsid.h"
#include "restart.h"
#include "watchdog.h"
#include "led.h"
#include "mstimer.h"

bool ResetAccepted = false;

bool ResetWasPushButton()
{
    return RsidExtr && RestartGetLastCause() == RESTART_CAUSE_RESET_BUTTON;
}
bool ResetHasAlarm()
{
    if (ResetAccepted) return false;
    if (RsidExtr)
    {
        if (RestartGetLastCause() == RESTART_CAUSE_RESET_BUTTON  ) return false;
        if (RestartGetLastCause() == RESTART_CAUSE_SOFTWARE_RESET) return false;
    }
    return true;
}
void ResetInit()
{
        RsidInit();
     RestartInit();
    WatchdogInit();
}
void ResetMain()
{
    //Feed the watchdog
    WatchdogMain();
    
    //Flash an led
    static uint32_t ledFlashTimer = 0;
    if (ResetHasAlarm())
    {
        if (MsTimerRepetitive(&ledFlashTimer, 100)) Led4Tgl();
    }
    else
    {
        Led4Set(false);
    }
}
