/*
 * Copyright (C) 2024 Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     library
 * @{
 *
 * @file
 * @brief       sen15901 functions library
 *
 * @author      Leo Festoc <leo.festoc@univ-grenoble-alpes.fr>
 * @author      Sarah Sabri <Sarah.sabri@univ-grenoble-alpes.fr>
 * @author      Dorian Poissonnet <Dorian.Poissonnet@univ-grenoble-alpes.fr>
 * @author      Thibault Abry <Thibault.abry@univ-grenoble-alpes.fr>
 * @}
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "senml.h"
#include "periph/adc.h"
#include "periph/gpio.h"
#include "flash_utils.h"
#include "phydat.h"
#include "senml/phydat.h"
#include "saul_reg.h"
#include "sen15901.h"
#include "fmt.h"


#define ADC_IN_USE 			ADC_LINE(0)
#define ADC_RES				ADC_RES_12BIT
#define M_PI  (3.14159265358979323846)

/* LoRaMac values */
#define JOIN_NEXT_RETRY_TIME            30 // Next join tentative in 0.5 minute
#define SECONDS_PER_DAY                 24 * 60 * 60


#include "ztimer.h"

#include <stdio.h>



void _sen15901_mesure(int duration)
{
 //to modify
    sen15901_t dev_sen15901;
	sen15901_params_t params = { 
		  .adc = ADC_LINE(0), 
		  .res = ADC_RES_12BIT, 
		  .sensor0_pin = GPIO_PIN(PORT_B, 14), 
		  .sensor0_mode = GPIO_IN, 
		  .sensor1_pin = EXTERNAL_GPIO_PIN, 
		  .sensor1_mode = GPIO_IN_PD, 
		  .sensor1_flank = GPIO_RISING, 
		  .sensor2_pin = BTN1_PIN, 
		  .sensor2_mode = GPIO_IN_PD, 
		  .sensor2_flank = GPIO_RISING
		};

    uint16_t orientation=-1;
	uint16_t ticks_vent=-1;
	uint16_t wind_speed=-1;
	uint16_t ticks_water=-1;
	uint16_t water_level=-1;

	if (sen15901_init(&dev_sen15901, &params) == SEN15901_OK){
		printf("Ready to get data from sen15901....\n");
			
		int res;
			
				
		/**** Wind direction ****/
		res = sen15901_get_wind_direction(&dev_sen15901, &orientation);
		if (res != SEN15901_OK){
			printf("Error %d for wind direction\n", res);
		}else{
			printf("Direction : %d\n", orientation);
			}
				
		/**** Wind speed ****/
		res = sen15901_get_wind_ticks(&dev_sen15901, &ticks_vent);
		if (res != SEN15901_OK){
			printf("Error %d when fetching wind ticks\n", res);
			wind_speed = 0;
		}else{
			wind_speed = ticks_vent*2.4/duration;
			printf("Vitesse du vent : %d km/h\n", (int) (wind_speed));
			}
				
		/**** Rain ****/
		res = sen15901_get_water_ticks(&dev_sen15901, &ticks_water);
		if (res != SEN15901_OK){
			printf("Error %d when fetching water ticks\n", res);
			water_level = 0;
		}else{
			water_level = ticks_water*0.2794;
			printf("Precipitation : %d mm\n", (int) (water_level));
			}
				
        }
        else printf("init sen15901 failed\n");
				
      
}