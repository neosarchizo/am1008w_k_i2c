/*
cm1106_i2c.h -- Arduino library to control Cubic CM1106 I2C

Copyright (c) 2019 Neosarchizo.
All rights reserved.

This file is part of the library AM1008W-K I2C.

AM1008W-K I2C is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

AM1008W-K I2C is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _AM1008W_K_I2C_h
#define _AM1008W_K_I2C_h

#include <Arduino.h>
#include <Wire.h>

#define AM1008W_K_I2C_ADDRESS 0x28

#define AM1008W_K_I2C_FRAME_STX 0x16
#define AM1008W_K_I2C_LEN_READ_DATA_COMMAND 25

// #define AM1008W_K_I2C_DEBUG

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t fan_at_high_revolving_speed:1;
        uint8_t fan_at_low_revolving_speed:1;
        uint8_t working_temperature_is_high:1;
        uint8_t working_temperature_is_low:1;
    } bits;
} am1008w_k_i2c_status;

class AM1008W_K_I2C
{

public:
  void begin(TwoWire &wirePort = Wire); //By default use Wire port
  
  uint8_t send_command_data();
  uint8_t read_data_command();

  uint16_t get_co2(void);
  uint16_t get_voc(void);
  float get_humidity(void);
  float get_temperature(void);
  uint16_t get_pm1p0(void);
  uint16_t get_pm2p5(void);
  uint16_t get_pm10(void);
  uint16_t get_voc_now_r_ref_r(void);
  uint16_t get_voc_ref_r(void);
  uint16_t get_voc_now_r(void);
  am1008w_k_i2c_status get_pm_sensor_status_alarm(void);

  uint8_t pm_operation_mode;

  uint16_t co2;
  uint16_t voc;
  float humidity;
  float temperature;
  uint16_t pm1p0;
  uint16_t pm2p5;
  uint16_t pm10;
  uint16_t voc_now_r_ref_r;
  uint16_t voc_ref_r;
  uint16_t voc_now_r;
  am1008w_k_i2c_status pm_sensor_status_alarm;
  
private:
  uint8_t _buffer[32];
  TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware
};

#endif
