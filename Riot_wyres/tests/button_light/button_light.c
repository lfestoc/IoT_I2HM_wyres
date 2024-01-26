#include "xtimer.h"
#include "board.h"
#include "periph/adc.h"
#include "periph/gpio.h"
#include <stdio.h>
#include "flash_utils.h"
#include "phydat.h"
#include "saul_reg.h"

#define ADC_IN_USE 			ADC_LINE(1)
#define ADC_RES				ADC_RES_12BIT


int main(void)
{

	LED_GREEN_ON;
	xtimer_sleep(1);
	LED_RED_OFF;
	xtimer_sleep(1);
	LED_RED_OFF;
	xtimer_sleep(1);
	LED_GREEN_OFF;
	xtimer_sleep(1);

	/* initialize the ADC line */
    if (adc_init(ADC_IN_USE) < 0) {
        printf("\r\nInitialization of ADC_LINE(%u) failed\r\n", ADC_IN_USE);

    	LED_RED_ON;
    	xtimer_sleep(5);
    	LED_RED_OFF;

        return 1;
    }
    else {
        printf("\r\nSuccessfully initialized ADC_LINE(%u)\r\n", ADC_IN_USE);

    	LED_GREEN_ON;
    	xtimer_sleep(2);
    	LED_GREEN_OFF;

    }
	
	int sample = 0;
	/*int64_t luminosity = 0;
	printf("Luminosity : %\n", luminosity);*/

	while(1){

        sample = adc_sample(ADC_IN_USE,ADC_RES);
		/*double part1 = sample<<16;
		double part2 = sample ; */
            //printf("ADC_LINE(%u): raw value: %d\r\nLuminisoty : %f", ADC_IN_USE, sample, luminosity);
			// lecture capteur pression / température
			//printf("Part 1 = %.2f\n Part 2 = %.2f\n", part1, part2);
			printf("%#010x\n", sample);
			
            LED_GREEN_ON;
            LED_GREEN_OFF;
			xtimer_msleep(1000);

		/*while(1){

		float voltage = adc_sample(ADC_IN_USE,ADC_RES);

		float kelvin = voltage * 100.0;

		printf("Port : %f\r\n", kelvin);

		xtimer_msleep(500);
	}*/
	}
    return 0;

}

