/*
 * Copyright (C) 2020-2021 Université Grenoble Alpes
 */

#include "fmt.h"

/*
 * @brief Data struct for I2HM 
 */

typedef struct 
{
   double temperature;
   double pressure; 
   double luminosity;
   double humidity;
   uint16_t water_level; 
   uint16_t wind_speed; 
   uint16_t wind_direction;

}I2HM_data;

/*
 * @brief 
 *
 * @param 
 * @param 
 */