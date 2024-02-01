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
 * @brief       Lora functions
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
#include "fmt.h"


#define ADC_IN_USE 			ADC_LINE(0)
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

 void _lora_init()
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

 int _lora_send_cayenne(semtech_loramac_t *mac,cayenne_lpp_t lpp)
{
    switch (semtech_loramac_send(&loramac,lpp.buffer, lpp.cursor)) {

        case SEMTECH_LORAMAC_NOT_JOINED:
            puts("Cannot send: not joined");
            return -1;

        case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
            puts("Cannot send: dutycycle restriction");
            return -1;

        case SEMTECH_LORAMAC_BUSY:
            puts("Cannot send: MAC is busy");
            return -1;

        case SEMTECH_LORAMAC_TX_ERROR:
            puts("Cannot send: error");
            _print_buffer(lpp.buffer, lpp.cursor, "Cannot send message LPP: ");
            return -1;

        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            puts("Fail to send: no ACK received");
            return -1;
        case SEMTECH_LORAMAC_TX_DONE:
            _print_buffer(lpp.buffer, lpp.cursor, "Message sent with success: ");
            return 0;

        }
return 0;
}
int _lora_wait()
{
    //TODO
}