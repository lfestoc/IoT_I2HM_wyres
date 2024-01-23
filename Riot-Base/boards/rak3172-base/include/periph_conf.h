/*
 * Copyright (C) 2023 Université Grenoble Alpes
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     boards_rak3172
 * @{
 *
 * @file
 * @brief       Board specific definitions for the wyres_base board
 *
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

//#ifdef TODO
//#error To be implemented
/* This board provides a LSE, marked as X1 on the board */
#ifndef CONFIG_BOARD_HAS_LSE
#define CONFIG_BOARD_HAS_LSE 1
#endif

/* This board provides a HSE, marked as X2 on the board */
#ifndef CONFIG_BOARD_HAS_HSE
#define CONFIG_BOARD_HAS_HSE 1
#endif

/* The HSE provides a 16 MHz clock */
#ifndef CONFIG_CLOCK_HSE
#define CONFIG_CLOCK_HSE MHZ(16)
#endif
//#endif

#include "periph_cpu.h"
#include "clk_conf.h"

/**
 * @name    Timer configuration
 * @{
 */
#if 0
static const timer_conf_t timer_config[] = {
    {
        .dev      = TIM2,
        .max      = 0x0000ffff,
        .rcc_mask = RCC_APB1ENR_TIM2EN,
        .bus      = APB1,
        .irqn     = TIM2_IRQn
    }
};

#endif


/** @} */

/**
 * @name    UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = USART1,
        .rcc_mask   = RCC_APB2ENR_USART1EN,
        .rx_pin     = GPIO_PIN(PORT_B, 7),
        .tx_pin     = GPIO_PIN(PORT_B, 6),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB2,
        .irqn       = USART1_IRQn
    }
};

#define UART_0_ISR          (isr_usart1)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

#define PWM_NUMOF           ARRAY_SIZE(pwm_config)
/** @} */

/**
 * @name    SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
    	// SPI bus dedicated to SX1272/SX1261
        .dev      = SPI1,
        .mosi_pin = GPIO_PIN(PORT_A,7),
        .miso_pin = GPIO_PIN(PORT_A,6),
        .sclk_pin = GPIO_PIN(PORT_A,5),
        .cs_pin   = GPIO_PIN(PORT_A,4),
        .mosi_af  = GPIO_AF5,
        .miso_af  = GPIO_AF5,
        .sclk_af  = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI1EN,
        .apbbus   = APB2
    }
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

/**
 * @name I2C configuration
  * @{
 */
static const i2c_conf_t i2c_config[] = {
//#ifdef TODO
//#error To be implemented
    {
        .dev            = I2C1,
        .speed          = I2C_SPEED_NORMAL,
        .scl_pin        = GPIO_PIN(PORT_A, 12),
        .sda_pin        = GPIO_PIN(PORT_A, 11),
        .scl_af         = GPIO_AF4,
        .sda_af         = GPIO_AF4,
        .bus            = APB1,
        .rcc_mask       = RCC_APB1ENR_I2C1EN,
        .clk            = CLOCK_APB1,
        .irqn           = I2C1_EV_IRQn
    }
};

//#ifdef TODO
//#error To be implemented
#define I2C_0_ISR           isr_i2c1_ev
#endif

#define I2C_NUMOF           ARRAY_SIZE(i2c_config)
/** @} */


#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
