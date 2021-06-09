#include <am1008w_k_i2c.h>

AM1008W_K_I2C am1008w_k_i2c;

// #define AM1008W_K_I2C_DEBUG

void setup() {
  am1008w_k_i2c.begin();
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  uint8_t ret = am1008w_k_i2c.read_data_command();

  if (ret == 0) {
    Serial.print("PM sensor operation mode : ");
    Serial.println(am1008w_k_i2c.pm_operation_mode, HEX);

    Serial.print("Co2 : ");
    Serial.println(am1008w_k_i2c.co2);
    Serial.print("VOC : ");
    Serial.println(am1008w_k_i2c.voc);
    Serial.print("Humidity : ");
    Serial.println(am1008w_k_i2c.humidity);
    Serial.print("Temperature : ");
    Serial.println(am1008w_k_i2c.temperature);
    Serial.print("PM1.0 : ");
    Serial.println(am1008w_k_i2c.pm1p0);
    Serial.print("PM2.5 : ");
    Serial.println(am1008w_k_i2c.pm2p5);
    Serial.print("PM10 : ");
    Serial.println(am1008w_k_i2c.pm10);
    Serial.print("VOC Now R / Ref. R (%) : ");
    Serial.println(am1008w_k_i2c.voc_now_r_ref_r);
    Serial.print("VOC Ref. R Value : ");
    Serial.println(am1008w_k_i2c.voc_ref_r);
    Serial.print("VOC Now R Value : ");
    Serial.println(am1008w_k_i2c.voc_now_r);
    
    Serial.println("PM sensor status alarm : (0 = false, 1 = true) >>");
    Serial.print("Fan at high revolving speed : ");
    Serial.println(am1008w_k_i2c.pm_sensor_status_alarm.bits.fan_at_high_revolving_speed);
    Serial.print("Fan at low revolving speed : ");
    Serial.println(am1008w_k_i2c.pm_sensor_status_alarm.bits.fan_at_low_revolving_speed);
    Serial.print("Working temperature is high : ");
    Serial.println(am1008w_k_i2c.pm_sensor_status_alarm.bits.working_temperature_is_high);
    Serial.print("Working temperature is low : ");
    Serial.println(am1008w_k_i2c.pm_sensor_status_alarm.bits.working_temperature_is_low);
  }
  delay(1000);
}