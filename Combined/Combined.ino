#include <Adafruit_NeoPixel.h>
#include "data_struct2.h"
#include <math.h>
#define DEBUG (0)                         // Whether or not to use DEBUG statements
#define MAX_X (49)                        // The maximum x coordinate in our array
#define MAX_Y (29)                        // The maximum y coordinate in our array
#define PIN 6                             // Which pin to use as output
#define DEL 12                            // The delay (in ms) between each time step

//Direction Defines
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


uint16_t i = 0;                        //pixel iteration count
int pixel_section;                     //keeps track of beginning of lit strip        
uint8_t width = 15;                    //Thickness of lit strip   
uint8_t x_coord = 0;                   // Current x-coordinate we are looking at
uint8_t y_coord = 0;                   // Current y-coordinate we are looking at
int lightDir = UP;                     //Direction of lights
uint16_t buttonPressed = 0;            //Used to prevent multiple increment/decrements on a button press

// Initialize the LED Strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN);

void sweep(uint16_t n, uint8_t x, uint8_t y, uint8_t width, uint8_t lightDir){
  //Select based on direction input
    switch (lightDir){
        case UP:
            // Loop back when we reach the end of the sequence
            if (pixel_section-width > 30){
                pixel_section = 1;
            }
            //Light strip within range specified by width
            if ((y > pixel_section) && (y< pixel_section+width)) {
                strip.setPixelColor(n, 5*x,9*y,3*(x+y));
            } else {
                strip.setPixelColor(n, 0,0,0);
            }   
            break;
        case DOWN: 
            // Loop back when we reach the end of the sequence
            if (pixel_section+width < 0){ //Min y coordinate
                pixel_section = 30; //Set LEDs to light back at the top (max Y coordinate)
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
                pixel_section = 50;
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
            if (pixel_section-width > 50){ ///ERROR(?): pixels start lit a section in instead of sliding in; I don't know why
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


//////// All of the sweeps were combined in the above function //////////
void sweep_down(uint16_t n, uint8_t x, uint8_t y, uint8_t width){
    // Loop back when we reach the end of the sequence
    if (pixel_section+width < 0){ //Min y coordinate
        pixel_section = 30; //Set LEDs to light back at the top (max Y coordinate)
    }
    
    //Light strip within range specified by width
    if ((y > pixel_section) && (y< pixel_section+width)) {
        strip.setPixelColor(n, 5*x,9*y,3*(x+y));
    } else {
        strip.setPixelColor(n, 0,0,0);
    }    
    return;
}

void sweep_up(uint16_t n, uint8_t x, uint8_t y, uint8_t width){
    // Loop back when we reach the end of the sequence
    if (pixel_section-width > 30){
        pixel_section = 1;
    }
    //Light strip within range specified by width
    if ((y > pixel_section) && (y< pixel_section+width)) {
        strip.setPixelColor(n, 5*x,9*y,3*(x+y));
    } else {
        strip.setPixelColor(n, 0,0,0);
    }   
    return;
}
void sweep_right(uint16_t n, uint8_t x, uint8_t y, uint8_t width){
    // Loop back when we reach the end of the sequence
    if (pixel_section-width > 50){
        pixel_section = 1;
    }
    //Light strip within range specified by width
    if ((x > pixel_section) && (x< pixel_section+width)) {
        strip.setPixelColor(n, 5*x,9*y,3*(x+y));
    } else {
        strip.setPixelColor(n, 0,0,0);
    }   
    return;
}

void sweep_left(uint16_t n, uint8_t x, uint8_t y, uint8_t width){
    // Loop back when we reach the end of the sequence
    if (pixel_section+width < 0){
        pixel_section = 50;
    }
    //Light strip within range specified by width
    if ((x > pixel_section) && (x< pixel_section+width)) {
        strip.setPixelColor(n, 5*x,9*y,3*(x+y));
    } else {
        strip.setPixelColor(n, 0,0,0);
    }   
    return;
}
//////////////////////////////////////////////////////////////////

/////////IDK Wbat this stuff is for ////////////////
// This type of function writes a pixel value given x, y coordinates, and a pixel_sectioner
typedef void (*update_grid)(uint16_t, uint8_t, uint8_t);
// Func is a pointer to the given update_grid function we are using
update_grid func;
///////////////////////////////////////////////////

void setup() {
    // put your setup code here, to run once:
    strip.begin();
    strip.show(); //Turns off all pixels
    if (DEBUG) {
        Serial.begin(9600);      // open the serial port at 9600 bps:
    }

    //Direction Inputs
    //00=up, 01=down, 10=left, 11=righ
    pinMode(2, INPUT);
    pinMode(3, INPUT);

    //Strip thickness
    pinMode(4, INPUT); //thicker
    pinMode(5, INPUT); //thinner
}

void loop() {
  // reset pixel iteration count
  i = 0;
  while(1) {
    //Map pixel to the sign layout
    x_coord = pixel_array[i][0];
    y_coord = pixel_array[i][1];
    
    //Direciton selection
    if ((digitalRead(2) == LOW) && (digitalRead(3) == LOW)){
      lightDir = UP;
    }
    else if ((digitalRead(2) == HIGH) && (digitalRead(3) == LOW)){
      lightDir = DOWN;
    }
    else if ((digitalRead(2) == LOW) && (digitalRead(3) == HIGH)){
      lightDir = LEFT;
    }
    else if ((digitalRead(2) == HIGH) && (digitalRead(3) == HIGH)){
      lightDir = RIGHT;
    }
    sweep(i, x_coord, y_coord, width, lightDir);
		
    //Increment through all pixels and then break to update strip location
    i++;
    if (i == 294){
        break;
    }
  }
  
  strip.show(); //Turns off all pixels

  //Lit Section Thickness
  if (digitalRead(4) == LOW){
    //Prevents multiple increments on each press
    if (!buttonPressed){    
      if (width < 15){
        width++; //increase width
      }
      buttonPressed = 1;
    }
  }
  else if (digitalRead(5) == LOW){
    if (!buttonPressed){
      if (width > 2){
        width--; //decrease width
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
 
  delay(DEL);
}


