/*
am1108w_k_i2c.h -- Arduino library to control Cubic AM1108W-K I2C

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

#define AM1008W_K_I2C_PSO_CLOSE 0x00
#define AM1008W_K_I2C_PSO_TIMING_MEASURING_MODE 0x05
#define AM1008W_K_I2C_PSO_DYNAMIC_MEASURING_MODE 0x06
#define AM1008W_K_I2C_PSO_CONTINOUS_MEASURING_MODE 0x0C
#define AM1008W_K_I2C_PSO_SINGLE_MEASURING_MODE 0x0D

#define AM1008W_K_I2C_CTRL_CLOSE_MEASUREMENT 0x01
#define AM1008W_K_I2C_CTRL_OPEN_MEASUREMENT 0x02
#define AM1008W_K_I2C_CTRL_CO2_CALIBRATION 0x03
#define AM1008W_K_I2C_CTRL_SET_UP_TIMING_MEASUREMENT 0x05
#define AM1008W_K_I2C_CTRL_SET_UP_DYNAMIC_MEASUREMENT 0x06
#define AM1008W_K_I2C_CTRL_SET_UP_PM_CALIBRATION_COEFFICIENT 0x07
#define AM1008W_K_I2C_CTRL_SET_UP_CONTINUOUSLY_MEASUREMENT 0x0C
#define AM1008W_K_I2C_CTRL_SET_UP_SINGLE_MEASUREMENT 0x0D

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
  
  uint8_t pm_sensor_mode_close(void);
  uint8_t pm_sensor_mode_open(void);
  uint8_t pm_sensor_mode_single_measurement(uint16_t range);
  uint8_t pm_sensor_mode_cotinuously_measurement(void);
  uint8_t pm_sensor_mode_timing_measurement(uint16_t range);
  uint8_t pm_sensor_mode_dynamic_measurment(void);

  uint8_t set_pm_coefficient(uint8_t range);
  uint8_t calibrate_co2(uint16_t range);

  uint8_t read_data_command(void);

  uint8_t get_pm_operation_mode(void);
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
  
private:
  uint8_t _buffer[32];
  TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware

  uint8_t _pm_operation_mode;
  uint16_t _co2;
  uint16_t _voc;
  float _humidity;
  float _temperature;
  uint16_t _pm1p0;
  uint16_t _pm2p5;
  uint16_t _pm10;
  uint16_t _voc_now_r_ref_r;
  uint16_t _voc_ref_r;
  uint16_t _voc_now_r;
  am1008w_k_i2c_status _pm_sensor_status_alarm;

  uint8_t send_command_data(uint8_t ctrl, uint16_t data);
};

#endif
