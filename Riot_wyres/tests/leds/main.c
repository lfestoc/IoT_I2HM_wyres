/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       LED test
 *
 * @author      Didier DONSEZ
 *
 * @}
 */
#include <stdio.h>


#include "xtimer.h"

#include "board.h"

int main(void)
{
	printf("\r\nToogle on board LEDs\r\n");

	LED_GREEN_ON;
	LED_RED_ON;
	double luminosity = 2.3;
	int test = 21;

	xtimer_sleep(5);

	while(1){
		LED_GREEN_TOGGLE;
		xtimer_sleep(1);
		LED_GREEN_TOGGLE;
		xtimer_sleep(1);
		LED_RED_TOGGLE;
		xtimer_sleep(1);
		LED_RED_TOGGLE;
		xtimer_sleep(1);
		
		
		printf("Luminosity : %.1f\nTEST : %d\n", luminosity, test);

		LED_GREEN_TOGGLE;
		xtimer_sleep(1);
		LED_RED_TOGGLE;
		xtimer_sleep(1);
		LED_GREEN_TOGGLE;
		xtimer_sleep(1);
		LED_RED_TOGGLE;
		xtimer_sleep(1);

	}
	
    return 0;
}
