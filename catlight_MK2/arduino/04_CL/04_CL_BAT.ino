void drawtext(char *text, uint16_t color, uint16_t tsize, uint16_t x, uint16_t y) {
  tft.setCursor(x, y);
  tft.setTextSize (tsize);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}



// POWERBANK FUNCTION
void BOOST(){
  if (bst_toggle == 0){
    digitalWrite(ENBST, HIGH);
    tft.fillRect(0, 0, 10, 80, ST77XX_RED );
    bst_toggle = 1;
  } else if (bst_toggle == 1)
  {
    digitalWrite(ENBST, LOW);
    tft.fillRect(0, 0 , 10, 80, ST77XX_BLACK );
    bst_toggle = 0;
  }
}




// BATDISP
void BATDISP(){     


    uint16_t bv =  analogRead(1);
    float v = 2.43000 * (float)bv  * 0.00025300;
    float mv = 2.43000 * (float)bv  * 0.00025300 * 1000;

    if (mv < 3200) {
    u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_RED);      // apply Adafruit GFX color
    } 

    if (mv > 3200 && mv < 3500) {
      u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_ORANGE);      // apply Adafruit GFX color
    } 

    if (mv > 3500 && mv < 3800) {
      u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_YELLOW);      // apply Adafruit GFX color
    } 

    if (mv > 3800 && mv < 4200) {
      u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_GREEN);      // apply Adafruit GFX color
    } 

    if (mv > 4200) {
      u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_CYAN);      // apply Adafruit GFX color
    }
  if (LIGHTSTATE == 0 && TORCHSTATE == 0 && PIXBRI == 0 && bst_toggle == 0)
  {
    tft.fillRect(10, 0 , 30, 80, ST77XX_BLACK);
    //tft.drawRect(0, 0 , 39, 79, ST77XX_WHITE);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_bubble_tr);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_for_adafruit_gfx.setCursor(30, 70);    
    u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    u8g2_for_adafruit_gfx.setFontDirection(3);            // left to right (this is default)
    u8g2_for_adafruit_gfx.print(v);
  } else {
    // uint16_t bv =  analogRead(1);
    // float v = 2.43000 * (float)bv  * 0.00025300;
    tft.fillRect(10, 0 , 30, 80, ST77XX_BLACK);
    tft.fillRect(10, 75 , 30, 5, ST77XX_RED);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_bubble_tr);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2_for_adafruit_gfx.setCursor(30, 70);    
    u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    u8g2_for_adafruit_gfx.setFontDirection(3);            // left to right (this is default)
    u8g2_for_adafruit_gfx.print(v);
  }
}


 

