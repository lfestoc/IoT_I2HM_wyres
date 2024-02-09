/*
 * Copyright (C) 2024 Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     apps
 * @{
 *
 * @file
 * @brief       I2HM climate station sent with semtech-Loramac 
 *
 * @author      Leo Festoc <leo.festoc@univ-grenoble-alpes.fr>
 * @author      Sarah Sabri <Sarah.sabri@univ-grenoble-alpes.fr>
 * @author      Dorian Poissonnet <Dorian.Poissonnet@univ-grenoble-alpes.fr>
 * @author      Thibault Abry <Thibault.abry@univ-grenoble-alpes.fr>
 * @author      Didier Donsez <didier.donsez@univ-grenoble-alpes.fr>
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


#define ADC_MISO 			ADC_LINE(0)
#define ADC_LIGHT_SENSOR 	ADC_LINE(1)
#define ADC_RES				ADC_RES_12BIT
#define M_PI  (3.14159265358979323846)

/* LoRaMac values */
#define JOIN_NEXT_RETRY_TIME            30 // Next join tentative in 0.5 minute
#define SECONDS_PER_DAY                 24 * 60 * 60

/* Use a fast datarate, e.g. BW125/SF7 in EU868 */
#define DR_INIT                         LORAMAC_DR_5
#include "ztimer.h"

#include "cayenne_lpp.h"
#include <stdio.h>
//#include "net/loramac.h"     /* core loramac definitions */
#include "semtech_loramac.h" /* package API */
#include "loramac_utils.h"
#ifdef MODULE_SEMTECH_LORAMAC_RX
#include "thread.h"
#include "msg.h"
#endif

  extern semtech_loramac_t loramac;  /* The loramac stack device descriptor */
      /* define the required keys for OTAA, e.g over-the-air activation (the
         null arrays need to be updated with valid LoRa values) */
   //Wyres Groupe 4
 /*    static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0xCA, 0xFE, 0xBA, 0xBE, \
                                                         0xC6, 0xB0, 0xDA, 0xF7 };
     static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0xCA, 0xFE, 0xBA, 0xBE, \
                                                         0x00, 0x00, 0x00, 0x00 };
     static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xD6, 0x90, 0x93, 0x71, \
                                                          0x8B, 0xAA, 0x94, 0x73, \
                                                        0x93, 0x88, 0xFA, 0x5B, \
                                                      0x58, 0x66, 0x78, 0xB2 };
*/  
     //Wyres Groupe 5
    static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0xCA, 0xFE, 0xBA, 0xBE, \
                                                         0x62, 0x58, 0x3C, 0xCC };
                                                    
     static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x00, 0x00, 0x00, 0x00, \
                                                         0x00, 0x00, 0x00, 0x00 };
     static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xa1, 0x71, 0xda, 0xe2, \
                                                          0x6e, 0xa4, 0x0a, 0x6f, \
                                                        0xc7, 0xc1, 0xe3, 0xae, \
                                                        0x21, 0xcf, 0xe2, 0xde };
                                                                       


static cayenne_lpp_t lpp;

static void _print_buffer(const uint8_t *buffer, size_t len, const char *msg)
{
    printf("%s: ", msg);
    for (uint8_t i = 0; i < len; i++)
    {
        printf("%02X", buffer[i]);
    }
}

/*
 * For emulating a track
 */
static void cassiniOval(double time, double a, double b, double *x, double *y)
{
    // Exemple de paramétrisation (à ajuster en fonction de tes besoins)
    double t = time; // Utilise le temps comme paramètre
    *x = a * cos(t);
    *y = b * sin(t);
}

int initialization_adc(void)
{
    // initialize the ADC line 
    if ((adc_init(ADC_LIGHT_SENSOR) < 0) && (adc_init(ADC_MISO) < 0)) {
        printf("\r\nInitialization of ADC_LINE(%u) and ADC_LINE(%u) failed\r\n", ADC_LIGHT_SENSOR, ADC_MISO);
        return -1;
    }
    if (adc_init(ADC_MISO) < 0) {
        printf("\r\nInitialization of ADC_LINE(%u) failed\r\n", ADC_MISO);
        return -2;
    }
    else {
        printf("\r\nSuccessfully initialized ADC_LINE(%u)\r\n", ADC_LIGHT_SENSOR);
        return 1;

    }

    LIGHT_SENSOR_SUPPLY_ON;
}

void initialization_join_cayenne(void) //mettre int pour return 1 ou 0
{
    
    /* 1. auto-initialize the LoRaMAC MAC layer ( nothing to do) */
    /* 2. set the keys identifying the device */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    puts("LORAMAC CAYENNE join procedure ");
      /* 3. join the network */
     /* start the OTAA join procedure (and retries in required) */
    /*uint8_t joinRes = */ loramac_utils_join_retry_loop(&loramac, DR_INIT, JOIN_NEXT_RETRY_TIME, SECONDS_PER_DAY);
    puts("Join procedure succeeded");
}

void display_luminosity(int sample)
{
    sample = adc_sample(ADC_LIGHT_SENSOR,ADC_RES);

    if( sample > 0){

        // Luminosity
        printf("ADC_LINE(%u): raw value: %.4i, percent: %.2d %% \r\n", ADC_LIGHT_SENSOR, sample, sample*100/4096);
        printf("\n%d\n", sample);

    }
}

    typedef struct {
    uint16_t wind_speed;
    uint16_t orientation;
    uint16_t water_level;
} sen15901_values;


int initialization_sen15901(sen15901_t dev_sen15901) 
        {
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

            if (sen15901_init(&dev_sen15901, &params) == SEN15901_OK){
			    printf("Ready to get data from sen15901....\n");
                return 1;
            }
            else 
            {
                printf("init sen15901 failed\n");
                return -1;
            }
		
        }

sen15901_values display_sen15901(sen15901_t dev_sen15901, int duration)
{
    int res;
    sen15901_values vals;
    vals.orientation = -1;
    vals.water_level=-1;
    vals.wind_speed=-1;
    uint16_t ticks_vent=-1;
    uint16_t ticks_water = -1;
    
    /**** Wind direction ****/
    res = sen15901_get_wind_direction(&dev_sen15901, &vals.orientation);
    if (res != SEN15901_OK){    //pas besoin condition vérifiée dans initialization_sen15901
        printf("Error %d for wind direction\n", res);
    }else{
        printf("Direction : %d\n", vals.orientation);
    }
    
    /**** Wind speed ****/
    res = sen15901_get_wind_ticks(&dev_sen15901, &ticks_vent);
    if (res != SEN15901_OK){
        printf("Error %d when fetching wind ticks\n", res);
        vals.wind_speed = 0;
    }else{
        vals.wind_speed = ticks_vent*2.4/duration;
        printf("Vitesse du vent : %d km/h\n", (int) (vals.wind_speed));
    }
    
    /**** Rain ****/
    res = sen15901_get_water_ticks(&dev_sen15901, &ticks_water);
    if (res != SEN15901_OK){
        printf("Error %d when fetching water ticks\n", res);
        vals.water_level = 0;
    }else{
        vals.water_level = ticks_water*0.2794;
        printf("Precipitation : %d mm\n", (int) (vals.water_level));
    }
    return vals;
}

int main(void)
{      


    //Constants and variables declarations

    int duration = 60; // time in seconds between two data fetch
    double init_latitude = 45.5;
    double init_longitude = 5.5;
    double init_altitude = 10000;  // meter
    double init_humidity = 50;
    //double init_luninosity = 500;
    double init_battery_voltage = 3.6; // mV
    int sample = 0;

    phydat_t res;


/*-----------------------------------------------------------------*/    

    puts("I2HM LORAMAC CAYENNE SENSORS START ");
    
    //Sensors initializations

    initialization_adc();

    initialization_join_cayenne();
    

/*-----------------------------------------------------------------*/   
    
    while (true)
    {   
        double pressure = 0;      // hPa  pourquoi ? à supprimer
        double temperature = 0; // °C pourquoi ? à supprimer

        sen15901_t dev_sen15901; //voir à mettre hors loop
        saul_reg_t *dev = saul_reg;
        sen15901_values vals;

        LED_GREEN_ON;
        //wait 5 sec before starting mesure
        puts("\n mesure starts in 10");
        for(int i=0;i<5;i++)
			{
                ztimer_sleep(ZTIMER_SEC, 1);   
                LED_RED_ON;
                LED_GREEN_OFF;
                ztimer_sleep(ZTIMER_SEC, 1);   
                LED_RED_OFF;
                LED_GREEN_ON; 
            }
        puts("Mesure starts");

        if (dev == NULL) {
            puts("No SAUL devices present");
            return 1;
        }

        // Lecture capteur luminosité
        display_luminosity(sample);

        // Plutôt utiliser boucle du saul pour plus de lisibilité pour pression et accéléromètre

        while (dev) { //Attention boucle infinie à corriger
            int dim = saul_reg_read(dev, &res);
            printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,
                   saul_class_to_str(dev->driver->type));
            phydat_dump(&res, dim);
            dev = dev->next;
        }
        puts("\n##########################");

        //    xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    

        // Lecture capteur pression (déjà fait dans boucle saul)
		/*	int dim = saul_reg_read(dev, &res);
            printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,saul_class_to_str(dev->driver->type));
            phydat_dump(&res, dim);
            dev = dev->next;
            pressure = res.val[0];*/

            // Lecture capteur température à remplacer par le bme
            /*dim = saul_reg_read(dev, &res);
            printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,saul_class_to_str(dev->driver->type));
           // senml_value_t test_senml;
           // phydat_to_senml_float(&test_senml, &res,dim);
            phydat_dump(&res, dim);
            temperature =res.val[0]/100.00;
           // temperature = (double)test_senml.value.value.f;
            dev = dev->next;*/

			if (initialization_sen15901(dev_sen15901) == 1)
            {   
                vals = display_sen15901(dev_sen15901, duration);      
	        }
    
        
				
        puts("Mesure ends");
        

        double latitude;
        double longitude;
        cassiniOval(0, 1, 2, &latitude, &longitude);
        latitude = init_latitude;
        longitude = init_longitude;
        double altitude = init_altitude ;    // meter
        //double pressure = init_pressure;      // hPa
        //double temperature = init_temperature ; // °C
        double humidity = init_humidity ;
        double luninosity = (double) sample;
        double battery_voltage = init_battery_voltage; // V

        /**** Build cayenne payload ****/
        cayenne_lpp_reset(&lpp);
        cayenne_lpp_add_temperature(&lpp, 1, temperature);
        cayenne_lpp_add_relative_humidity(&lpp, 2, humidity);
        cayenne_lpp_add_barometric_pressure(&lpp, 3, pressure);
        cayenne_lpp_add_luminosity(&lpp, 4, luninosity);
        //cayenne_lpp_add_gps(&lpp, 5, latitude, longitude, altitude);
        cayenne_lpp_add_analog_input(&lpp, 6, battery_voltage);
        cayenne_lpp_add_analog_input(&lpp, 7, (double)vals.water_level);
		cayenne_lpp_add_analog_input(&lpp, 8, (double)vals.wind_speed);
		cayenne_lpp_add_analog_input(&lpp, 9, (double)vals.orientation);

        printf("\n==== Point #test ====\n");

        printf(" temperature:     %.2f °C\n",temperature);
        printf(" humidity:        %.1f RH\n",humidity);
        printf(" pressure:        %.0f hPa\n",pressure);
        printf(" luninosity:      %.1f lux\n",luninosity);
        printf(" position:        lat=%.5f° lon=%.5f° alt=%.0fm\n",latitude, longitude, altitude);
        printf(" battery_voltage: %.1f mV\n",battery_voltage);

       // uint8_t ucnf = LORAMAC_TX_UNCNF;  /* Default: confirmable */
      //  uint8_t port = CONFIG_LORAMAC_DEFAULT_TX_PORT; /* Default: 2 */
       // semtech_loramac_set_tx_mode(&loramac, ucnf);
        //semtech_loramac_set_tx_port(&loramac, port);
        //semtech_loramac_set_dr(&loramac, 5);
        // _print_buffer(lpp.buffer, lpp.cursor, "LPP: ");
       // semtech_loramac_set_class(&lorama, loramac_class_t cls);

        /*Sending of the buffer */
        puts("LORAMAC start sending buffer ");
        printf("tx mode : %d",semtech_loramac_get_tx_mode(&loramac));
        printf("\n dr : %d",semtech_loramac_get_dr(&loramac));

    LED_GREEN_OFF;
    LED_RED_ON;

    ztimer_sleep(ZTIMER_SEC, 10);


        switch (semtech_loramac_send(&loramac,lpp.buffer, lpp.cursor)) {

        case SEMTECH_LORAMAC_NOT_JOINED:
            puts("Cannot send: not joined");
            break;

        case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
            puts("Cannot send: dutycycle restriction");
            break;

        case SEMTECH_LORAMAC_BUSY:
            puts("Cannot send: MAC is busy");
            break;

        case SEMTECH_LORAMAC_TX_ERROR:
            puts("Cannot send: error");
            _print_buffer(lpp.buffer, lpp.cursor, "Cannot send message LPP: ");
            break;

        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            puts("Fail to send: no ACK received");
            break;
        case SEMTECH_LORAMAC_TX_DONE:
            _print_buffer(lpp.buffer, lpp.cursor, "Message sent with success: ");
            break;

        
    }   
        puts("LORAMAC buffer sent");
        puts("LORAMAC start waiting");
        LED_RED_OFF;
        ztimer_sleep(ZTIMER_SEC, 60);
       
    }
}
