#include <am1008w_k_i2c.h>

AM1008W_K_I2C am1008w_k_i2c;

// #define DEBUG

void setup() {
  am1008w_k_i2c.begin();
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  uint8_t ret = am1008w_k_i2c.measure_result();

  if (ret == 0) {
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
    Serial.print("PM sensor status alarm : ");
    Serial.println(am1008w_k_i2c.pm_sensor_status_alarm);

#if defined(DEBUG)
    // switch (am1008w_k_i2c.status) {
    //   case CM1106_I2C_STATUS_PREHEATING: {
    //       Serial.println("Preheating");
    //       break;
    //     }
    //   case CM1106_I2C_STATUS_NORMAL_OPERATION: {
    //       Serial.println("Normal operation");
    //       break;
    //     }
    //   case CM1106_I2C_STATUS_OPERATING_TROUBLE: {
    //       Serial.println("Operating trouble");
    //       break;
    //     }
    //   case CM1106_I2C_STATUS_OUT_OF_FS: {
    //       Serial.println("Out of FS");
    //       break;
    //     }
    //   case CM1106_I2C_STATUS_NON_CALIBRATED: {
    //       Serial.println("Non calibrated");
    //       break;
    //     }
    // }
#endif
    
  }
  delay(1000);
}