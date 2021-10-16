  void CHKBTN()
  {
  

  //    // Button UP 
  int up_reading = digitalRead(BUTTON_UP);
  if (up_reading == LOW && up_lastReading == HIGH) {
    up_onTime = millis();
  }
  if (up_reading == LOW && up_lastReading == LOW) {
    if ((millis() - up_onTime) > holdTime) {
      // do something when held
      //Serial.println("up_hold");
      LIGHTSTATE_UP();                                 
      up_hold = 1;
    }
  }
  if (up_reading == HIGH && up_lastReading == LOW) {
    if (((millis() - up_onTime) > bounceTime) && up_hold != 1) {
      onUPRelease();
    }
    if (up_hold == 1) {
      // do something if released after hold
      up_hold = 0;
    }
  }
  up_lastReading = up_reading;
  if (up_single == 1 && (millis() - up_lastSwitchTime) > doubleTime) {
      // button single click
      //Serial.println("up_single");
      LIGHTSTATE_UP();
    up_single = 0;
  }

  // Button DOWN 
  int down_reading = digitalRead(BUTTON_DOWN);
  if (down_reading == LOW && down_lastReading == HIGH) {
    down_onTime = millis();
  }
  if (down_reading == LOW && down_lastReading == LOW) {
    if ((millis() - down_onTime) > holdTime) {
      // do something on hold
      LIGHTSTATE_DOWN();
      TORCHSTATE_DOWN();
      //Serial.println("down_hold");                                     
      down_hold = 1;
    }
  }
  if (down_reading == HIGH && down_lastReading == LOW) {
    if (((millis() - down_onTime) > bounceTime) && down_hold != 1) {
      onDOWNRelease();
    }
    if (down_hold == 1) {
      // do something if released after hold
      down_hold = 0;
    }
  }
  down_lastReading = down_reading;
  if (down_single == 1 && (millis() - down_lastSwitchTime) > doubleTime) {
    // button single click
    LIGHTSTATE_DOWN();
    TORCHSTATE_DOWN();

    //Serial.println("down_single");
    down_single = 0;
  }
  // Button ENTER
    int enter_reading = digitalRead(BUTTON_ENTER);
    if (enter_reading == LOW && enter_lastReading == HIGH) {
      enter_onTime = millis();
    }
    if (enter_reading == LOW && enter_lastReading == LOW) {
      if ((millis() - enter_onTime) > holdTime) {
        // do something on hold
        //digitalWrite(ENBST, HIGH);
        //Serial.println("enter_hold");
        FLASH();                                
        enter_hold = 1;
      }
    }
    if (enter_reading == HIGH && enter_lastReading == LOW) {
      if (((millis() - enter_onTime) > bounceTime) && enter_hold != 1) {
        onENTERRelease();
      }
      if (enter_hold == 1) {
        // do something if released after hold
        enter_hold = 0;
      }
    }
    enter_lastReading = enter_reading;
    if (enter_single == 1 && (millis() - enter_lastSwitchTime) > doubleTime) {
      // button single click
      // digitalWrite(ENBST, LOW);
      COLOR();
      // Serial.println("enter klick off");

      enter_single = 0;
    }


}


//onENTERRelease button function
void onENTERRelease() {
  if ((millis() - enter_lastSwitchTime) >= doubleTime) {
    enter_single = 1;
    enter_lastSwitchTime = millis();
    return;
  }
  if ((millis() - enter_lastSwitchTime) < doubleTime) {
    TORCH();                  // do something on double click
    enter_single = 0;
    enter_lastSwitchTime = millis();
  }
}

////onDOWNRelease button function
void onDOWNRelease() {
  if ((millis() - down_lastSwitchTime) >= doubleTime) {
    down_single = 1;
    down_lastSwitchTime = millis();
    return;
  }
  if ((millis() - down_lastSwitchTime) < doubleTime) {
  // do something on double click
      if (wifi_toggle == 0){
      STARTWIFI();
      
      }    
    //wm.autoConnect();  
    //TRIGGER_PIN=LOW;
    down_single = 0;
    down_lastSwitchTime = millis();
  }
}

//onUPRelease button function
void onUPRelease() {
  if ((millis() - up_lastSwitchTime) >= doubleTime) {
    up_single = 1;
    up_lastSwitchTime = millis();
    return;
  }
  if ((millis() - up_lastSwitchTime) < doubleTime) {
    BOOST();                  // do something on double click
    up_single = 0;
    up_lastSwitchTime = millis();
  }
}
