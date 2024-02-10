#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

#define MLX90640_address 0x33
#define TA_SHIFT         8 //Default shift for MLX90640 in open air

extern float mlx90640To[32*24]; // buffer for full frame of temperatures
extern paramsMLX90640 mlx90640;

extern void IR_setup(void);
extern void take_ir_image(void);