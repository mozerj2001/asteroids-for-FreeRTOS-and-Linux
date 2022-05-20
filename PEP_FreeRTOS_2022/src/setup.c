#include <em_cmu.h>
#include <stdbool.h>
#include <sys/_stdint.h>
#include "setup.h"
#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "em_gpio.h"
//#include "gpiointerrupt.h"
#include "FreeRTOSConfig.h"

// Define score variable.
unsigned int score = 0;

// Global variables assisting random number generation.
unsigned int random = 0;
volatile unsigned int random_freeze = 0;

// Define three variables storing steps to generate pseudo-random
// numbers.
unsigned int forward = 0;
unsigned int left = 0;
unsigned int right = 0;
unsigned int turnover = 1;


// Variable to decide if start or restart should be printed.
int isStart = 1;

/* This function updates the variables used for more pseudorandom number generation. */
void updateRNGParams(void* in){
	while(1){
	switch(object.turn){
	case egyenesen:
		forward++;
		break;
	case balra:
		left++;
		break;
	case jobbra:
		right++;
		break;
	}
	vTaskResume(xHandle_refreshLCD);
	}
}

/*
 * This function draws the spaceship onto the LCD screen. Before drawing,
 * it will delete the previous visualisations setup. WARNING: This deletes
 * asteroids as well, so those need to be redrawn every cycle.
 */

void refreshLCD(void* in){
	while(1){
	// clear display
	for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
	      lowerCharSegments[p].raw = 0;
	}
	// update spaceship location
	switch(object.positions[1].segment){
	case 'a':
		lowerCharSegments[object.positions[1].digit].a  = 1;
		break;
	case 'd':
		lowerCharSegments[object.positions[1].digit].d  = 1;
		break;
	case 'e':
		lowerCharSegments[object.positions[1].digit].e  = 1;
		break;
	case 'f':
		lowerCharSegments[object.positions[1].digit].f  = 1;
		break;
	case 'g':
		lowerCharSegments[object.positions[1].digit].g  = 1;
		lowerCharSegments[object.positions[1].digit].m  = 1;
		break;
	}
	// draw asteroids
	switch(asteroids[0].segment){
	case 'a':
		lowerCharSegments[asteroids[0].digit].a  = 1;
		break;
	case 'd':
		lowerCharSegments[asteroids[0].digit].d  = 1;
		break;
	case 'e':
		lowerCharSegments[asteroids[0].digit].e  = 1;
		break;
	case 'f':
		lowerCharSegments[asteroids[0].digit].f  = 1;
		break;
	case 'g':
		lowerCharSegments[asteroids[0].digit].g  = 1;
		lowerCharSegments[asteroids[0].digit].m  = 1;
		break;
	}

	switch(asteroids[1].segment){
	case 'a':
		lowerCharSegments[asteroids[1].digit].a  = 1;
		break;
	case 'd':
		lowerCharSegments[asteroids[1].digit].d  = 1;
		break;
	case 'e':
		lowerCharSegments[asteroids[1].digit].e  = 1;
		break;
	case 'f':
		lowerCharSegments[asteroids[1].digit].f  = 1;
		break;
	case 'g':
		lowerCharSegments[asteroids[1].digit].g  = 1;
		lowerCharSegments[asteroids[1].digit].m  = 1;
		break;
	}

	switch(asteroids[2].segment){
	case 'a':
		lowerCharSegments[asteroids[2].digit].a  = 1;
		break;
	case 'd':
		lowerCharSegments[asteroids[2].digit].d  = 1;
		break;
	case 'e':
		lowerCharSegments[asteroids[2].digit].e  = 1;
		break;
	case 'f':
		lowerCharSegments[asteroids[2].digit].f  = 1;
		break;
	case 'g':
		lowerCharSegments[asteroids[2].digit].g  = 1;
		lowerCharSegments[asteroids[2].digit].m  = 1;
		break;
	}


	SegmentLCD_LowerSegments(lowerCharSegments);
	SegmentLCD_Number(score);

	vTaskDelay(500);

	vTaskResume(xHandle_detectCollision);
	vTaskResume(xHandle_evolveObject);
	vTaskResume(xHandle_updateRNGParams);

	vTaskSuspend(NULL);

	}
}

/***************************************************************************//**
 * @brief  Gpio callback
 * @param  pin - pin which triggered interrupt
 ******************************************************************************/



void initAll(){

  /* Chip errata */
  CHIP_Init();

  /* Enable GPIO in CMU */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Initialize LCD display */
  SegmentLCD_Init(false);

  /* Initialize GPIO interrupt dispatcher */
  //GPIOINT_Init();

  /* Configure PB9 and PB10 as input */
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
  GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0);

  /* Register callbacks before setting up and enabling pin interrupt. */

  /* Set falling edge interrupt for both ports */
  GPIO_ExtIntConfig(gpioPortB, 9,9, false, true, true);
  GPIO_ExtIntConfig(gpioPortB, 10,10, false, true, true);

  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_SetPriority(GPIO_ODD_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);
  NVIC_SetPriority(GPIO_EVEN_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY);

  NVIC_SetPriorityGrouping(0);
}


//sets the direction(fel, le, elore) marker of the current state based on the earlier position of the object
//so we can decide if the object goes up or down
void Setdir(){
	//forward option added for better understanding. It is not completely necessary
	if(object.positions[1].segment=='d' || object.positions[1].segment=='g' || object.positions[1].segment=='a'){
		object.dir = elore;
		forward++;
	}

	else if(object.positions[1].segment=='f'){
		if(object.positions[0].segment=='a'|| object.positions[0].segment=='d'){
			object.dir = le;
		}
		else if(object.positions[0].segment =='g'){
			object.dir=fel;
		}
		else if(object.positions[0].segment =='e'){
			object.dir=object.dir;
		}
	}
	else if (object.positions[1].segment =='e'){
		if(object.positions[0].segment=='a'|| object.positions[0].segment=='d'){
			object.dir = fel;
		}
		else if(object.positions[0].segment =='g'){
			object.dir=le;
		}
		else if(object.positions[0].segment =='f'){
			object.dir=object.dir;
		}
	}
}
//step digit
void nextdigit(){
	unsigned int i = 0;
			if(object.positions[1].digit<SEGMENT_LCD_NUM_OF_LOWER_CHARS-1){
				object.positions[1].digit=object.positions[1].digit + 1;
				turnover++;
			}
			else{
				object.positions[1].digit=0;
				// increase speed if max speed isn't reached yet
				if (speed != 14) speed=speed+(100000-speed)/10;
				// course passed --> generate new asteroids
				generateAsteroids();
				// asteroids generated --> reset asteroid generating numbers

				score++;
				for(i = 0; i < 8; i++){
					printf("%c", '+');
				}
			}
}

void evolveObject(void* in){
	while(1){
    //step position
	object.positions[0].digit=object.positions[1].digit;
	object.positions[0].segment=object.positions[1].segment;
	//options from horizontal segments
	if((object.positions[0].segment=='a'||object.positions[0].segment=='g'||object.positions[0].segment=='d') && object.turn==egyenesen){
		nextdigit();
	}
	else if(object.positions[0].segment=='g'&& object.turn==jobbra){
		object.positions[1].segment='e';
		nextdigit();
	}
	else if((object.positions[0].segment=='a'||object.positions[0].segment=='d')&& object.turn==jobbra){
		object.positions[1].segment='f';
		nextdigit();
	}
	else if(object.positions[0].segment=='g'&& object.turn==balra){
			object.positions[1].segment='f';
			nextdigit();
	}
	else if((object.positions[0].segment=='a'||object.positions[0].segment=='d')&& object.turn==balra){
			object.positions[1].segment='e';
			nextdigit();
	}
	//options from vertical segments

	//from f
	else if ((object.positions[0].segment=='f') && (object.dir==fel||le) && (object.turn==egyenesen)){
			object.positions[1].segment='e';
			turnover++;
	}
	else if ((object.positions[0].segment=='f') && (object.dir==fel) && (object.turn==balra)){
				object.positions[1].segment='e';
				turnover++;
	}
	else if ((object.positions[0].segment=='f') && (object.dir==fel) && (object.turn==jobbra)){
				object.positions[1].segment='a';
	}
	else if ((object.positions[0].segment=='f') && (object.dir==le) && (object.turn==balra)){
				object.positions[1].segment='g';
	}
	else if ((object.positions[0].segment=='f') && (object.dir==le) && (object.turn==jobbra)){
				object.positions[1].segment='e';
				turnover++;
	}
	//from e
	else if ((object.positions[0].segment=='e') && (object.dir==fel||le) && (object.turn==egyenesen)){
			object.positions[1].segment='f';
			turnover++;
	}
	else if ((object.positions[0].segment=='e') && (object.dir==fel) && (object.turn==balra)){
				object.positions[1].segment='f';
				turnover++;
	}
	else if ((object.positions[0].segment=='e') && (object.dir==fel) && (object.turn==jobbra)){
				object.positions[1].segment='g';
	}
	else if ((object.positions[0].segment=='e') && (object.dir==le) && (object.turn==balra)){
				object.positions[1].segment='d';
	}
	else if ((object.positions[0].segment=='e') && (object.dir==le) && (object.turn==jobbra)){
				object.positions[1].segment='f';
				turnover++;
	}
	Setdir();
	object.turn=egyenesen;
	// Emit current positions on the USRT
	printf("%d%c%d%c%d%c%d%c", object.positions[0].digit, object.positions[0].segment,
								asteroids[0].digit, asteroids[0].segment,
								asteroids[1].digit, asteroids[1].segment,
								asteroids[2].digit, asteroids[2].segment);
	vTaskSuspend(NULL);
	}
}

// This function creates the three asteroids every time the spaceship passes
// between the right side of the screen and the left side. It generates six
// random numbers. A digit and a segment for each asteroid.

void generateAsteroids(){
	char output[6];						// store output
	unsigned int outputInt[6];			// store output as integer from 0 - 6
	unsigned int outputLength = 6;		// length of viable output of poll
	unsigned int ii = 0;
	// read random values from the periphery registers, store them in char* output
	//debugRNG = mbedtls_trng_poll(&randomNumberContext, &output, 6, &outputLength);

	// Use pseudo-random values to determine the position of asteroids
	// The pseudo-random number random is further modified with data from the
	// game, so the relative position of asteroids is still random.
	output[0] = random_freeze + forward + score;
	output[1] = random_freeze - forward - score ;
	output[2] = random_freeze + left - score;
	output[3] = random_freeze - left + score;
	output[4] = random_freeze + right - score;
	output[5] = random_freeze - right + score;

	// even index will be used to determine the digit of the asteroid (7 options)
	for(; ii < outputLength/2; ii++){ outputInt[ii*2] = ((unsigned int) output[ii*2]) % 7; }
	// odd index will be used to determine the segment of the asteroid (5 options)
	for(; ii < outputLength/2; ii++){ outputInt[ii*2+1] = ((unsigned int) output[ii*2+1]) % 5; }


	// set locations for asteroids
	// (asteroids on digit 6 can't be avoided, so any asteroid that would start
	// there is moved to digit 2, 3 and 4)
	switch(outputInt[0]){
	case 0:
		asteroids[0].digit = 3;
		break;
	case 1:
		asteroids[0].digit = 1;
		break;
	case 2:
		asteroids[0].digit = 2;
		break;
	case 3:
		asteroids[0].digit = 3;
		break;
	case 4:
		asteroids[0].digit = 4;
		break;
	case 5:
		asteroids[0].digit = 5;
		break;
	case 6:
		asteroids[0].digit = 6;
		break;
	}
	switch(outputInt[2]){
	case 0:
		asteroids[1].digit = 4;
		break;
	case 1:
		asteroids[1].digit = 1;
		break;
	case 2:
		asteroids[1].digit = 2;
		break;
	case 3:
		asteroids[1].digit = 3;
		break;
	case 4:
		asteroids[1].digit = 4;
		break;
	case 5:
		asteroids[1].digit = 5;
		break;
	case 6:
		asteroids[1].digit = 6;
		break;
	}
	switch(outputInt[4]){
	case 0:
		asteroids[2].digit = 5;
		break;
	case 1:
		asteroids[2].digit = 1;
		break;
	case 2:
		asteroids[2].digit = 2;
		break;
	case 3:
		asteroids[2].digit = 3;
		break;
	case 4:
		asteroids[2].digit = 4;
		break;
	case 5:
		asteroids[2].digit = 5;
		break;
	case 6:
		asteroids[2].digit = 6;
		break;
	}

	switch(outputInt[1]){
	case 0:
		asteroids[0].segment = 'a';
		break;
	case 1:
		asteroids[0].segment = 'f';
		break;
	case 2:
		asteroids[0].segment = 'g';
		break;
	case 3:
		asteroids[0].segment = 'd';
		break;
	case 4:
		asteroids[0].segment = 'e';
		break;

	}

	switch(outputInt[3]){
	case 0:
		asteroids[1].segment = 'a';
		break;
	case 1:
		asteroids[1].segment = 'f';
		break;
	case 2:
		asteroids[1].segment = 'g';
		break;
	case 3:
		asteroids[1].segment = 'd';
		break;
	case 4:
		asteroids[1].segment = 'e';
		break;
	}

	switch(outputInt[5]){
	case 0:
		asteroids[2].segment = 'a';
		break;
	case 1:
		asteroids[2].segment = 'f';
		break;
	case 2:
		asteroids[2].segment = 'g';
		break;
	case 3:
		asteroids[2].segment = 'd';
		break;
	case 4:
		asteroids[2].segment = 'e';
		break;
	}
}

/*
 * These functions print start game and restart game to the screen.
 * */

void printGameOver(){
	// Write message.
	unsigned int i = 0;
	for(i = 0; i < 8; i++){
		printf("%c", '_');
	}

	SegmentLCD_Write("RESTART");
	SegmentLCD_Symbol(LCD_SYMBOL_DP2, 1);
	SegmentLCD_Symbol(LCD_SYMBOL_DP3, 1);
	SegmentLCD_Symbol(LCD_SYMBOL_DP4, 1);
	SegmentLCD_Symbol(LCD_SYMBOL_DP5, 1);
	SegmentLCD_Symbol(LCD_SYMBOL_DP6, 1);
	delay(3);
	SegmentLCD_Write("");
	SegmentLCD_Symbol(LCD_SYMBOL_DP2, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP3, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP4, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP5, 0);
	SegmentLCD_Symbol(LCD_SYMBOL_DP6, 0);
	delay(3);
}

void printStart(){
	unsigned int i = 0;
	// Write message.
	SegmentLCD_Write("START");
	for(i = 0; i < 8; i++){
		printf("%c", '-');
	}
}

void detectCollision(void* in){
	while(1){
		if(object.positions[1].digit == asteroids[0].digit){
			if(object.positions[1].segment == asteroids[0].segment){
				gamePhase = game_over;
			}
		}
		else if(object.positions[1].digit == asteroids[1].digit){
			if(object.positions[1].segment == asteroids[1].segment){
				gamePhase = game_over;
			}
		}
		else if(object.positions[1].digit == asteroids[2].digit){
			if(object.positions[1].segment == asteroids[2].segment){
				gamePhase = game_over;
			}
		}
		if(gamePhase == game_over){
			vTaskResume(xHandle_gamestate);
		}
		vTaskSuspend(NULL);
	}
}

/* Simple delay function */
void delay(unsigned int x) {
	for(int d=0;d<(x*(100000-speed));d++){random++;}
}
































