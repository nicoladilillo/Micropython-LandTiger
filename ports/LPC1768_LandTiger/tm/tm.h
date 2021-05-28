#include <time.h>

#include "time64.h"

extern void   TmUtcFromTime64  (time64 time, struct tm* ptm);
extern void   TmLocalFromTime64(time64 time, struct tm* ptm);
extern time64 TmUtcToTime64    (             struct tm* ptm);

extern void   TmUtcToLocal(struct tm* ptm);
extern void   TmFromAsciiDateTime(const char* pDate, const char* pTime, struct tm* ptm);
extern void   TmFromInteger(int year, int month, int mday, int hour, int min, int sec, struct tm* ptm);
extern int    TmSecondsBetween(struct tm* ptmLater, struct tm* ptmEarlier);
extern void   TmIncrement(struct tm* ptm);