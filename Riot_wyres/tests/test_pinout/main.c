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

	phydat_t res;

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
	

	while(1){
		saul_reg_t *dev = saul_reg;

        if (dev == NULL) {
            puts("No SAUL devices present");
            return 1;
        }

        sample = adc_sample(ADC_IN_USE,ADC_RES);

            printf("ADC_LINE(%u): raw value: %.4i, percent: %.2d %% \r\n", ADC_IN_USE, sample, sample*100/4096);
            printf("\n%d\n", sample);
			// lecture capteur pression / température
			for(int i=0;i<2;i++)
			{
				int dim = saul_reg_read(dev, &res);
            printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,
                   saul_class_to_str(dev->driver->type));
            phydat_dump(&res, dim);
            dev = dev->next;
			}
			

            LED_GREEN_ON;
			printf("Button 1 has been released!\r\n");
            LED_GREEN_OFF;


	}
	
    return 0;
}


/*	while(1){

		float voltage = adc_sample(ADC_IN_USE,ADC_RES);

		float kelvin = voltage * 100.0;

		printf("Port : %f\r\n", kelvin);

		xtimer_msleep(500);
	}
	
    return 0;
}
*/