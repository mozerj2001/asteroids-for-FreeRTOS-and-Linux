#ifndef SETUP_H
#define SETUP_H

#include "setup.h"
#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "em_gpio.h"
//#include "gpiointerrupt.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t xHandle_refreshLCD;
TaskHandle_t xHandle_updateRNGParams;
TaskHandle_t xHandle_evolveObject;
TaskHandle_t xHandle_detectCollision;
TaskHandle_t xHandle_gamestate;
TaskHandle_t xHandle_delay;

// Declare variable for spaceship speed. This will be incremented
// every time the spaceship goes from the right side of the screen
// to the left.
extern unsigned int speed;

// Declare score counter variable.
extern unsigned int score;

// Create random number for random number generation based on user input.
extern unsigned int random;
extern volatile unsigned int random_freeze;

// Declare three variables storing steps to generate pseudo-random
// numbers.
extern unsigned int forward;
extern unsigned int left;
extern unsigned int right;
extern unsigned int turnover;

// Declare name state types, create global variable for it.
enum gameState {startup, playing, game_over};
extern volatile enum gameState gamePhase;

/* This function updates the variables, based upon which
 * the asteroids are randomly generated */
void updateRNGParams(void* in);


// Declare arrays representing the LCD digits
SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

// Direction types
enum direction {fel, le, elore};
enum turnDirection {balra, jobbra, egyenesen};


// Declare sruct that stores a position on the LCD display
typedef struct positioning positioning;
struct positioning{
	uint8_t digit;		// index of the current digit
	char segment;		// index of the current segment in the current digit
};

// Declare struct that stores all relevant data of the spaceship
// params: current direction ; current turn command ; last position ; current position
typedef struct obj_data obj_data;
struct obj_data{
	enum direction dir;
	volatile enum turnDirection turn;
	positioning positions[2];
};

// Declare three global variables that store all data relevant to the asteroids
extern positioning asteroids[3];

obj_data object;

void initAll();

void refreshLCD(void* in);

void Setdir();

void nextdigit();

void evolveObject(void* in);

void generateAsteroids();

void detectCollision(void* in);

void printGameOver();

void printStart();

/* Simple delay function */
void delay(unsigned int x);

#endif
