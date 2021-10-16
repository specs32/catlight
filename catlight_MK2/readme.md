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


#### WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! WARNING !!! 

DO NOT put too much capacitance on the LED side, it will mess with the soft latch ! 1µF per duris LED is sufficient and works.
If you put a lot of high value CAPs on the LED side you need to lower the value of R5. (1.2KΩ with 25 µF on the LED side. Flash and array)

and

the duris LEDs are mosfet driven and that is their ONE and ONLY current limiter, a duty cycle of 30 % on a full lipo makes a nice bright light.
if you pull up the mosfet with 100 % duty cycle it make a smolered PCB with LEDs falling off... probably a good idea to keep that in mind when fumbling with the firmware...


there is a footprint for additional flash on with a solder bridge CS0/CS1 parallel to the internal flash. if you have an ESP32-S2 without flash you can put a flash chip here (AT256Q i think) please refer to the flash compatibility list for the ESP32-S2 on the espressiv website.

wifi standby time with a genuine LIPO Battery is 60+ hours

USB-C fast charging works up to 2.5 Ampere !! (almost full in 60 minutes ;))
the MAX 77751 supports pass through and power delivery, so you can charge your phne on the light in an emerrgency !

Please keep in mind that this project is my private experiment and CC-BY-NC-SA !
If you blow up your toaster .. not my fault.

the overall development cost was 600 €, the cost per CATLight (PCB + parts + 3dprint) is around 50 € assembly time is 6-8 hours if you do it 100% by hand.

![screen1](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-15-09.png)
![screen2](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-15-27.png)
![screen3](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-08-22%2008-27-51.png)
![screen4](https://github.com/specs32/catlight/blob/master/catlight_MK2/Screenshot%202021-09-01%2018-01-33.png)

