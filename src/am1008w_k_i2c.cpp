/*
pm2008_i2c.cpp -- Arduino library to control Cubic PM2008 I2C

Copyright (c) 2018 Neosarchizo.
All rights reserved.

This file is part of the library CM1106 I2C.

CM1106 I2C is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CM1106 I2C is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "am1008w_k_i2c.h"

/**
 * Start AM1008W-K I2C library
 */
void AM1008W_K_I2C::begin(TwoWire &wirePort)
{
  _i2cPort = &wirePort; //Grab which port the user wants us to use

  //We expect caller to begin their I2C port, with the speed of their choice external to the library
  //But if they forget, we start the hardware here.
  _i2cPort->begin();
}


uint8_t AM1008W_K_I2C::send_command_data()
{
  return 0;
}

uint8_t AM1008W_K_I2C::read_data_command()
{
  _i2cPort->requestFrom(AM1008W_K_I2C_ADDRESS, 25);
  uint8_t idx = 0;

  while (_i2cPort->available())
  { // slave may send less than requested
    uint8_t b = _i2cPort->read();
    _buffer[idx++] = b;
    if (idx == AM1008W_K_I2C_LEN_READ_DATA_COMMAND) {
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

  if (_buffer[1] != AM1008W_K_I2C_LEN_READ_DATA_COMMAND) {
#ifdef AM1008W_K_I2C_DEBUG
    Serial.println("AM1008W_K_I2C::read_data_command : frame length is not AM1008W_K_I2C_LEN_READ_DATA_COMMAND");
#endif
    return 3;
  }

  pm_operation_mode = _buffer[2];
  
  return 0;
}