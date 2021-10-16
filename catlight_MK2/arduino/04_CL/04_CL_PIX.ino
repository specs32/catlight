uint16_t rgb565( int r, int g, int b)
{
  uint16_t R = r & 0xFF;
  uint16_t G = g & 0xFF;
  uint16_t B = b & 0xFF;

  uint16_t ret  = (R & 0xF8) << 8;  // 5 bits
           ret |= (G & 0xFC) << 3;  // 6 bits
           ret |= (B & 0xF8) >> 3;  // 5 bits
       
  return( ret);
}


void COLOR()         // COLOR
// *********************************************************************
{
    PIXBRI = 100;
    HOLD = 0;
    LIGHTSTATE = 0;
    TORCHSTATE = 0;
    ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    ledcWrite(LEDC_TORCHANNEL, TORCHSTATE);

    tft.fillRect(40, 40 , 120, 40, ST77XX_BLACK);
  IMUDS3();
  // IMUDSM
    while(true)
    {
      readIMU();
      for(uint16_t i=0; i<LED_COUNT ; i++) {
        int16_t r = abs(accelx) * 254 / 8192;
        int16_t g = abs(accely) * 254 / 8192;
        int16_t b = abs(accelz) * 254 / 8192;
        if (r > 254){r=254;}
        if (g > 254){g=254;}
        if (b > 254){b=254;}
        
        tetecolor =   rgb565(r,g,b);
        tft.fillRect(70, 50 , 40, 20, tetecolor);

        int up_reading = digitalRead(BUTTON_UP);
        int down_reading = digitalRead(BUTTON_DOWN);

        if (PIXBRI < 254 && up_reading == LOW){
          PIXBRI = PIXBRI + 1;
        }
        if (PIXBRI > 10 && down_reading == LOW){
          PIXBRI = PIXBRI - 1;
        }
        strip.setBrightness(PIXBRI);
        strip.setPixelColor(i, strip.Color(r,g,b));
        strip.show();
        //delay(10);
      }  
      
      int kitkat = digitalRead(BUTTON_ENTER);
      if (kitkat == LOW)
      {
        tft.fillRect(40, 40 , 120, 40, ST77XX_BLACK);
        tft.drawRect(40, 40 , 119, 39, tetecolor );
        u8g2_for_adafruit_gfx.setFont(u8g2_font_maniac_tf);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
        u8g2_for_adafruit_gfx.setForegroundColor(tetecolor);      // apply Adafruit GFX color
        u8g2_for_adafruit_gfx.setCursor(50, 70);    
        u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
        u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
        u8g2_for_adafruit_gfx.print("COLOR");
        HOLD = 1;
        delay(200);
        while (true){

          int up_reading = digitalRead(BUTTON_UP);
          int down_reading = digitalRead(BUTTON_DOWN);

          if (PIXBRI < 254 && up_reading == LOW){
            PIXBRI = PIXBRI + 1;
          }
          if (PIXBRI > 12 && down_reading == LOW){
            PIXBRI = PIXBRI - 1;
          }

          strip.setBrightness(PIXBRI);
          strip.show();

          kitkat = digitalRead(BUTTON_ENTER);
          if (HOLD == 1 && kitkat == LOW){
                PIXBRI = 0;
                int16_t r = 0;
                int16_t g = 0;
                int16_t b = 0;
                for(uint16_t i=0; i<LED_COUNT ; i++) {
                  strip.setBrightness(PIXBRI);
                  strip.setPixelColor(i, strip.Color(r,g,b));
                  strip.show();
                }  
              tft.fillRect(40, 40 , 120, 40, ST77XX_BLACK);
              u8g2_for_adafruit_gfx.setFont(u8g2_font_maniac_tf);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
              u8g2_for_adafruit_gfx.setForegroundColor(0x1B58);      // apply Adafruit GFX color
              u8g2_for_adafruit_gfx.setCursor(50, 70);    
              u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
              u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
              u8g2_for_adafruit_gfx.print("MANUAL");
              HOLD = 0;

              IMUSTREG();
   break;
      }
    }
  break;

  }
}
}
