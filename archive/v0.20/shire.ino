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

Direction lightDir = UP;		//Direction of lights

int pixel_section = 0;			//keeps track of LED strip position       
uint8_t width = 15;				//Thickness of lit strip   
uint8_t x_coord = 0;			// Current x-coordinate we are looking at
uint8_t y_coord = 0;			// Current y-coordinate we are looking at
boolean buttonPressed = false;	//Used to prevent multiple increment/decrements on a button press
boolean blePressed = false;		//DEBUG VARIABLE, WILL CHANGE ONCE FPGA IS READY

/*
	
	Name: NeoPixel Sweep Direction
	Params: uint16_t, uint8_t, uint8_t, uint8_t, Direction
	Description:  Determines the sweep direction of the NeoPixel LED strips and sets the LEDs matrix for the next frame display

*/
void neopixel_sweep(uint16_t n, uint8_t x, uint8_t y, uint8_t width, Direction lightDir){
  //Select based on direction input
    switch (lightDir){
        case UP:
            // Loop back when we reach the end of the sequence
            if (pixel_section-width > MAX_Y){
                pixel_section = 1;
            }
            //Light strip within range specified by width
            if ((y > pixel_section) && (y < pixel_section+width)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }   
            break;
        case DOWN: 
            // Loop back when we reach the end of the sequence
            if (pixel_section+width < 0){ //Min y coordinate
                pixel_section = MAX_Y; //Set LEDs to light back at the top (max Y coordinate)
            }
            //Light strip within range specified by width
            if ((y > pixel_section) && (y< pixel_section+width)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }   
            break;
        case LEFT: 
            // Loop back when we reach the end of the sequence
            if (pixel_section+width < 0){
                pixel_section = MAX_X;
            }
            //Light strip within range specified by width
            if ((x > pixel_section) && (x< pixel_section+width)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }   
            break;
        case RIGHT:
            // Loop back when we reach the end of the sequence
            if (pixel_section-width > MAX_X){
                pixel_section = 1;
            }
            //Light strip within range specified by width
            if ((x > pixel_section) && (x< pixel_section+width)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }   
            break;
    }       
    return;
}

/*
	
	Name: NeoPixel LED Transmit
	Params: boolean
	Description:  Determines the direction, thickness, and direction starting point in which the NeoPixel LEDs will light up and sends the direction to the NeoPixel LED strip

*/
void neopixel_tx(boolean pressed){
	
	//If button is pressed and held from Bluefruit app, direction of lights should go up, otherwise go down
	if (pressed){
		lightDir = UP;
	}
	else {
		lightDir = DOWN;
	}

	//Direction selection using physical buttons on Arduino Uno (if installed)
	/*
	if ((digitalRead(DIRECTION_LSB) == LOW) && (digitalRead(DIRECTION_MSB) == LOW)){
	  lightDir = UP;
	}
	else if ((digitalRead(DIRECTION_LSB) == HIGH) && (digitalRead(DIRECTION_MSB) == LOW)){
	  lightDir = DOWN;
	}
	else if ((digitalRead(DIRECTION_LSB) == LOW) && (digitalRead(DIRECTION_MSB) == HIGH)){
	  lightDir = LEFT;
	}
	else if ((digitalRead(DIRECTION_LSB) == HIGH) && (digitalRead(DIRECTION_MSB) == HIGH)){
	  lightDir = RIGHT;
	}
	*/
	
	//TODO: Add code for all directions using inputted transmitted bluetooth data from FPGA
	
	
	//Set all individual NeoPixel LEDs with appropriate color...
	for (uint16_t i = 0; i < NUM_LIGHTS; i++){
		//Grab LED location based on LED matrix [See Excel Spreadsheet for coordinate sytem and header file for table lookup]
		//Since the table is loaded into program memory to save dynamic memory space, pgm_read_byte must be used
		x_coord = pgm_read_byte(&(pixel_array[i][0]));
		y_coord = pgm_read_byte(&(pixel_array[i][1]));

		neopixel_sweep(i, x_coord, y_coord, width, lightDir);
	}
  
  //Then show it!
  strip.show(); //Display current NeoPixel frame

  //Lit Section Thickness
  if (digitalRead(NEOPIXEL_THICKNESS_INCREMENT) == LOW){
    //Prevents multiple increments on each press
    if (!buttonPressed){    
      if (width < 15){
        width++;
      }
      buttonPressed = 1;
    }
  }
  else if (digitalRead(NEOPIXEL_THICKNESS_DECREMENT) == LOW){
    if (!buttonPressed){
      if (width > 2){
        width--;
      }
      buttonPressed = 1;
    }
  }
  else{
    buttonPressed = 0; 
  }

  //Adjusting lit strip section starting point (sweep direction)
	if(lightDir == DOWN || lightDir == LEFT){
    pixel_section--;
	}
	else if (lightDir == UP || lightDir == RIGHT){
    pixel_section++;
	}
  
  //Change in header file to change speed.
  //TODO: Add user adjustable delay post programming
  delay(NEOPIXEL_DELAY);
}

void setup(void)
{
	
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
	strip.clear();  //Turn off all LEDs and clear frame matrix

	//Direction Inputs pin setup
	//00=up, 01=down, 10=left, 11=right
	pinMode(DIRECTION_LSB, INPUT);
	pinMode(DIRECTION_MSB, INPUT);

	//Strip thickness pin setup
	pinMode(NEOPIXEL_THICKNESS_INCREMENT, INPUT);
	pinMode(NEOPIXEL_THICKNESS_DECREMENT, INPUT);

}

void loop(void)
{
  //Check for a packet.  If no packet (len = 0) then run the current direction sweep of LEDs
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) {
	  neopixel_tx(blePressed); //Default button release on first pass
	  return;
  }

  //Otherwise analyze data received from bluetooth module
  //THIS WILL BE UPDATED ONCE FPGA SIDE IS IMPLEMENTED.  THIS ONLY WORKS WITH BLUEFRUIT APP CONTROLLER MODE_LED_BEHAVIOUR
  //EX: BUTTON PRESSED AND HELD = SWEEP UP
  //BUTTON RELEASED = SWEEP DOWN
  //USEFUL AS DEBUG TOOL TO TEST BLUETOOTH RX
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
	blePressed = !blePressed;
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }
  }

}
