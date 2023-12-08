#include "xtimer.h"
#include "board.h"
#include "periph/adc.h"
#include "periph/gpio.h"


#define ADC_IN_USE 			ADC_LINE(0)
#define ADC_RES				ADC_RES_12BIT


int main(void)
{
	printf("\r\nRead button 1 state\r\n");

	uint8_t btn_state = 0;

    printf("\r\nTest light sensor\r\n");

	LED_GREEN_ON;
	xtimer_sleep(1);
	LED_RED_OFF;
	xtimer_sleep(1);
	LED_RED_OFF;
	xtimer_sleep(1);
	LED_GREEN_OFF;
	xtimer_sleep(1);

	LIGHT_SENSOR_SUPPLY_ON;
	
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

		while(gpio_read(BTN1_PIN) == btn_state);

        sample = adc_sample(ADC_IN_USE,ADC_RES);

		btn_state = gpio_read(BTN1_PIN);

		if(btn_state && sample > 0){
			printf("Button 1 has been pressed!\r\n");
            printf("ADC_LINE(%u): raw value: %.4i, percent: %.2d %% \r\n", ADC_IN_USE, sample, sample*100/4096);
            printf("\n%d\n", sample);
            LED_GREEN_ON;
		}else{
			printf("Button 1 has been released!\r\n");
            LED_GREEN_OFF;
		}

        
		
		/*if (sample < 0) {
            printf("\r\nADC_LINE(%u): selected resolution not applicable\r\n",ADC_IN_USE);
        }else {
            printf("ADC_LINE(%u): raw value: %.4i, percent: %.2d %% \r\n", ADC_IN_USE, sample, sample*100/4096);
            if(sample > 4096/2) {
            	LED_GREEN_ON;
            	xtimer_msleep(500);
            	LED_GREEN_OFF;
            } else {
            	LED_RED_ON;
            	xtimer_msleep(500);
            	LED_RED_OFF;
            }
        }
		
    	xtimer_msleep(500);*/
	}
	
    return 0;
}
