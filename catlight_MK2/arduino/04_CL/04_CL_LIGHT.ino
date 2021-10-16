void FLASH(){
  if (avgNTC3() < FLAMAXTEMP){
    ledcWrite(LEDC_FLACHANNEL, 1024);
    delay(50);
    ledcWrite(LEDC_FLACHANNEL, 0);
    delay(100);
    ledcWrite(LEDC_FLACHANNEL, 1024);
    delay(50);
    ledcWrite(LEDC_FLACHANNEL, 0);
    delay(100);
    ledcWrite(LEDC_FLACHANNEL, 1024);
    delay(50);
    ledcWrite(LEDC_FLACHANNEL, 0);
    delay(100);
  }
}
void TORCH(){
  if (avgNTC3() > TORMAXTEMP){
    ledcWrite(LEDC_TORCHANNEL, 0);
  } else if (avgNTC3() < TORMAXTEMP) {
    ledcWrite(LEDC_TORCHANNEL, 500);
  }
}
void READBOOK(){
  ledcWrite(LEDC_TORCHANNEL, 4);
  ledcWrite(LEDC_PWMCHANNEL, 0);
  ledcWrite(LEDC_TFTCHANNEL, 0);
  while(true){
    if(digitalRead(BUTTON_DOWN)==HIGH||digitalRead(BUTTON_UP)==HIGH||digitalRead(BUTTON_ENTER)==HIGH){
      ledcWrite(LEDC_TFTCHANNEL, 200);
      break;
    }
  }
  
};                             // do something on double click

// Brightness routines //
void LIGHTSTATE_UP() {
  if (avgNTC2() > LIGHTMAXTEMP ){
    LIGHTSTATE = 0;
    ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    
  } else {
    while(true){
    delay(10);

    if (LIGHTSTATE >= LIGHTPWMLIMIT - 5 ){
      LIGHTSTATE = LIGHTPWMLIMIT;
      ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);

    
    } else {
      LIGHTSTATE = LIGHTSTATE + 5;
      ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    }
    int kitkat = digitalRead(BUTTON_UP);
      if (kitkat == HIGH) {
        break;
      }
    }
  }
}

void LIGHTSTATE_DOWN() {
  while(true){
  delay(10);

  if (LIGHTSTATE <= 5) {
    LIGHTSTATE = 0;
      ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);

  } else {
    LIGHTSTATE = LIGHTSTATE - 5;
      ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
    }
 int kitkat = digitalRead(BUTTON_DOWN);
   if (kitkat == HIGH) {
     break;
   }
 }
}




void TORCHSTATE_DOWN() {
  while(true){
  delay(10);

  if (TORCHSTATE <= 1) {
    TORCHSTATE = 0;
      ledcWrite(LEDC_TORCHANNEL, TORCHSTATE);

  } else {
    TORCHSTATE = TORCHSTATE - 1;
      ledcWrite(LEDC_TORCHANNEL, TORCHSTATE);
    }
 int kitkat = digitalRead(BUTTON_DOWN);
   if (kitkat == HIGH) {
     break;
   }
 }
}