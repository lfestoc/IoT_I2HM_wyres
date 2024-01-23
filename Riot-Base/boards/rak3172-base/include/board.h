/*
 * Copyright (C) 2023 Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_wyres-base
 * @{
 *
 * @file
 * @brief       Board specific definitions for the Wyres Base board.
 *
 * @author      Tristan Lailler <tristan.lailler@etu.univ-grenoble-alpes.fr
 * @author      Didier Donsez <didier.donsez@univ-grenoble-alpes.fr
 */

#ifndef BOARD_H
#define BOARD_H


#include <stdint.h>

#include "cpu.h"
#include "periph_conf.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Xtimer configuration
 * @{
 */
// All timers that are less than XTIMER_BACKOFF ticks in the future will just spin.
//#define XTIMER_BACKOFF              (11)
#define XTIMER_WIDTH                  (16)
/** @} */

/**
 * @name    sx1272 configuration
 * @{
 */

#define SX127X_PARAM_SPI                    (SPI_DEV(0))

#define SX127X_PARAM_SPI_NSS                GPIO_PIN(PORT_A,4)       /* SPI1_CS */

/*
#define SX127X_PARAM_DIO0                   GPIO_PIN(PORT_B,1)       
#define SX127X_PARAM_DIO1                   GPIO_PIN(PORT_B,10)      
#define SX127X_PARAM_DIO2                   GPIO_PIN(PORT_B,11)      
#define SX127X_PARAM_DIO3                   GPIO_PIN(PORT_B,7)       
#define SX127X_PARAM_DIO4                   GPIO_PIN(PORT_B,5)      
#define SX127X_PARAM_DIO5                   GPIO_PIN(PORT_B,4)      
*/
/**< RFO HF or RFO LF */
#define SX127X_PARAM_PASELECT               (SX127X_PA_RFO)

// code to manipulate these IO is in the BSP
#define SX127X_PARAM_TX_SWITCH              GPIO_PIN(PORT_B,6)		/* PA4 */
#define SX127X_PARAM_RX_SWITCH              GPIO_PIN(PORT_B,7)		/* PC13 */

/** @} */

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
//void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
