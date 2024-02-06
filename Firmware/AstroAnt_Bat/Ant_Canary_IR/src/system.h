#pragma once

#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define BAT_V_PIN    A0

#define SERIAL_DEBUG true

#define HW_TIMER_INTERVAL_MS 1
#define TIMER_INTERVAL_100ms 100L

extern void Serialsetup(void);

extern void output_debug_info(const char *info);
extern void output_debug_info_int(const char *info, int value);
extern void output_debug_info_float(const char *info, float value);

extern uint32_t read_bat_V(void);

#ifdef __cplusplus
}
#endif

