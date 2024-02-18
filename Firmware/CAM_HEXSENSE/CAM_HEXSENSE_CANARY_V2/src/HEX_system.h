#include <Arduino.h>

#define SERIAL_DEBUG true

extern void serialport_setup(void);
extern void output_debug_info(const char *info);
extern void output_debug_info_int(const char *info, int value);
extern void output_debug_info_float(const char *info, float value);
extern void output_debug_info_int32(const char *info, uint32_t value);