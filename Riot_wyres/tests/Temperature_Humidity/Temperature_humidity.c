#include "xtimer.h"
#include "board.h"
#include "periph/adc.h"
#include "periph/gpio.h"


#define ADC_IN_USE 			ADC_LINE(0)
#define ADC_th_sensor			ADC_LINE(1)
#define ADC_RES				ADC_RES_12BIT


int main(void)
{
	printf("\r\nRead button 1 state\r\n");

	//uint8_t btn_state = 0;

	
	/* initialize the ADC line */
    if (adc_init(ADC_th_sensor) < 0) {
        printf("\r\nInitialization of ADC_th_sensor(%u) failed\r\n", ADC_th_sensor);

    	LED_RED_ON;
    	xtimer_sleep(5);
    	LED_RED_OFF;

        return 1;
    }
    else {
        printf("\r\nSuccessfully initialized ADC_th_sensor(%u)\r\n", ADC_th_sensor);

    	LED_GREEN_ON;
    	xtimer_sleep(5);
    	LED_GREEN_OFF;

    }
	
	uint32_t sample = 0;
	//uint8_t data[4];

   /* data[0] = (donnee & 0xFF);           // Masque pour extraire les 8 bits de poids faible.
    data[1] = ((donnee >> 8) & 0xFF);    // Décalage de 8 bits vers la droite pour extraire la prochaine partie.
    data[2] = ((donnee >> 16) & 0xFF);   // Décalage de 16 bits vers la droite pour extraire la prochaine partie.
    data[3] = ((donnee >> 24) & 0xFF);   // Décalage de 24 bits vers la droite pour extraire les 8 bits de poids fort.

    // Affichage des résultats.
    printf("data[0] = 0x%02X\n", data[0]);
    printf("data[1] = 0x%02X\n", data[1]);
    printf("data[2] = 0x%02X\n", data[2]);
    printf("data[3] = 0x%02X\n", data[3]);	*/

	while(1){

        sample = adc_sample(ADC_th_sensor,ADC_RES);

		printf("Val : %010lX\n", sample );
		


	}
	
    return 0;
}
