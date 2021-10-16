
/* Catlight rev 2
 *  by http://github.com/specs32/catlight
 *  Hard and Software is licensend under CC-NC-BY-SA
 *
 *  ESIDE :
 *
 *  ESP32-S2 (@ 240 MHz with 40 MHz Crystal & 4MB Flash)
 *  MAX77751 Charger (INOK - ITOPOFF - STAT - ENBST)
 *  Navigation  (3 Buttons)
 *  1 TFT - 1.2" SPI Color Display 
 *  1 IMU - Gyro Accel - (LSM6DS3)
 *
 *  LEDSIDE :
 *
 *  10 x SK6812 - 3535  8x Light leds (0-7) & 2x status-leds (8+9) (GND PWR Mosfet switched)
 *  16 Cree J-Series HighPower (@ 5600 Kelvin) (GND PWR PWM switched)
 *  1 NTC
 *  
 *  
 */




// USB







// WIFI
 

/**
 * OnDemandNonBlocking.ino
 * example of running the webportal or configportal manually and non blocking
 * trigger pin will start a webportal for 120 seconds then turn it off.
 * startAP = true will start both the configportal AP and webportal
 */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
bool TRIGGER_PIN = HIGH;
WiFiManager wm;
WiFiManagerParameter custom_field; // global param ( for non blocking w params )


unsigned int  timeout   = 600; // seconds to run for
unsigned int  startTime = millis();
bool portalRunning      = false;
bool startAP            = false; // start AP and webserver if true, else start only webserver



// PIXEL
#include <Adafruit_NeoPixel.h>

#define LED_PIN    5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


uint16_t PIXBRI = 50;                                         // Pixel brightness


// CHARGER PINS
#define INOK 35
#define ENBST 33
#define CHGSTAT 34


// IMU
#include "SparkFunLSM6DS3.h"                                  // please check out https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library for license and such
LSM6DS3 myIMU( I2C_MODE, 0x6A );                              //I2C device address 0x6A

int16_t accelx;
int16_t accely;
int16_t accelz;
int16_t imutemp;
int16_t imutemph;
int16_t imutempl;

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
uint8_t sl_toggle = 1;

//// CHECK !!
//// BUTTON PINS

const int BUTTON_UP = 39;                                    // define the up button pin
const int BUTTON_ENTER = 38;
const int BUTTON_DOWN = 37;                                  // define the down button pin


#define LEDC_PWMCHANNEL     0
#define LEDC_TFTCHANNEL     1
#define LEDC_TORCHANNEL     2
#define LEDC_FLACHANNEL     3

#define LEDC_TORTIMER   10
#define LEDC_TORFREQ     1000000

#define LEDC_FLATIMER   10
#define LEDC_FLAFREQ     1000000


#define LEDC_PWMTIMER   10
#define LEDC_PWMFREQ     1000000

#define LEDC_TFTTIMER   10
#define LEDC_TFTFREQ     1000000

#define TORLED_PIN            6
#define FLALED_PIN            7
#define PWMLED_PIN            8
#define TFTPWR_PIN            9
#define PIXPWR_PIN            4


// CONSTRAINS to the leds
uint16_t TFTPWM = 505;
uint16_t LIGHTPWMLIMIT = 220;                                  // PWMLIMIT FOR THE FULLY CHARGED LIPO !!!! 
uint16_t LIGHTMAXTEMP = 50;                                   // (when to STEP down !) Depends very strong on the thermal connection between LED and NTC... 
uint16_t LIGHTCRITTEMP = 60;                                   // (when to SHUT down  !) compensate for overshoot ...
uint16_t LIGHTSTATE = 0;                                       // LIGHT @ 1-(PWMLIMIT)
uint16_t TORCHPWMLIMIT = 250;
uint16_t TORCHSTATE = 0;                                       // LIGHT @ 1-(PWMLIMIT)
uint16_t Vf_LIGHT = 3000;    
uint16_t FLAMAXTEMP = 50;
uint16_t TORMAXTEMP = 50;


uint16_t bat_full = 4300;                                     // Lipo charged
uint16_t bat_empty = 2850;                                    // Lipo discharged 

// BUTTON SETTINGS
const uint8_t bounceTime = 50;                                // define button debounce
const uint16_t holdTime = 250;                                 // define button hold time
const uint16_t doubleTime = 700;                              // define button doubleclick time
const int TAPTIME = 400;                                // TAP / DT DETECTION TIME



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


// BUTTON ENTER
boolean enter_lastReading = HIGH;                              // button read value
long enter_onTime = 0;                                         // button read ontime
long enter_lastSwitchTime = 0;                                 // button switchtime value
uint8_t enter_hold = 0;                                        // button hold value
uint8_t enter_single = 0;

                          // define button doubleclick time

// PLASMA FUNCTION
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0



// SPI-DISPLAY
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#define TFT_CS        10
#define TFT_RST       11 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        12 
#define TFT_MOSI   14 // Data out
#define TFT_SCLK  13 // Clock out
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Fonts
#include <U8g2_for_Adafruit_GFX.h>
U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

#include <SimpleTimer.h>                                      // out of the arduino repo
SimpleTimer TEMPSCHECK;
SimpleTimer BATCHECK;
SimpleTimer AUTOPWMCHECK;

uint16_t tcolor1 = ST77XX_GREEN;
uint16_t tcolor2 = ST77XX_GREEN;
uint16_t tcolor3 = ST77XX_GREEN;
uint16_t HOLD = 0;
uint16_t TOGGLED = 0;
int dt_toggle_old = 0;
uint16_t tetecolor = 0;
int bst_toggle = 0;
int wifi_toggle = 0;




void setup() {

  // serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(1000);
  Serial.println("\n Starting");

//  HWSerial.begin(115200);
//  HWSerial.setDebugOutput(true);
//
//  USB.onEvent(usbEventCallback);
//  MSC_Update.onEvent(usbEventCallback);
//  MSC_Update.begin();
//  USBSerial.begin();
//  USB.begin();
//


// WIFI 
 
 

  
  // timers
  TEMPSCHECK.setInterval(1053);                       // call  every x milliseconds
  BATCHECK.setInterval(3001);               
  AUTOPWMCHECK.setInterval(157);     
   
  // i2c
  Wire.begin(18,17);

  u8g2_for_adafruit_gfx.begin(tft);                 // connect u8g2 procedures to Adafruit GFX

  // IMU SETUP
  IMUSTREG();
  
  // setting up the interrupt pins
  pinMode(int1Pin, INPUT);                                       
  attachInterrupt(digitalPinToInterrupt(int1Pin), int1ISR, RISING); 
  pinMode(int2Pin, INPUT);                                      
  attachInterrupt(digitalPinToInterrupt(int2Pin), int2ISR, RISING);

  //define BUTTON pins
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);

  //define CHARGER pins
  pinMode(ENBST, OUTPUT);           // boost  function
  digitalWrite(ENBST, LOW);         // off by default
  pinMode(INOK, INPUT_PULLUP);      // input source detection high = good | high = boost | off = no VBUS IO
  pinMode(CHGSTAT, INPUT_PULLUP);   // charger status 500ms pulse = charging | off=done

  // define Pins for NEOPIXEL PWR MOSFET and setup pixels
  pinMode(PIXPWR_PIN, OUTPUT);
  digitalWrite(PIXPWR_PIN, HIGH);     // on by default
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(PIXBRI); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Setup  Torch  // Setup  Flash
  ledcSetup(LEDC_TORCHANNEL, LEDC_TORFREQ, LEDC_TORTIMER);
  ledcAttachPin(TORLED_PIN, LEDC_TORCHANNEL);  
  ledcWrite(LEDC_TORCHANNEL, 0);

  ledcSetup(LEDC_FLACHANNEL, LEDC_FLAFREQ, LEDC_FLATIMER);
  ledcAttachPin(FLALED_PIN, LEDC_FLACHANNEL);  
  ledcWrite(LEDC_FLACHANNEL, 0);

  // Setup  Main LEDs
  ledcSetup(LEDC_PWMCHANNEL, LEDC_PWMFREQ, LEDC_PWMTIMER);
  ledcAttachPin(PWMLED_PIN, LEDC_PWMCHANNEL);
  ledcWrite(LEDC_PWMCHANNEL, LIGHTSTATE);
  

  // Setup and Turn on TFT 
  ledcSetup(LEDC_TFTCHANNEL, LEDC_TFTFREQ, LEDC_TFTTIMER);  // ledc for Dimming .. we have a mosfet there
  ledcAttachPin(TFTPWR_PIN, LEDC_TFTCHANNEL);
  ledcWrite(LEDC_TFTCHANNEL, TFTPWM);
  
  //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display
  
  tft.initR(INITR_GREENTAB);
  tft.setRotation(ST7735_MADCTL_BGR);
  tft.setColRowStart(26, 1);

  tft.invertDisplay(1);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);                             // clear the graphcis buffer  

  u8g2_for_adafruit_gfx.setFont(u8g2_font_bubble_tr);  // NICE !!! select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
  u8g2_for_adafruit_gfx.setForegroundColor(ST77XX_BLACK);      // apply Adafruit GFX color

  // Set ADC Gain and Res
  analogSetPinAttenuation(1, ADC_11db); // ADC_BATV (BAT divider)
  analogSetPinAttenuation(2, ADC_11db); // NTC_PWMLED (top between the LEDS)
  analogSetPinAttenuation(3, ADC_11db); // NTC_BAT / XP_LED (bottom NTC)

}


/* MAIN LOOP */

void loop() {


  wm.process();

  
  CHKBTN();
  CHKIMU();


  if(TEMPSCHECK.isReady()){
    TEMPS();
    TEMPSCHECK.reset();
    }
  
  if(BATCHECK.isReady()){
    BATDISP();
    BATCHECK.reset();
    }
  
 
  if(AUTOPWMCHECK.isReady()){
    AUTOPWM();
    AUTOPWMCHECK.reset();
    }

 


} 
