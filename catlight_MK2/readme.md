# CATLight (Creativity Art Technology)

This is MARK 2 of the new and shiny ESP32-S2 driven flat flashlight (aka flatlight)

The last flatlight revision was 20.

So this would be the flatlight REV 21 aka CATLight MK2


## This Revision works ! 

... with no errors as far as I can see and test (I own no OSZI ;))

There is one thing though... the Antenna matching Network works better if you put a cap in series and the coil in the first place to ground. (swap C24 with L3)

This revision works with WLED beta 2 (0.13.0-b2) there are filesystem issues, but the default install seems to work and you can save the LED settings and power off / on again and it maintains theses settings even there is no "official" support for the ESP32-S2 yet.
MAYBE I will fork and branch and modify and write a usermod for wled in the future. Maybe...
For now there is rudimentary arduino firmware in the arduino folder.
There is a platformio_override.ini in the base Folder I used to compile WLED with in code OSS with platformio under arch linux.

DO NOT put too much capacitance on the LED side, it will mess with the soft latch ! 1µF per duris LED is sufficient and works.
If you put a lot of high value CAPs on the LED side you need to lower the value of R5. (1.2KΩ with 25 µF on the LED side. Flash and array)

wifi standby time with a genuine LIPO Battery is 30+ hours

USB-C fast charging works up to 2.5 Ampere !! (almost full in 60 minutes ;))

![screen1](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-15-09.png)
![screen2](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-15-27.png)
![screen3](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-27-51.png)
![screen4](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-09-01%2018-01-33.png)

