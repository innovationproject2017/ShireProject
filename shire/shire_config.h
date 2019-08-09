#ifndef SHIRE_CONFIG_H
#define SHIRE_CONFIG_H

//***BEGIN BLUEFRUIT DEFAULT SETUP FROM EXAMPLE***//

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// ----------------------------------------------------------------------------------------------
// These settings are used in both SW UART, HW UART and SPI mode
// ----------------------------------------------------------------------------------------------
#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLE_READPACKET_TIMEOUT         30   // Timeout in ms waiting to read a response


// ----------------------------------------------------------------------------------------------
// The following macros declare the pins that will be used for 'SW' serial.
// You should use this option if you are connecting the UART Friend to an UNO
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SWUART_RXD_PIN       9    // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       10   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         11   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         -1   // Optional, set to -1 if unused


// ----------------------------------------------------------------------------------------------
// The following macros declare the HW serial port you are using. Uncomment
// this line if you are connecting the BLE to Leonardo/Micro or Flora
// ----------------------------------------------------------------------------------------------
#ifdef Serial1    // this makes it not complain on compilation if there's no Serial1
  #define BLUEFRUIT_HWSERIAL_NAME      Serial1
#endif


// ----------------------------------------------------------------------------------------------
// The following sets the optional Mode pin, its recommended but not required
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_UART_MODE_PIN        12    // Set to -1 if unused


// ----------------------------------------------------------------------------------------------
// The following macros declare the pins to use for HW and SW SPI communication.
// SCK, MISO and MOSI should be connected to the HW SPI pins on the Uno when
// using HW SPI.  This should be used with nRF51822 based Bluefruit LE modules
// that use SPI (Bluefruit LE SPI Friend).
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4

// SOFTWARE SPI SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the pins to use for SW SPI communication.
// This should be used with nRF51822 based Bluefruit LE modules that use SPI
// (Bluefruit LE SPI Friend).
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SPI_SCK              13
#define BLUEFRUIT_SPI_MISO             12
#define BLUEFRUIT_SPI_MOSI             11
//***END BLUEFRUIT DEFAULT SETUP FROM EXAMPLE***//


//NeoPixel Serial IN Pin
#define NEOPIXEL_IN 6

//Sweep Direction Button Pins
#define DIRECTION_MSB 3
#define DIRECTION_LSB 2

//NeoPixel Thickness Button Pins
#define NEOPIXEL_THICKNESS_DECREMENT 0
#define NEOPIXEL_THICKNESS_INCREMENT 1

//Turns DEBUG mode on and off
#define DEBUG 0				//Enable = 1, Disable = 0

//Setup NeoPixel Refresh Interval
#define NEOPIXEL_DELAY 1	//Delay in ms
#define MAX_NP_DELAY 200
#define MIN_NP_DELAY 1

//Sign Dimensions
#define MAX_X 49
#define MAX_Y 29
#define MAX_THICKNESS 14
#define MIN_THICKNESS 0

//State of the neopixel pattern and thickness
typedef enum {
	NP_UP,
	NP_DOWN,
	NP_LEFT,
	NP_RIGHT,
	NP_ALL_WHITE,
	NP_ALL_RED,
	NP_ALL_BLUE,
	NP_ALL_GREEN,
	NP_FLASH,
	NP_PICK,
	NP_THICKNESS_INC,
	NP_THICKNESS_DEC,
	NP_THICKNESS_NONE,
	NP_DELAY_INC,
	NP_DELAY_DEC,
	NP_DELAY_NONE
} neopixel_state;

//Create 2D array LUT (look up table) of the NeoPixel individual LEDs and initialize it into program memory
//See shire spreadsheet for explanation of values
#define NUM_LIGHTS 294
const uint8_t pixel_array[NUM_LIGHTS][2] PROGMEM = {{10, 29},
{11, 29},
{12, 29},
{13, 29},
{14, 29},
{15, 29},
{16, 29},
{17, 29},
{18, 29},
{14, 28},
{14, 27},
{14, 26},
{14, 25},
{14, 24},
{14, 23},
{14, 22},
{14, 21},
{14, 20},
{14, 19},
{14, 18},
{14, 17},
{14, 16},
{14, 15},
{20, 15},
{20, 16},
{20, 17},
{20, 18},
{20, 19},
{20, 20},
{20, 21},
{20, 22},
{20, 23},
{20, 24},
{20, 25},
{20, 26},
{20, 27},
{20, 28},
{20, 29},
{21, 22},
{22, 22},
{23, 22},
{24, 22},
{25, 22},
{26, 22},
{27, 22},
{28, 22},
{29, 29},
{29, 28},
{29, 27},
{29, 26},
{29, 25},
{29, 24},
{29, 23},
{29, 22},
{29, 21},
{29, 20},
{29, 19},
{29, 18},
{29, 17},
{29, 16},
{29, 15},
{30, 15},
{30, 16},
{30, 17},
{30, 18},
{30, 19},
{30, 20},
{30, 21},
{30, 22},
{30, 23},
{30, 24},
{30, 25},
{30, 26},
{30, 27},
{30, 28},
{30, 29},
{31, 29},
{32, 29},
{33, 29},
{34, 29},
{35, 29},
{36, 29},
{37, 29},
{38, 29},
{39, 29},
{39, 22},
{38, 22},
{37, 22},
{36, 22},
{35, 22},
{34, 22},
{33, 22},
{32, 22},
{31, 22},
{31, 15},
{32, 15},
{33, 15},
{34, 15},
{35, 15},
{36, 15},
{37, 15},
{38, 15},
{39, 15},
{8, 12},
{7, 13},
{6, 14},
{5, 14},
{4, 14},
{3, 14},
{2, 14},
{1, 13},
{0, 12},
{0, 11},
{0, 10},
{0, 9},
{0, 8},
{1, 8},
{2, 7},
{3, 7},
{4, 7},
{5, 7},
{6, 7},
{7, 6},
{8, 6},
{8, 5},
{8, 4},
{8, 3},
{8, 2},
{7, 2},
{6, 1},
{5, 1},
{4, 1},
{3, 1},
{2, 1},
{1, 2},
{0, 2},
{10, 0},
{10, 1},
{10, 2},
{10, 3},
{10, 4},
{10, 5},
{10, 6},
{10, 7},
{10, 8},
{10, 9},
{10, 10},
{10, 11},
{10, 12},
{10, 13},
{10, 14},
{11, 7},
{12, 7},
{13, 7},
{14, 7},
{15, 7},
{16, 7},
{17, 7},
{18, 7},
{19, 0},
{19, 1},
{19, 2},
{19, 3},
{19, 4},
{19, 5},
{19, 6},
{19, 7},
{19, 8},
{19, 9},
{19, 10},
{19, 11},
{19, 12},
{19, 13},
{19, 14},
{20, 14},
{21, 14},
{22, 14},
{23, 14},
{24, 14},
{25, 14},
{26, 14},
{27, 14},
{28, 14},
{29, 14},
{24, 13},
{24, 12},
{24, 11},
{24, 10},
{24, 9},
{24, 8},
{24, 7},
{24, 6},
{24, 5},
{24, 4},
{24, 3},
{24, 2},
{24, 1},
{20, 0},
{21, 0},
{22, 0},
{23, 0},
{24, 0},
{25, 0},
{26, 0},
{27, 0},
{28, 0},
{29, 0},
{30, 0},
{30, 1},
{30, 2},
{30, 3},
{30, 4},
{30, 5},
{30, 6},
{30, 7},
{30, 8},
{30, 9},
{30, 10},
{30, 11},
{30, 12},
{30, 13},
{30, 14},
{31, 14},
{32, 14},
{33, 14},
{34, 14},
{35, 14},
{36, 14},
{37, 13},
{38, 12},
{39, 11},
{39, 10},
{39, 9},
{38, 8},
{37, 7},
{36, 6},
{35, 6},
{34, 6},
{33, 6},
{32, 6},
{31, 6},
{31, 5},
{32, 4},
{33, 4},
{33, 3},
{34, 3},
{34, 2},
{35, 2},
{35, 1},
{36, 1},
{37, 0},
{38, 0},
{40, 0},
{40, 1},
{40, 2},
{40, 3},
{40, 4},
{40, 5},
{40, 6},
{40, 7},
{40, 8},
{40, 9},
{40, 10},
{40, 11},
{40, 12},
{40, 13},
{40, 14},
{41, 14},
{42, 14},
{43, 14},
{44, 14},
{45, 14},
{46, 14},
{47, 14},
{48, 14},
{49, 14},
{49, 7},
{48, 7},
{47, 7},
{46, 7},
{45, 7},
{44, 7},
{43, 7},
{42, 7},
{41, 7},
{41, 0},
{42, 0},
{43, 0},
{44, 0},
{45, 0},
{46, 0},
{47, 0},
{48, 0},
{49, 0}};

#endif