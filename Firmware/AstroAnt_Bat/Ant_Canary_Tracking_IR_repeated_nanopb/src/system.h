#pragma once

#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define ANT_ID 0

#define TIMER_INTERVAL_100ms 100L

extern volatile bool timer4Interrupt_100ms;

#define ANT_NAME     "AntCanary - #0"

#define BAT_V_PIN    A0

#define SERIAL_DEBUG false

#define HW_TIMER_INTERVAL_MS 1
#define TIMER_INTERVAL_100ms 100L

#define BLE_LED_PIN 16
#define MSG_LED_PIN 10

extern void Serialsetup(void);

extern void output_debug_info(const char *info);
extern void output_debug_info_int(const char *info, int value);
extern void output_debug_info_float(const char *info, float value);

extern void system_setup(void);
extern void flash_led(uint8_t pin, uint8_t times, uint16_t interval);

extern uint32_t read_bat_V(void);

#ifdef __cplusplus
}
#endif

