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
 * @brief       temperature sensors functions
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

#include "fmt.h"


#define ADC_IN_USE 			ADC_LINE(0)
#define ADC_RES				ADC_RES_12BIT
#define M_PI  (3.14159265358979323846)


#include "ztimer.h"


#include <stdio.h>
#ifdef MODULE_SEMTECH_LORAMAC_RX
#include "thread.h"
#include "msg.h"
#endif

 

int Read_pressure_and_temperature(void)
{      
    phydat_t res;
    saul_reg_t *dev = saul_reg;
    if (dev == NULL) {
        puts("No SAUL devices present");
        return 1;
    }
    double pressure = 500;    // hPa
    double temperature = -20; // °C
    // Pressure sensor read 
	int dim = saul_reg_read(dev, &res);
    printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,saul_class_to_str(dev->driver->type));
    phydat_dump(&res, dim);
            
    dev = dev->next;
    pressure = res.val[0];
    // Lecture capteur température
    dim = saul_reg_read(dev, &res);
    printf("\nDev: %s\tType: %" PRIsflash "\n", dev->name,saul_class_to_str(dev->driver->type));
    phydat_dump(&res, dim);
    temperature =res.val[0]/100.00;
    dev = dev->next;
}

