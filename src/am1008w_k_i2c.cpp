/*
am1008w_k_i2c.cpp -- Arduino library to control Cubic AM1008W-K I2C

Copyright (c) 2018 Neosarchizo.
All rights reserved.

This file is part of the library AM1108W-K I2C.

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

#include "am1008w_k_i2c.h"

/**
 * Read result of measuring
 * @return {@code 0} Reading AM1008W-K value succeeded
 *         {@code 1} Buffer(index) is short
 *         {@code 2} Frame header is different
 *         {@code 3} Frame length is different
 *         {@code 4} Checksum is wrong
 */
void AM1008W_K_I2C::begin(TwoWire &wirePort)
{
  _i2cPort = &wirePort; //Grab which port the user wants us to use

  //We expect caller to begin their I2C port, with the speed of their choice external to the library
  //But if they forget, we start the hardware here.
  _i2cPort->begin();
}

uint8_t AM1008W_K_I2C::send_command_data(uint8_t ctrl, uint16_t data)
{
  uint8_t tx_data[7] = {0};

  tx_data[0] = AM1008W_K_I2C_FRAME_STX;
  tx_data[1] = 7; // FRAME LENGTH
  tx_data[2] = ctrl; // Control Command of the PM Sensor
  tx_data[3] = (data >> 8) & 0xff; // high byte of data
  tx_data[4] = data & 0xff; // low byte of data
  tx_data[5] = 0;

  uint8_t cs = tx_data[0];

  for (uint8_t i = 1; i < 6; i++)
  {
    cs ^= tx_data[i];
  }

  tx_data[6] = cs;
  
  _i2cPort->beginTransmission(AM1008W_K_I2C_ADDRESS);
  _i2cPort->write(tx_data, 7);
  _i2cPort->endTransmission();

  return 0;
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_close(void)
{
  return send_command_data(AM1008W_K_I2C_CTRL_CLOSE_MEASUREMENT, 0);
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_open(void)
{
  return send_command_data(AM1008W_K_I2C_CTRL_OPEN_MEASUREMENT, 0);
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_single_measurement(uint16_t range)
{
  return send_command_data(AM1008W_K_I2C_CTRL_SET_UP_SINGLE_MEASUREMENT, range);
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_cotinuously_measurement(void)
{
  return send_command_data(AM1008W_K_I2C_CTRL_SET_UP_CONTINUOUSLY_MEASUREMENT, 0);
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_timing_measurement(uint16_t range)
{
  return send_command_data(AM1008W_K_I2C_CTRL_SET_UP_TIMING_MEASUREMENT, range);
}

uint8_t AM1008W_K_I2C::pm_sensor_mode_dynamic_measurment(void)
{
  return send_command_data(AM1008W_K_I2C_CTRL_SET_UP_DYNAMIC_MEASUREMENT, 0);
}

uint8_t AM1008W_K_I2C::set_pm_coefficient(uint8_t range)
{
  return send_command_data(AM1008W_K_I2C_CTRL_SET_UP_PM_CALIBRATION_COEFFICIENT, range);
}

uint8_t AM1008W_K_I2C::calibrate_co2(uint16_t range)
{
  return send_command_data(AM1008W_K_I2C_CTRL_CO2_CALIBRATION, range);
}

uint8_t AM1008W_K_I2C::read_data_command(void)
{
  _i2cPort->requestFrom(AM1008W_K_I2C_ADDRESS, 25);
  uint8_t idx = 0;

  while (_i2cPort->available())
  { // slave may send less than requested
    uint8_t b = _i2cPort->read();
    _buffer[idx++] = b;
    if (idx == AM1008W_K_I2C_LEN_READ_DATA_COMMAND)
    {
      break;
    }
  }

  if (idx < AM1008W_K_I2C_LEN_READ_DATA_COMMAND)
  {
#ifdef AM1008W_K_I2C_DEBUG
    Serial.println("AM1008W_K_I2C::read_data_command : buffer is short!");
#endif
    return 1;
  }

  if (_buffer[0] != AM1008W_K_I2C_FRAME_STX)
  {
#ifdef AM1008W_K_I2C_DEBUG
    Serial.print("AM1008W_K_I2C::read_data_command : frame STX is different ");
    Serial.println(_buffer[0], HEX);
#endif
    return 2;
  }

  if (_buffer[1] != AM1008W_K_I2C_LEN_READ_DATA_COMMAND)
  {
#ifdef AM1008W_K_I2C_DEBUG
    Serial.println("AM1008W_K_I2C::read_data_command : frame length is not AM1008W_K_I2C_LEN_READ_DATA_COMMAND");
#endif
    return 3;
  }

  uint8_t check_code = _buffer[0];

  for (uint8_t i = 1; i < AM1008W_K_I2C_LEN_READ_DATA_COMMAND - 1; i++)
  {
    check_code ^= _buffer[i];
  }

  if (_buffer[AM1008W_K_I2C_LEN_READ_DATA_COMMAND - 1] != check_code)
  {
#ifdef AM1008W_K_I2C_DEBUG
    Serial.print("AM1008W_K_I2C::read_data_command : check code is different - _buffer[");
    Serial.print(AM1008W_K_I2C_LEN_READ_DATA_COMMAND - 1);
    Serial.print("] : ");
    Serial.print(_buffer[AM1008W_K_I2C_LEN_READ_DATA_COMMAND - 1], HEX);
    Serial.print(", check_code : ");
    Serial.println(check_code, HEX);
#endif
    return 4;
  }

  _pm_operation_mode = _buffer[2];
  _co2 = (_buffer[3] << 8) + _buffer[4];
  _voc = (_buffer[5] << 8) + _buffer[6];
  _humidity = (float)((_buffer[7] << 8) + _buffer[8]) / 10;
  _temperature = (float)((_buffer[9] << 8) + _buffer[10] - 500) / 10;
  _pm1p0 = (_buffer[11] << 8) + _buffer[12];
  _pm2p5 = (_buffer[13] << 8) + _buffer[14];
  _pm10 = (_buffer[15] << 8) + _buffer[16];
  _voc_now_r_ref_r = (_buffer[17] << 8) + _buffer[18];
  _voc_ref_r = (_buffer[19] << 8) + _buffer[20];
  _voc_now_r = (_buffer[21] << 8) + _buffer[22];
  _pm_sensor_status_alarm.value = _buffer[23];

  return 0;
}

/**
 * PM Operation Mode
 * @return PM Operation Mode
 */
uint8_t AM1008W_K_I2C::get_pm_operation_mode(void)
{
  return _pm_operation_mode;
}


/**
 * CO2
 * @return CO2
 */
uint16_t AM1008W_K_I2C::get_co2(void)
{
  return _co2;
}

/**
 * VOC
 * @return VOC
 */
uint16_t AM1008W_K_I2C::get_voc(void)
{
  return _voc;
}

/**
 * Humidity
 * @return Humidity
 */
float AM1008W_K_I2C::get_humidity(void)
{
  return _humidity;
}

/**
 * Temperature
 * @return Temperature
 */
float AM1008W_K_I2C::get_temperature(void)
{
  return _temperature;
}

/**
 * PM1.0
 * @return PM1.0
 */
uint16_t AM1008W_K_I2C::get_pm1p0(void)
{
  return _pm1p0;
}

/**
 * PM2.5
 * @return PM2.5
 */
uint16_t AM1008W_K_I2C::get_pm2p5(void)
{
  return _pm2p5;
}

/**
 * PM10
 * @return PM10
 */
uint16_t AM1008W_K_I2C::get_pm10(void)
{
  return _pm10;
}

/**
 * VOC Now R / Ref. R (%)
 * @return VOC Now R / Ref. R (%)
 */
uint16_t AM1008W_K_I2C::get_voc_now_r_ref_r(void)
{
  return _voc_now_r_ref_r;
}

/**
 * VOC Ref. R Value
 * @return VOC Ref. R Value
 */
uint16_t AM1008W_K_I2C::get_voc_ref_r(void)
{
  return _voc_ref_r;
}

/**
 * VOC Now R Value
 * @return VOC Now R Value
 */
uint16_t AM1008W_K_I2C::get_voc_now_r(void)
{
  return _voc_now_r;
}

/**
 * PM sensor status alarm
 * @return PM sensor status alarm
 */
am1008w_k_i2c_status AM1008W_K_I2C::get_pm_sensor_status_alarm(void)
{
  return _pm_sensor_status_alarm;
}
