#include "ant_canary_IR.h"
#include "system.h"
#include "ant_canary_IR.h"
#include "comm.h"

float mlx90640To[32*24] = {0.0};
paramsMLX90640 mlx90640;

boolean isConnected() {
  Wire.beginTransmission((uint8_t)MLX90640_address);
  if (Wire.endTransmission() != 0)
    return (false); //Sensor did not ACK
  return (true);
}

void IR_setup(void) {
  Wire.begin();
  Wire.setClock(400000); //Increase I2C clock speed to 400kHz

  if (isConnected() == false) {
    output_debug_info("MLX90640 not detected.");
  } else {
    output_debug_info("MLX90640 found.");
  }

  int status;
  uint16_t eeMLX90640[832];

  status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);

  if (status != 0) {
    output_debug_info("Failed to load system parameters");
  }
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
    output_debug_info("Parameter extraction failed");

  output_debug_info("IR setup done");
}

void take_ir_image(void) {
  for (byte x = 0 ; x < 2 ; x++) { //Read both subpages
    uint16_t mlx90640Frame[834];
    int status = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);

    if (status < 0) {
      output_debug_info_int("GetFrame Error: ", status);
    }

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    float emissivity = 0.95;

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
  }

  // for (uint8_t h=0; h<24; h++) {
  //   for (uint8_t w=0; w<32; w++) {
  //     float t = mlx90640To[h*32 + w];

  //     char c = '&';
  //     if (t < 20) c = ' ';
  //     else if (t < 23) c = '.';
  //     else if (t < 25) c = '-';
  //     else if (t < 27) c = '*';
  //     else if (t < 29) c = '+';
  //     else if (t < 31) c = 'x';
  //     else if (t < 33) c = '%';
  //     else if (t < 35) c = '#';
  //     else if (t < 37) c = 'M';
  //     Serial.print(c);
  //   }
  //   Serial.println();
  // }
  // Serial.println();

  IR_ack();
}