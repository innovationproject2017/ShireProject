/*

Name: Shire Sign
Description: Controls the NeoPixel LED strips for the Shire sign.  Includes sweeping of lights in 4 cardinal directions
	Signal path: FPGA/Accelerometer > BT Module TX > BT Module RX > Arduino UNO > NeoPixel LED Strip

*/

//Include C libraries
#include <string.h>
#include <math.h>

//Include Arduino Libraries
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

//Include header files related to this project
#include "shire_config.h"  //Handles pin assignments and Bluefruit setup values

//----------------------------DO NOT MODIFY : FROM BLUEFRUIT SETUP EXAMPLE------------------
//Construct SPI connection to Bluefruit module
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer (to link to packetparser.cpp)
extern uint8_t packetbuffer[];

//-------------------------------------------------------------------------------------------

//GLOBAL VARIABLES

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, NEOPIXEL_IN);	//Initialize NeoPixel object

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

uint8_t neopixel_pos_x = 0;		//Keeps track of LED strip position in x direction for sweeping
uint8_t neopixel_pos_y = 0;		//Keeps track of LED strip position in y direction for sweeping
neopixel_state neopixel_current = NP_DOWN;
neopixel_state neopixel_previous = NP_DOWN;

neopixel_state neopixel_thickness = NP_THICKNESS_NONE;
uint8_t neopixel_thickness_val = 0;		//Thickness of lit strip

neopixel_state neopixel_delay = NP_DELAY_NONE;
uint16_t neopixel_delay_val = 1;	//Default delay value in ms

/*
	
	Name: NeoPixel Pattern
	Params: uint16_t, uint8_t, uint8_t
	Description:  Determines the pattern of the NeoPixel LED strips and sets the LEDs matrix for the next frame display

*/
void neopixel_pattern(uint16_t n, uint8_t x, uint8_t y){
	
	//Select based on pattern selection by button, bluefruit controller, color picker, etc
    switch (neopixel_current){
        case NP_UP:
            // Loop back to the thickness offset when we reach the end of the sequence
            if (neopixel_pos_y - neopixel_thickness_val >= MAX_Y){
                neopixel_pos_y = neopixel_thickness_val;
            }
            //Light strip within range specified by neopixel_thickness_val
            if ((y >= neopixel_pos_y) && (y <= neopixel_pos_y + neopixel_thickness_val)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }
			
			//This accounts for the wrap around
			if ((y >= neopixel_pos_y - MAX_Y) && (y <= neopixel_pos_y - MAX_Y + neopixel_thickness_val)) {
				strip.setPixelColor(n, 5*x,9*y,3*(x+y));
			}
			
            break;
        case NP_DOWN: 
            // Loop back when we reach the end of the sequence
            if (neopixel_pos_y > MAX_Y + neopixel_thickness_val){
                neopixel_pos_y = MAX_Y;
            }
            //Light strip within range specified by neopixel_thickness_val
            if ((y >= neopixel_pos_y) && (y <= neopixel_pos_y + neopixel_thickness_val)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }
			
			//This accounts for the wrap around
			if ((y >= neopixel_pos_y - MAX_Y) && (y <= neopixel_pos_y - MAX_Y + neopixel_thickness_val)) {
				strip.setPixelColor(n, 5*x,9*y,3*(x+y));
			}
            break;
        case NP_LEFT: 
            // Loop back when we reach the end of the sequence
            if (neopixel_pos_x > MAX_X + neopixel_thickness_val){
                neopixel_pos_x = MAX_X;
            }
            //Light strip within range specified by neopixel_thickness_val
            if ((x >= neopixel_pos_x) && (x <= neopixel_pos_x + neopixel_thickness_val)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }
			
			//This accounts for the wrap around
			if ((x >= neopixel_pos_x - MAX_X) && (x <= neopixel_pos_x - MAX_X + neopixel_thickness_val)) {
				strip.setPixelColor(n, 5*x,9*y,3*(x+y));
			}
            break;
        case NP_RIGHT:
            // Loop back to the thickness offset when we reach the end of the sequence
            if (neopixel_pos_x - neopixel_thickness_val >= MAX_X){
                neopixel_pos_x = neopixel_thickness_val;
            }
            //Light strip within range specified by neopixel_thickness_val
            if ((x >= neopixel_pos_x) && (x <= neopixel_pos_x + neopixel_thickness_val)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }
			
			//This accounts for the wrap around
			if ((x >= neopixel_pos_x - MAX_X) && (x <= neopixel_pos_x - MAX_X + neopixel_thickness_val)) {
				strip.setPixelColor(n, 5*x,9*y,3*(x+y));
			}
            break;
		case NP_ALL_WHITE:
			strip.setPixelColor(n, 100, 100, 100);
			break;
		case NP_ALL_RED:
			strip.setPixelColor(n, 100, 0, 0);
			break;
		case NP_ALL_BLUE:
			strip.setPixelColor(n, 0, 100, 0);
			break;
		case NP_ALL_GREEN:
			strip.setPixelColor(n, 0, 0, 100);
			break;
		case NP_PICK:
			strip.setPixelColor(n, red, green, blue);
			break;
		default:
			break;
    }

    return;
}

/*
	
	Name: NeoPixel Frame Update
	Params: None
	Description:  Updates the direction, thickness, and direction starting point in which the NeoPixel LEDs will light up and sends the direction to the NeoPixel LED strip

*/
void neopixel_frame_update(){

	//Set Thickness before setting LEDs
	//Verify that button has been pressed and has not already been pressed and held to avoid rapid increase or decrease
	if (neopixel_thickness == NP_THICKNESS_INC && (neopixel_thickness_val < MAX_THICKNESS)){
		neopixel_thickness_val++;
		neopixel_thickness = NP_THICKNESS_NONE;
	}
	else if (neopixel_thickness == NP_THICKNESS_DEC && (neopixel_thickness_val > MIN_THICKNESS)){
		neopixel_thickness_val--;
		neopixel_thickness = NP_THICKNESS_NONE;
	}

	//Set all individual NeoPixel LEDs with appropriate color...
	for (uint16_t i = 0; i < NUM_LIGHTS; i++){
		//Grab LED location based on LED matrix [See Excel Spreadsheet for coordinate sytem and header file for table lookup]
		//Since the table is loaded into program memory to save dynamic memory space, pgm_read_byte must be used
		neopixel_pattern(i, pgm_read_byte(&(pixel_array[i][0])), pgm_read_byte(&(pixel_array[i][1])));
	}

	//Adjust LED position in x and y directions for next frame depending on state
	switch (neopixel_current){
		case NP_UP:
			neopixel_pos_y++;
			break;
		case NP_DOWN:
			neopixel_pos_y--;
			break;
		case NP_LEFT:
			neopixel_pos_x--;
			break;
		case NP_RIGHT:
			neopixel_pos_x++;
			break;
		default:
			break;
	}

	//Set current state as the previous state to keep history required for future patterns
	neopixel_previous = neopixel_current;
	
	//Then show it by displaying current NeoPixel frame
	strip.show();
  
	//Update frame delay
	//Verify that button has been pressed and has not already been pressed and held to avoid rapid increase or decrease
	if (neopixel_delay == NP_DELAY_INC && (neopixel_delay_val < MAX_NP_DELAY)){
		neopixel_delay_val += 50;
		neopixel_delay = NP_DELAY_NONE;
	}
	else if (neopixel_delay == NP_DELAY_DEC && (neopixel_delay_val > MIN_NP_DELAY)){
		neopixel_delay_val -= 50;
		neopixel_delay = NP_DELAY_NONE;
	}
	
	delay(neopixel_delay_val);
}

void setup(void) {
	
  //----------------------------DO NOT MODIFY : FROM BLUEFRUIT SETUP EXAMPLE------------------
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
  
  //-------------------------------------------------------------------------------
  
  //-----------------------------BEGIN USER SETUP----------------------------------

	strip.begin();	//Fully initialize the pin and pixel count of object
	delay(100);
	strip.clear();  //Turn off all LEDs and clear frame matrix
	delay(100);
	
	//Change colors of sign and start sweep to right to show connectivity is ready
	neopixel_current = NP_ALL_RED;
	neopixel_frame_update();
	delay(500);
	neopixel_current = NP_ALL_BLUE;
	neopixel_frame_update();
	delay(500);
	neopixel_current = NP_ALL_GREEN;
	neopixel_frame_update();
	delay(500);
	neopixel_current = NP_ALL_WHITE;
	neopixel_frame_update();
	delay(500);
	neopixel_current = NP_RIGHT;
	
	//Direction Inputs pin setup
	//00=up, 01=down, 10=left, 11=right
	//pinMode(DIRECTION_LSB, INPUT);
	//pinMode(DIRECTION_MSB, INPUT);

	//Strip thickness pin setup
	//pinMode(NEOPIXEL_THICKNESS_INCREMENT, INPUT);
	//pinMode(NEOPIXEL_THICKNESS_DECREMENT, INPUT);

}

void loop(void) {
	
	//Check for a packet.  If no packet (len = 0) then run the current direction sweep of LEDs
	uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
	if (len == 0) {
	  neopixel_frame_update();
	  return;
	}

	//Otherwise analyze data received from bluetooth module and process based on option chosen (Controller, color picker, etc...)
	
	//Controller
	if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean buttonPressed = packetbuffer[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
		if (buttonPressed) {
			switch (buttnum){
				case 5:
					neopixel_current = NP_UP;
					break;
				case 6:
					neopixel_current = NP_DOWN;
					break;
				case 7:
					neopixel_current = NP_LEFT;
					break;
				case 8:
					neopixel_current = NP_RIGHT;
					break;
				case 1:
					neopixel_thickness = NP_THICKNESS_INC;
					break;
				case 3:
					neopixel_thickness = NP_THICKNESS_DEC;
					break;
				case 2:
					neopixel_delay = NP_DELAY_INC;
					break;
				case 4:
					neopixel_delay = NP_DELAY_DEC;
					break;
				default:
					break;
			}
			Serial.println(" pressed");
			buttonPressed = 0; //Prevents the above states from changing multiple times in one press.
		}
		else {
			Serial.println(" released");
		}
	}
	
	//Color Picker
	if (packetbuffer[1] == 'C') {
		neopixel_current = NP_PICK;
		red = packetbuffer[2];
		green = packetbuffer[3];
		blue = packetbuffer[4];
		Serial.print ("RGB #");
		if (red < 0x10) Serial.print("0");
		Serial.print(red, HEX);
		if (green < 0x10) Serial.print("0");
		Serial.print(green, HEX);
		if (blue < 0x10) Serial.print("0");
		Serial.println(blue, HEX);
	}
}
