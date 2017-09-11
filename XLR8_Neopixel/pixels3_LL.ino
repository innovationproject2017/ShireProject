#include <Adafruit_NeoPixel.h>
#include "data_struct2.h"
#include <math.h>
#define DEBUG (0)                         // Whether or not to use DEBUG statements
#define MAX_X (49)                        // The maximum x coordinate in our array
#define MAX_Y (29)                        // The maximum y coordinate in our array
#define NUM_FUNCS (1)                     // The number of functions we have to loop through
#define PIN 6                             // Which pin to use as output
#define DEL 5                            // The delay (in ms) between each time step
#define SWEEP_LEFT_TIM_MAX (MAX_X)        // The maximum timer value for sweep_left

// This array holds the coordinates of each of the pixels
uint16_t i = 0;
int tim = 49;                              // Current stage in the programming process (function dependent)
uint8_t which_func = 0;
uint8_t x_coord = 0;                              // Current x-coordinate we are looking at
uint8_t y_coord = 0;                              // Current y-coordinate we are looking at

// Initialize the LED Strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN);

void sweep_right(uint16_t n, uint8_t x, uint8_t y){
    // Loop back when we reach the end of the sequence
    if (tim < 0){
        tim = 50;
    }
// Check to see if the x coordinate is large enough to light up
    if (x < tim) {
        strip.setPixelColor(n, 0, 0, 0);
    } else if (x < 2*tim){
//        Serial.print("Turn On Pixel: ");
//        Serial.println(x);
        strip.setPixelColor(n, 5*x,9*y,3*(x+y));
    } else {
//        Serial.print("Turn Off Pixel: ");
//        Serial.println(x);
        strip.setPixelColor(n, 0,0,0);
    }    
    return;
}

// This type of function writes a pixel value given x, y coordinates, and a timer
typedef void (*update_grid)(uint16_t, uint8_t, uint8_t);
// Func is a pointer to the given update_grid function we are using
update_grid func;

// This function chooses the given function we want to use
void choose_func(int x){
    switch(x){
        case 0:
            func = &sweep_right;
            return;
        default:
            func = &sweep_right;
            return;
        }
}

/*
// Gets the x-coordinate of the current pixel
int get_x_coord(int idx) {
    return pixel_array[idx][0];
}
// Gets the y-coordinate of the current pixel
int get_y_coord(int idx) {
    return pixel_array[idx][1];
}
*/

void setup() {
    // put your setup code here, to run once:
    strip.begin();
    strip.show();
    if (DEBUG) {
        Serial.begin(9600);      // open the serial port at 9600 bps:
    }
}

void loop() {
    // chase(strip.Color(0, 0, 255)); // Blue
    // put your main code here, to run repeatedly:
//    delay(DEL);
//    strip.setPixelColor(0, 255, 0, 0);
//    strip.show();
//    delay(DEL);
//    strip.setPixelColor(1, 0, 255, 0);
//    strip.show();
//    delay(DEL);
//    strip.setPixelColor(2, 0, 0, 255);
//    strip.show();
//    delay(DEL);
//    strip.setPixelColor(0, 0, 255, 0);
//    strip.show();
//    delay(DEL);
//    strip.setPixelColor(1, 0, 0, 255);
//    strip.show();
//    delay(DEL);
//    strip.setPixelColor(2, 255, 0, 0);
//    strip.show();
//    delay(DEL);
//      if (DEBUG) {
//    Serial.println(NUM_LIGHTS);
//      }
    
//    //choose_func(which_func);
//    // Update each of the lights from the display
    i = 0;
    while(1) {
        x_coord = pixel_array[i][0];
        y_coord = pixel_array[i][1]; 
        // strip.setPixelColor(i, 60, 60, 60);
        sweep_right(i, x_coord, y_coord);       
        //func(i, x_coord, y_coord);
        i++;
        if (i == 294)
            break;
    }
    strip.show();
    tim--;
    delay(DEL);
}


