# Shire Project

![](/img/Shire.jpg)

Please see the wiki for github help and necessary Arduino libraries [HERE!](https://github.com/innovationproject2017/ShireProject/wiki)

# Current Usage
	
1. See wiki for adding proper libraries to Arduino IDE.
2. Download the Bluefruit app from Adafruit via iOS/Android stores.
3. Program the Arduino using the project file located in this project folder (/shire/shire.ino).
4. Search for the Adafruit Bluefruit module in the app.
5. In the app, select Controller.
6. Press and hold any button to see the sign sweep upwards, let go for it to sweep downwards (default).

Note: The file Shire_Sign_Address_Map.xlsx shows the physical Neopixel LED mapping to look up table in shire_config.h.  This does not need to be adjusted unless the sign is physically altered.

# Changelog

---

## v0.21

## General Changes

* Added flashing sign when bluetooth connection is made and loop() has started.
* Added user ability to control sweep speed
* Added user ability to change thickness
* Added user ability to change sweep direction to any direction instead of just up/down.
* Fixed bug related to sweep direction
* General code cleanup

## Known Bugs

## Upcoming Features

* Add user ability to choose color
* Add motion sensor detection
* Add accelerometer parsing
* Readd DEBUG flag modification for ease of debug usage

---

## v0.20

## General Changes

* Created the change log!

* Implemented Bluefruit LE Shield bluetooth code to work with the Bluefruit iOS/Android app.  This is to prep for the FPGA implementation.

* Refactored code for optimization and readability
    * Added comments and more comments!
	* Removed unused functions and code
	* Pushed LED LUT to program memory.

* Due to limited space of the Arduino Uno, the sign LUT was placed into program memory to allow the NeoPixel object to initialize its LED count as originally intended.  If it is unable to do this, this means there is not enough dynamic memory space to allocate the NeoPixel object and the object will return "0" using strip.numPixels().  With the LUT in program memory, it now initializes and returns NUM_LEDS using strip.numPixels() which is at the time of writing "294".  Since the LUT should not change unless the sign physically changes, the LUT can be updated, and there is plenty of program memory space, this will be optimal until the code fills the program memory.  Otherwise we'd need a bigger Arduino.

## Known Bugs

* Sign does not sweep direction properly sometimes.  Instead of sweeping in all rows 1 by 1, sometimes it will pause, then light a section first, and then start the sweep.  This is due to the "known position" becoming lost using var "pixel_section" in function "neopixel_sweep"
	
## Upcoming Features

* Flash sign when bluetooth connection is made and loop() has started.
* Add user ability to control sweep speed
* Add user ability to change thickness
* Add user ability to change sweep direction to any direction instead of just up/down.
* Add user ability to choose color
* Readd DEBUG flag modification for ease of debug usage

---

# v0.10 and older

## General Changes

* Creation of neopixel/arduino interfacing