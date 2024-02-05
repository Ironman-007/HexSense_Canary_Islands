#pragma once

#include <Arduino.h>
#include <SPI.h>

// FZ: register map, stole from ArduCAM Arduino library
#define CAM_REG_POWER_CONTROL      0X02
#define ARDUCHIP_FIFO              0x04 // FIFO and I2C control
#define CAM_REG_SENSOR_RESET       0X07
#define CAM_REG_FORMAT             0X20
#define CAM_REG_CAPTURE_RESOLUTION 0X21
#define ARDUCHIP_TRIG              0x44 // Trigger source
#define FIFO_SIZE1                 0x45 // Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2                 0x46 // Camera write FIFO size[15:8]
#define FIFO_SIZE3                 0x47 // Camera write FIFO size[18:16]

#define FIFO_CLEAR_ID_MASK         0x01 // cameraClearFifoFlag
#define FIFO_START_MASK            0x02
#define FIFO_CLEAR_MASK            0x80
#define CAP_DONE_MASK              0x04

#define CAMERA_CS 10

class FZ_ArduCAM_Mega {
private:
    int      _cs;
    uint32_t _total_length;
public:
    FZ_ArduCAM_Mega(int pin_cs = CAMERA_CS);
    ~FZ_ArduCAM_Mega();

    void set_CS_pin(int pin_cs);

    void write_register(uint8_t addr, uint8_t data);
    uint8_t read_register(uint8_t addr);
    uint8_t getBit(uint8_t addr, uint8_t bit);

    void set_format(uint8_t fmt);
    void set_resolution(uint8_t resolution);

    uint32_t ReadFifoLength(void);

    void SetCapture(void);

    uint32_t getTotalLength(void);
};

