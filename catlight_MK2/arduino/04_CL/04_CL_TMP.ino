    
void TEMPS() {
      uint16_t temp1 = avgIMU();
      uint16_t temp2 = avgNTC2();
      uint16_t temp3 = avgNTC3();

      if (temp1 < 40){
        tcolor1 = ST77XX_GREEN;
      } else if (temp1 > 40 && temp1 < 45) {
        tcolor1 = ST77XX_YELLOW;
      } else if (temp1 > 45 && temp1 < 50) {
        tcolor1 = ST77XX_ORANGE;
      } else if (temp1 > 50) {
        tcolor1 = ST77XX_RED;
      }

      if (temp2 < 40){
        tcolor2 = ST77XX_GREEN;
      } else if (temp2 > 40 && temp2 < 45) {
        tcolor2 = ST77XX_YELLOW;
      } else if (temp2 > 45 && temp2 < 50) {
        tcolor2 = ST77XX_ORANGE;
      } else if (temp2 > 50) {
        tcolor2 = ST77XX_RED;
      }

      if (temp3 < 40){
        tcolor3 = ST77XX_GREEN;
      } else if (temp3 > 40 && temp3 < 45) {
        tcolor3 = ST77XX_YELLOW;
      } else if (temp3 > 45 && temp3 < 50) {
        tcolor3 = ST77XX_ORANGE;
      } else if (temp3 > 50) {
        tcolor3 = ST77XX_RED;
      }
      
      tft.fillRect(40, 0 , 120, 40, ST77XX_BLACK );
      //tft.drawRect(40, 0 , 59, 39, ST77XX_WHITE );
      //tft.drawRect(100, 0 , 59, 39, ST77XX_WHITE );
      
      u8g2_for_adafruit_gfx.setFont(u8g2_font_bubble_tr);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
      u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
      u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
      //u8g2_for_adafruit_gfx.setForegroundColor(tcolor1);      // apply Adafruit GFX color
      //u8g2_for_adafruit_gfx.setCursor(50, 18);                // start writing at this position
      //u8g2_for_adafruit_gfx.print(temp1);
      u8g2_for_adafruit_gfx.setForegroundColor(tcolor2);      // apply Adafruit GFX color
      u8g2_for_adafruit_gfx.setCursor(55, 28);                // start writing at this position
      u8g2_for_adafruit_gfx.print(temp2);
      u8g2_for_adafruit_gfx.setForegroundColor(tcolor3);      // apply Adafruit GFX color
      u8g2_for_adafruit_gfx.setCursor(115, 28);                // start writing at this position
      u8g2_for_adafruit_gfx.print(temp3);
      
      
}

// calculate temp in °C from ntc analog raw data
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((8192.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}


// TOP NTC
uint16_t avgNTC2(void) {
      int val=analogRead(2);                  //  ESP32-S2 Analog NTC input on pin io4
      int itemp=Thermistor(val) - 20;
    return itemp;
}
//

// BOTTOM NTC
uint16_t avgNTC3(void) {
      int  val=analogRead(3);                  //  
      int  itemp=Thermistor(val) - 20;
    return itemp;
}

// Average IMU reading
uint16_t avgIMU(void) {
   int itemp= myIMU.readTempC();
   return itemp;
 }


void AUTOPWM(){ 
 // LIGHT
 if (avgNTC2() > LIGHTCRITTEMP){
   LIGHTSTATE = 0;
    ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
   }

 // always down
 if (avgNTC2() > LIGHTMAXTEMP && LIGHTSTATE > 1){
   LIGHTSTATE = LIGHTSTATE - 1;
    ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
 }


 
if (dt_toggle == 1 && sl_toggle == 1) {
  if(TOGGLED == 0){
    TOGGLED = 1;
    tft.fillRect(40, 40 , 120, 40, ST77XX_BLACK);
    //tft.drawRect(40, 40 , 119, 39, ST77XX_WHITE);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_maniac_tf);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_for_adafruit_gfx.setForegroundColor(0x881F);      // apply Adafruit GFX color
    u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
    u8g2_for_adafruit_gfx.setCursor(60, 70);                // start writing at this position
    u8g2_for_adafruit_gfx.print("AUTO");
  }
  
  // LIGHTS UP
    if (avgNTC2() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
      LIGHTSTATE = LIGHTSTATE + 1;
    ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    } 
  
  } else if (dt_toggle == 0) {
    // LIGHTSTATE = 0 ;
    // ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    if (TOGGLED == 0){
    TOGGLED = 1;
    tft.fillRect(40, 40 , 120, 40, ST77XX_BLACK);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_maniac_tf);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_for_adafruit_gfx.setForegroundColor( 0x1B58 );      // apply Adafruit GFX color
    u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    u8g2_for_adafruit_gfx.setCursor(50, 70);                // start writing at this position
    u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
    u8g2_for_adafruit_gfx.print("MANUAL");
    }
  }
}


 
