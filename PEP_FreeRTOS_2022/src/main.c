#include <stdio.h>
#include <retargetserial.h>
#include "em_device.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "setup.h"
#include "FreeRTOS.h"
#include "task.h"


#define  LCD_task_prio ( tskIDLE_PRIORITY + 4 )
#define  gamestate_task_prio ( tskIDLE_PRIORITY + 5 )
#define  detect_task_prio ( tskIDLE_PRIORITY + 3 )
#define  evolve_task_prio ( tskIDLE_PRIORITY + 2 )
#define  RNG_task_prio ( tskIDLE_PRIORITY + 1 )

#define  defaultTASK_STACK 2*configMINIMAL_STACK_SIZE

void gamestate(){
	while(1){
		if(gamePhase != playing){
			// Reset game speed to fix delay function.
			speed = 0;

			if(gamePhase == startup){
				printStart();
			}
			else{
				printGameOver();
			}

			/* Set default spaceship positions */
			object.dir = elore;
			object.turn = egyenesen;
			object.positions[1].digit = 0;
			object.positions[1].segment = 'g';
			score=0;
			forward = 0;
			left = 0;
			right = 0;
			turnover=1;

		}
		else{
			vTaskSuspend(NULL);
		}

	}
}

// import global variable for speed
unsigned int speed = 50000;
// import global variables for the three asteroids
struct positioning asteroids[3];

// pseudo-random number needs to be initialized here as well...
unsigned int random;

// import global game state variable (set to pause on startup)
volatile enum gameState gamePhase = startup;





int main(void)
{
    RETARGET_SerialInit();
    RETARGET_SerialCrLf(1);

    xTaskCreate(gamestate, "", defaultTASK_STACK, NULL, gamestate_task_prio, &xHandle_gamestate);
    xTaskCreate(refreshLCD, "", defaultTASK_STACK, NULL, LCD_task_prio, &xHandle_refreshLCD);
    xTaskCreate(updateRNGParams, "", defaultTASK_STACK, NULL, RNG_task_prio, &xHandle_updateRNGParams);
    xTaskCreate(evolveObject, "", defaultTASK_STACK, NULL, evolve_task_prio, &xHandle_evolveObject);
    xTaskCreate(detectCollision, "", defaultTASK_STACK, NULL, detect_task_prio, &xHandle_detectCollision);



	/* Initialize all peripheries */
	initAll();

	/* Set default positions */
	object.dir = elore;
	object.turn = egyenesen;
	object.positions[1].digit = 0;
	object.positions[1].segment = 'g';

	/* create starting asteroids */
	asteroids[0].digit = 3;
	asteroids[0].segment = 'e';
	asteroids[1].digit = 5;
	asteroids[1].segment = 'g';
	asteroids[2].digit = 2;
	asteroids[2].segment = 'a';

    vTaskStartScheduler();

}
void GPIO_ODD_IRQHandler(void)
{
	// clear IT flag
	GPIO_IntClear(1 << 9);

	// update data
	object.turn = balra;
	left++;
	random_freeze = random;	// save random number
	gamePhase = playing;			// restart game if the game was over
}

void GPIO_EVEN_IRQHandler(void)
{
	// clear IT flag
	GPIO_IntClear(1 << 10);

	// update data
	object.turn = jobbra;
	right++;
	random_freeze = random;	// save random number
	gamePhase = playing;			// restart game if the game was over
}
