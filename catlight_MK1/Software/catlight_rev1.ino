/* Catlight rev 1
 *  by http://github.com/specs32/catlight
 *  Hard and Software is licensend under CC-NC-BY-SA
 *
 *  ESIDE :
 *
 *  ESP32-S2 (@ 240 MHz with 40 MHz Crystal & 4MB Flash)
 *  MAX77751 Charger (INOK - ITOPOFF - STAT)
 *  Navigattion wheel (3 Buttons)
 *  1 TFT - 1.2" SPI Color Display 
 *  1 IMU - Gyro Accel - (LSM6DS3)
 *
 *  LEDSIDE :
 *
 *  10 x SK6812 - 3535  8x Light leds (0-7) & 2x status-leds (8+9) (GND PWR Mosfet switched)
 *  16 Cree J-Series HighPower (@ 5600 Kelvin) (GND PWR PWM switched)
 *  1 NTC
 *  
 */

// TODO   :
// integrade display 
// sleep modes
// charger control
// pixel control 
// button redo 




// INDICATORS
#include <FastLED.h>                                            // please check out https://github.com/FastLED/FastLED for license and such
#define SLED_PIN     GPIO6
#define WLED_PIN     GPIO7
#define NUM_SLEDS    2
#define NUM_WLEDS    8
#define BRIGHTNESS  64
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
struct CRGB sleds[NUM_SLEDS];                                   // Initialize our LED array.
struct CRGB wleds[NUM_WLEDS];                                   // Initialize our LED array.

CRGBPalette16 currentPalette;                                 // Palette definitions
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

// CHECK !!
uint8_t statled = 0;                                           //name the status pixels (statled for charger connected charge status)
uint8_t boostled = 1;                                          // name the boost led (for reverse boost on USB C Port (powerbank function))
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.


// DONT NEED ?

// TIMERS
//#include <SimpleTimer.h>                                      // out of the arduino repo
//SimpleTimer TEMPCHECK;
//SimpleTimer BATCHECK;
//SimpleTimer PWMCHECK;

// IMU
#include "SparkFunLSM6DS3.h"                                  // please check out https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library for license and such
LSM6DS3 myIMU( I2C_MODE, 0x6A );                              //I2C device address 0x6A

// INT 1
const int int1Pin = 16;                                        //Use pin 2 for int.0 on uno
uint8_t int1Status = 0;
uint8_t t_toggle = 0;                                         // tap toggle
uint8_t t_toggle_last = 0;
uint8_t dt_toggle = 0;                                        // dt toggle
uint8_t dt_toggle_last = 0;

// INT 2
const int int2Pin = 15;                                        //Use pin 2 for int.0 on uno
uint8_t int2Status = 0;
uint8_t wu_toggle = 0;

// CHECK !!
// BUTTON PINS
const int BUTTON_UP = 1;                                    // define the up button pin
const int BUTTON_DOWN = 2;                                  // define the down button pin
const int BUTTON_PUSH = 3;

// PIXEL GND PWR (mosfet) PIN
const int PIXELS = 8;                                           // PowerLED  PWM control. (OC1A // PB1 // PIN9)

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN            5

// CONSTRAINS to the leds
uint8_t LIGHTPWMLIMIT = 150;                                  // PWMLIMIT FOR THE FULLY CHARGED LIPO !!!! 
uint16_t LIGHTMAXTEMP = 40;                                   // (when to STEP down !) Depends very strong on the thermal connection between LED and NTC... 40 works for me here atm.
uint8_t LIGHTCRITTEMP = 60;                                   // (when to SHUT down  !) compensate for overshoot ...
uint8_t LIGHTSTATE = 0;                                       // LIGHT @ 1-(PWMLIMIT)

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = LIGHTPWMLIMIT) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}



// LIPO Setup jeah, no ADC ... dumbass !!

//uint16_t Vf_LIGHT = 3000;    
//uint16_t bat_full = 4300;                                     // Lipo charged
//uint16_t bat_empty = 2800;                                    // Lipo discharged 

// CHECK !! buttons active high

// BUTTON UP
boolean up_lastReading = HIGH;                                // button read value
long up_onTime = 0;                                           // button read ontime
long up_lastSwitchTime = 0;                                   // button switchtime value
uint8_t up_hold = 0;                                          // button hold value
uint8_t up_single = 0;                                        // button single press value

// BUTTON DOWN
boolean down_lastReading = HIGH;                              // button read value
long down_onTime = 0;                                         // button read ontime
long down_lastSwitchTime = 0;                                 // button switchtime value
uint8_t down_hold = 0;                                        // button hold value
uint8_t down_single = 0;

// BUTTON SETTINGS
const uint8_t bounceTime = 50;                                // define button debounce
const uint8_t holdTime = 250;                                 // define button hold time
const uint16_t doubleTime = 700;                              // define button doubleclick time

// PLASMA FUNCTION
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

// SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP SETUP 

void setup() {
  
  // grace
  delay(500);
  
  // debug
  Serial.begin(115200);

  // setup pixels
  LEDS.addLeds<LED_TYPE, SLED_PIN, COLOR_ORDER>(sleds, NUM_SLEDS);
  LEDS.addLeds<LED_TYPE, WLED_PIN, COLOR_ORDER>(wleds, NUM_WLEDS);
  FastLED.setBrightness(max_bright);
  currentPalette = OceanColors_p;


// CHECK !!!
// timers 
//TEMPCHECK.setInterval(53, CHECKTEMP);                       // call  every x milliseconds
//BATCHECK.setInterval(3001, BATDISP);               
//PWMCHECK.setInterval(557, SETPWM);               

  // IMU SETUP
  myIMU.begin();                                                        // This is the config for a LSM6DSM ! (check datasheet which registers to pull !)
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL1_XL, 0x61);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_CTRL8_XL, 0x04);
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_CFG1, 0x9E );               
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_TAP_THS_6D, 0x8C );
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT_DUR2, 0x7F );              
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x81 );            // threshold settings for taps and wakeup
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0x0F );            // threshold settings for sleep and wakeup 
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD1_CFG, 0x48 );                // interrupt 1 event config (tap and wakeup) // 48 for tap
  myIMU.writeRegister( LSM6DS3_ACC_GYRO_MD2_CFG, 0x20 );                // interrupt 2 event config (inactivity and wakeup)

  // setting up the interrupt pins
  pinMode(int1Pin, INPUT);                                       
  attachInterrupt(digitalPinToInterrupt(int1Pin), int1ISR, RISING); 
  pinMode(int2Pin, INPUT);                                      
  attachInterrupt(digitalPinToInterrupt(int2Pin), int2ISR, RISING);

  //define BUTTON pins
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_PUSH, INPUT);
  
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);

} // setup end

// LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP LOOP OF INFINITY
void loop() {
  
  // TIMERS
  //TEMPCHECK.run();
  //BATCHECK.run();
  //PWMCHECK.run();
  
  // INT1 TAP
  if( int1Status > 0 ) {
      delay(300);
      // TAP
      if( int1Status == 1 ) {
//        Serial.println("T");
        if (t_toggle == 0){
        t_toggle = 1;
        } else if (t_toggle =! 0){
        t_toggle = 0;
        }
      }
      // DOuBLE TAP
      if( int1Status > 1 )  {
//        Serial.println("DT");
        if (dt_toggle == 0){
        dt_toggle = 1;
        } else if (dt_toggle =! 0){
        dt_toggle = 0;
        dt_toggle_last = 1;
        }
      }
      int1Status = 0;
  }

// CHECK !!! ESP SLEEP / DEEP SLEEP / WAKEUP ROUTINES 

  // INT2 WAKEUP
  
//  if(digitalRead(int2Pin) == LOW) {
//    //Serial.println("SLEEP");
//    
//    while(true){
//      EVERY_N_MILLISECONDS(25) {                                  // FastLED based non-blocking delay to update/display the sequence.
//        plasma();
//        }
//      EVERY_N_MILLISECONDS(500) {
//        uint8_t maxChanges = 56; 
//        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
//        }
//      EVERY_N_SECONDS(50) {                                 // Change the target palette to a random one every 5 seconds.
//        uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
//        targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
//        }
//      FastLED.show();
//    if (digitalRead(int2Pin) == HIGH){
//      break;
//      }
//    }
//    int2Status = 0;
//  }

// CHECK !!!!!   ---  complete BUTTON REDO !!!

  // Button UP 
  int up_reading = digitalRead(BUTTON_UP);
  if (up_reading == LOW && up_lastReading == HIGH) {
    up_onTime = millis();
  }
  if (up_reading == LOW && up_lastReading == LOW) {
    if ((millis() - up_onTime) > holdTime) {
      // do something when held
      LIGHTSTATE_UP();                                 
      up_hold = 1;
    }
  }
  if (up_reading == HIGH && up_lastReading == LOW ) {
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
    down_single = 0;
  }
} // loop end


// CHECK !!! REDO !!!

// void SLEEP(){
//   wu_toggle = 0;

//   // ADD Power saving !!!
//   //    
//   //    
  
//   while(true){
    
//       EVERY_N_MILLISECONDS(25) {                                  // FastLED based non-blocking delay to update/display the sequence.
//       plasma();
//       }
//     EVERY_N_MILLISECONDS(500) {
//       uint8_t maxChanges = 56; 
//       nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
//       }
//     EVERY_N_SECONDS(50) {                                 // Change the target palette to a random one every 5 seconds.
//       uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
//       targetPalette = CRGBPalette16(CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)), CHSV(baseC+random8(32), 192, random8(128,255)), CHSV(baseC+random8(32), 255, random8(128,255)));
//       }
//     FastLED.show();
    
//     int kitkat = digitalRead(BUTTON_UP);
//       if (kitkat == LOW) {
//         break;
//       }
//   }
      
// }




// NTC Sensor dependent LIGHTLEVEL calculation bla
void CHECKTEMP(){ 
  // check for crtitcal temps first !

  // LIGHT
  if (avgNTC() > LIGHTCRITTEMP){
    LIGHTSTATE = 0;
    ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
    }

  // always down
  
  if (avgNTC() > LIGHTMAXTEMP && LIGHTSTATE > 2){
    LIGHTSTATE = LIGHTSTATE - 2;
    ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
  }

  // CLEAR 
  if (dt_toggle_last == 1 && dt_toggle == 0){
      LIGHTSTATE = 0;
      ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
      dt_toggle_last = 0;
  }

  // AUTO 
  if (dt_toggle == 1){
    
    // LIGHTS UP
    if (avgNTC() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
        LIGHTSTATE = LIGHTSTATE + 1;
        ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
      }
    
      if (LIGHTSTATE > 1){
          LIGHTSTATE = 0;
          ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
      } 
      
    }
    // LIGHTS OFF 
    if (t_toggle == 0){
      if (t_toggle_last == 0){
        if (avgNTC() < LIGHTMAXTEMP && LIGHTSTATE < LIGHTPWMLIMIT) {
          LIGHTSTATE = LIGHTPWMLIMIT;
          ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
          t_toggle_last = 1;
        }
      }
    }
  }


//onDOWNRelease button function
void onDOWNRelease() {
  if ((millis() - down_lastSwitchTime) >= doubleTime) {
    down_single = 1;
    down_lastSwitchTime = millis();
    return;
  }
  if ((millis() - down_lastSwitchTime) < doubleTime) {
    //READBOOK();                             // do something on double click
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
    //FLASH();                  // do something on double click
    up_single = 0;
    up_lastSwitchTime = millis();
  }
}

// Brightness routines //
void LIGHTSTATE_UP() {

  if (avgNTC() > LIGHTMAXTEMP ){
    LIGHTSTATE = 10;
    ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
    
  } else {
    while(true){
    if (LIGHTSTATE >= LIGHTPWMLIMIT-5) {
      LIGHTSTATE = LIGHTPWMLIMIT;
      ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
    
    } else {
      LIGHTSTATE = LIGHTSTATE + 5;
      ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
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
  if (LIGHTSTATE <= 5) {
    LIGHTSTATE = 0;
    ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);

  } else {
    LIGHTSTATE = LIGHTSTATE - 5;
    ledcAnalogWrite(LEDC_CHANNEL_0, LIGHTSTATE);
    }
  int kitkat = digitalRead(BUTTON_DOWN);
    if (kitkat == HIGH) {
      break;
    }
  }
}




// Nice plasma on the pixels please !
void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.
  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);
  for (int k=0; k<NUM_WLEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..
    wleds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }
} // plasma()



// // status led brightness routine
// uint8_t APA_bri(int BRI_setting){

//       leds[briled].b = (255 - BRI_setting);
//       leds[briled].g = 0;
//       leds[briled].r = BRI_setting;
//       FastLED.show();
//   }

// calculate temp in °C from ntc analog raw data
double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}


// measure ADC and calculate middle
uint16_t avgNTC(void) {
    int val;
    uint16_t imax=0;
    uint16_t imin=65535;
    uint16_t isum=0;
    uint16_t itemp=0;
    uint8_t ctr;
    for (ctr=10;ctr>0;ctr--) {
        val=analogRead(4);                  //  ESP32-S2 Analog NTC input on pin io4
        itemp=Thermistor(val);
        if (itemp<imin) imin=itemp;
        if (itemp>imax) imax=itemp;
        isum+=itemp;
        delay(4);
    }
    isum-=imin;
    isum-=imax; //subtract the min and max from the sum, now only 8 are left
    itemp=isum/8; //compiler will automatically optimize this to a shift
    return itemp;
}


void int1ISR()
{
  int1Status++;
}

void int2ISR()
{
  int2Status++;
}
// EOF
