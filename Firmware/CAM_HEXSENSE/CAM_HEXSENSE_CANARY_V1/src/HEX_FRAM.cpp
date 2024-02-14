#include <Arduino.h>

#include "Adafruit_FRAM_I2C.h"


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// FRAM write and read pointers
extern uint16_t FRAM_r_P;
extern uint16_t FRAM_w_P;

// ===================== FRAM =====================
extern Adafruit_FRAM_I2C fram;

extern void fram_setup(void);
extern void reset_fram_pointer(void);

extern void write_ant_packet_to_fram(uint16_t w_pointer);

extern void calc_FRAM_w_P();
extern void calc_FRAM_r_P();

extern void read_an_ant_packet(uint16_t FRAM_w_P);

#ifdef __cplusplus
}
#endif
