void IMUSTREG (){
  myIMU.begin();   
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL3_C, 0x01);         // reset
  delay(100);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL1_XL, 0x40);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_FIFO_CTRL2, 0x08);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL8_XL, 0x04);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_CFG1, 0xBE );               
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_THS_6D, 0x06);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT_DUR2, 0x7F );              
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x81 );            // threshold settings for taps and wakeup
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0x21 );            // threshold settings for sleep and wakeup 
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD1_CFG, 0x48 );                // interrupt 1 event config (tap and wakeup) // 48 for tap
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD2_CFG, 0x80 );                // interrupt 2 event config (inactivity and wakeup)
}

void IMUDS3(){
// IMU RESET
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL3_C, 0x01);             
  // IMU ACC INIT FOR LSM6DS3
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL1_XL, 0x4A);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL9_XL, 0x3C);
  //myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL3_C, 0x44);
}


void IMUDSM(){
// IMU RESET
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL3_C, 0x01);             
  // IMU ACC INIT FOR LSM6DSM
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL1_XL, 0x4A);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL9_XL, 0xE4);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL3_C, 0x44);
}


//IMU();
void CHKIMU(){

// INT1 TAP
 if( int1Status > 0 ) {
     delay(TAPTIME);
     // TAP
     if( int1Status == 1 ) {
//        Serial.println("T");
       if (t_toggle == 0){
//          ledcWrite(LEDC_TORCHANNEL, 16);
       t_toggle = 1;
       } else if (t_toggle =! 0){
//          ledcWrite(LEDC_TORCHANNEL, 0);
       t_toggle = 0;
       }
     }
     // DOuBLE TAP
     if( int1Status > 1 )  {
        TOGGLED = 0;
//        Serial.println("DT");
       if (dt_toggle == 0){
//        LIGHTSTATE = 10;
//        ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
        dt_toggle = 1;
        } else if (dt_toggle =! 0){
//          LIGHTSTATE = 1;
//          ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
          dt_toggle = 0;
//          dt_toggle_last = 1;
       }
     }
     int1Status = 0;
 }

   // INT2 WAKEUP
  if(int2Status > 0 ) {
    // read wake up register
    uint8_t sl_STAT = 0;
    myIMU.readRegister(&sl_STAT, LSM6DS3_ACC_GYRO_WAKE_UP_SRC);
    // if sleep event is true
    if(sl_STAT < 8){
      sl_toggle = 1;

    // if not
    } else if (sl_STAT > 8){
      sl_toggle = 0;

    }
    int2Status = 0;  
  }

  // toggle sleep /wakeup only in auto mode ...
  if(sl_toggle == 1) {
    // Lets sleep then

  }
  if (sl_toggle == 0){

  }
}
void readIMU(){
  myIMU.readRegisterInt16(&accelx, LSM6DS3_ACC_GYRO_OUTX_L_XL);
  myIMU.readRegisterInt16(&accely, LSM6DS3_ACC_GYRO_OUTY_L_XL);
  myIMU.readRegisterInt16(&accelz, LSM6DS3_ACC_GYRO_OUTZ_L_XL);
  myIMU.readRegisterInt16(&imutemph, LSM6DS3_ACC_GYRO_OUT_TEMP_H);
  myIMU.readRegisterInt16(&imutempl, LSM6DS3_ACC_GYRO_OUT_TEMP_L);
}

void int1ISR()
{
 int1Status++;
}

void int2ISR()
{
 int2Status++;
}
