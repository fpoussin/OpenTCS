/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F0-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_OPENTCS
#define BOARD_NAME "Open TCS"

/*
 * Board frequencies.
 * NOTE: The both crystals are not fitted by default on the board but there
 *       the option to both mount an 8MHz HE crystal or drive HSE with an
 *       8MHz clock from the on-board ST-Link. LSE can be optionally mounted
 *       too.
 */
#if defined(DISCOVERY_HSE_MOUNTED) && defined(DISCOVERY_HSE_DRIVEN)
#error "HSE OSC-IN cannot be both crystal equipped and externally driven"
#endif

#if defined(DISCOVERY_LSE_MOUNTED)
#define STM32_LSECLK            32768
#else
#define STM32_LSECLK            0
#endif

#if defined(DISCOVERY_HSE_MOUNTED) || defined(DISCOVERY_HSE_DRIVEN)
#define STM32_HSECLK            8000000
#else
#define STM32_HSECLK            0
#endif

#if defined(DISCOVERY_HSE_DRIVEN)
#define STM32_HSE_BYPASS
#endif

/*
 * MCU type as defined in the ST header file stm32f0xx.h.
 */
#define STM32F0XX

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON            0

#define GPIOC_LED4              8
#define GPIOC_LED3              9

#define GPIOC_BUTTON_SEL            13
#define GPIOC_BUTTON_UP            14
#define GPIOC_BUTTON_DOWN            15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_400K(n)          (0U << ((n) * 2))
#define PIN_OSPEED_2M(n)            (1U << ((n) * 2))
#define PIN_OSPEED_10M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_40M(n)           (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * Port A setup.
 * All input with pull-up except:
 * PA0  - GPIOA_BUTTON  (input floating).
 * PA13 - JTMS/SWDAT    (alternate 0).
 * PA14 - JTCK/SWCLK    (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(0) |         \
                                     PIN_MODE_ANALOG(1) |                    \
                                     PIN_MODE_ANALOG(2) |                    \
                                     PIN_MODE_ANALOG(3) |                    \
                                     PIN_MODE_ANALOG(4) |                    \
                                     PIN_MODE_ANALOG(5) |                    \
                                     PIN_MODE_ALTERNATE(6) |                    \
                                     PIN_MODE_ALTERNATE(7) |                    \
                                     PIN_MODE_ANALOG(8) |                    \
                                     PIN_MODE_ALTERNATE(9) |                    \
                                     PIN_MODE_ALTERNATE(10) |                   \
                                     PIN_MODE_ALTERNATE(11) |                   \
                                     PIN_MODE_ANALOG(12) |                   \
                                     PIN_MODE_ALTERNATE(13) |               \
                                     PIN_MODE_ALTERNATE(14) |               \
                                     PIN_MODE_ALTERNATE(15))
#define VAL_GPIOA_OTYPER            PIN_OTYPE_PUSHPULL(0) |                   \
                                    PIN_OTYPE_PUSHPULL(1) |                   \
                                    PIN_OTYPE_PUSHPULL(2) |                   \
                                    PIN_OTYPE_PUSHPULL(3) |                   \
                                    PIN_OTYPE_PUSHPULL(4) |                   \
                                    PIN_OTYPE_PUSHPULL(5) |                   \
                                    PIN_OTYPE_PUSHPULL(6) |                   \
                                    PIN_OTYPE_PUSHPULL(7) |                   \
                                    PIN_OTYPE_PUSHPULL(8) |                   \
                                    PIN_OTYPE_PUSHPULL(9) |                   \
                                    PIN_OTYPE_PUSHPULL(10) |                   \
                                    PIN_OTYPE_PUSHPULL(11) |                   \
                                    PIN_OTYPE_PUSHPULL(12) |                   \
                                    PIN_OTYPE_PUSHPULL(13) |                   \
                                    PIN_OTYPE_PUSHPULL(14) |                   \
                                    PIN_OTYPE_PUSHPULL(15)
#define VAL_GPIOA_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOA_PUPDR             (PIN_PUDR_FLOATING(0) |      \
                                     PIN_PUDR_FLOATING(1) |                   \
                                     PIN_PUDR_FLOATING(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_FLOATING(13) |                \
                                     PIN_PUDR_FLOATING(14) |                \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOA_ODR               0xFFFFFFFF
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(0, 0) |    \
                                     PIN_AFIO_AF(1, 0) |           \
                                     PIN_AFIO_AF(2, 0) |           \
                                     PIN_AFIO_AF(3, 0) |           \
                                     PIN_AFIO_AF(4, 0) |           \
                                     PIN_AFIO_AF(5, 0) |           \
                                     PIN_AFIO_AF(6, 1) |           \
                                     PIN_AFIO_AF(7, 1))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(8, 0) |    \
                                     PIN_AFIO_AF(9, 1) |           \
                                     PIN_AFIO_AF(10, 1) |           \
                                     PIN_AFIO_AF(11, 2) |           \
                                     PIN_AFIO_AF(12, 0) |           \
                                     PIN_AFIO_AF(13, 0) |           \
                                     PIN_AFIO_AF(14, 0) |           \
                                     PIN_AFIO_AF(15, 0))

/*
 * Port B setup.
 * All input with pull-up except:
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(0) |                    \
                                     PIN_MODE_ALTERNATE(1) |                    \
                                     PIN_MODE_ANALOG(2) |                    \
                                     PIN_MODE_ALTERNATE(3) |                    \
                                     PIN_MODE_ALTERNATE(4) |                    \
                                     PIN_MODE_ALTERNATE(5) |                    \
                                     PIN_MODE_ALTERNATE(6) |                   \
                                     PIN_MODE_ALTERNATE(7) |                  \
                                     PIN_MODE_ALTERNATE(8) |                    \
                                     PIN_MODE_ANALOG(9) |                    \
                                     PIN_MODE_ALTERNATE(10) |                   \
                                     PIN_MODE_ALTERNATE(11) |                   \
                                     PIN_MODE_OUTPUT(12) |                   \
                                     PIN_MODE_OUTPUT(13) |                   \
                                     PIN_MODE_ANALOG(14) |                   \
                                     PIN_MODE_ANALOG(15))
#define VAL_GPIOB_OTYPER            PIN_OTYPE_PUSHPULL(0) |                   \
                                    PIN_OTYPE_PUSHPULL(1) |                   \
                                    PIN_OTYPE_PUSHPULL(2) |                   \
                                    PIN_OTYPE_PUSHPULL(3) |                   \
                                    PIN_OTYPE_PUSHPULL(4) |                   \
                                    PIN_OTYPE_PUSHPULL(5) |                   \
                                    PIN_OTYPE_PUSHPULL(6) |                   \
                                    PIN_OTYPE_PUSHPULL(7) |                   \
                                    PIN_OTYPE_PUSHPULL(8) |                   \
                                    PIN_OTYPE_PUSHPULL(9) |                   \
                                    PIN_OTYPE_PUSHPULL(10) |                   \
                                    PIN_OTYPE_PUSHPULL(11) |                   \
                                    PIN_OTYPE_PUSHPULL(12) |                   \
                                    PIN_OTYPE_PUSHPULL(13) |                   \
                                    PIN_OTYPE_PUSHPULL(14) |                   \
                                    PIN_OTYPE_PUSHPULL(15)
#define VAL_GPIOB_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOB_PUPDR             (PIN_PUDR_PULLUP(0) |                   \
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_FLOATING(6) |                   \
                                     PIN_PUDR_FLOATING(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                   \
                                     PIN_PUDR_PULLUP(9) |                   \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                  \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOB_ODR               0xFFFFFFFF
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0, 1) |    \
                                     PIN_AFIO_AF(1, 1) |           \
                                     PIN_AFIO_AF(2, 0) |           \
                                     PIN_AFIO_AF(3, 0) |           \
                                     PIN_AFIO_AF(4, 0) |           \
                                     PIN_AFIO_AF(5, 0) |           \
                                     PIN_AFIO_AF(6, 1) |           \
                                     PIN_AFIO_AF(7, 1))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(8, 2) |    \
                                     PIN_AFIO_AF(9, 0) |           \
                                     PIN_AFIO_AF(10, 0) |           \
                                     PIN_AFIO_AF(11, 0) |           \
                                     PIN_AFIO_AF(12, 0) |           \
                                     PIN_AFIO_AF(13, 0) |           \
                                     PIN_AFIO_AF(14, 0) |           \
                                     PIN_AFIO_AF(15, 0))

/*
 * Port C setup.
 * All input with pull-up except:
 * PC8  - GPIOC_LED4    (output push-pull).
 * PC9  - GPIOC_LED3    (output push-pull).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(0) |                    \
                                     PIN_MODE_ANALOG(1) |                    \
                                     PIN_MODE_ANALOG(2) |                    \
                                     PIN_MODE_ANALOG(3) |                    \
                                     PIN_MODE_ANALOG(4) |                    \
                                     PIN_MODE_ANALOG(5) |                    \
                                     PIN_MODE_ANALOG(6) |                    \
                                     PIN_MODE_ANALOG(7) |                    \
                                     PIN_MODE_ANALOG(8) |                   \
                                     PIN_MODE_ANALOG(9) |                   \
                                     PIN_MODE_ANALOG(10) |                   \
                                     PIN_MODE_ANALOG(11) |                   \
                                     PIN_MODE_ANALOG(12) |                   \
                                     PIN_MODE_INPUT(13) |                   \
                                     PIN_MODE_INPUT(14) |                   \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER            PIN_OTYPE_PUSHPULL(0) |                   \
                                    PIN_OTYPE_PUSHPULL(1) |                   \
                                    PIN_OTYPE_PUSHPULL(2) |                   \
                                    PIN_OTYPE_PUSHPULL(3) |                   \
                                    PIN_OTYPE_PUSHPULL(4) |                   \
                                    PIN_OTYPE_PUSHPULL(5) |                   \
                                    PIN_OTYPE_PUSHPULL(6) |                   \
                                    PIN_OTYPE_PUSHPULL(7) |                   \
                                    PIN_OTYPE_PUSHPULL(8) |                   \
                                    PIN_OTYPE_PUSHPULL(9) |                   \
                                    PIN_OTYPE_PUSHPULL(10) |                   \
                                    PIN_OTYPE_PUSHPULL(11) |                   \
                                    PIN_OTYPE_PUSHPULL(12) |                   \
                                    PIN_OTYPE_PUSHPULL(13) |                   \
                                    PIN_OTYPE_PUSHPULL(14) |                   \
                                    PIN_OTYPE_PUSHPULL(15)
#define VAL_GPIOC_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOC_PUPDR             (PIN_PUDR_PULLUP(0) |                   \
                                     PIN_PUDR_PULLUP(1) |                   \
                                     PIN_PUDR_PULLUP(2) |                   \
                                     PIN_PUDR_PULLUP(3) |                   \
                                     PIN_PUDR_PULLUP(4) |                   \
                                     PIN_PUDR_PULLUP(5) |                   \
                                     PIN_PUDR_PULLUP(6) |                   \
                                     PIN_PUDR_PULLUP(7) |                   \
                                     PIN_PUDR_PULLUP(8) |                 \
                                     PIN_PUDR_PULLUP(9) |                 \
                                     PIN_PUDR_PULLUP(10) |                  \
                                     PIN_PUDR_PULLUP(11) |                  \
                                     PIN_PUDR_PULLUP(12) |                  \
                                     PIN_PUDR_PULLUP(13) |                  \
                                     PIN_PUDR_PULLUP(14) |                \
                                     PIN_PUDR_PULLUP(15))
#define VAL_GPIOC_ODR               0xFFFFFCFF
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(0, 0) |    \
                                     PIN_AFIO_AF(1, 0) |           \
                                     PIN_AFIO_AF(2, 0) |           \
                                     PIN_AFIO_AF(3, 0) |           \
                                     PIN_AFIO_AF(4, 0) |           \
                                     PIN_AFIO_AF(5, 0) |           \
                                     PIN_AFIO_AF(6, 0) |           \
                                     PIN_AFIO_AF(7, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(8, 0) |    \
                                     PIN_AFIO_AF(9, 0) |           \
                                     PIN_AFIO_AF(10, 0) |           \
                                     PIN_AFIO_AF(11, 0) |           \
                                     PIN_AFIO_AF(12, 0) |           \
                                     PIN_AFIO_AF(13, 0) |           \
                                     PIN_AFIO_AF(14, 0) |           \
                                     PIN_AFIO_AF(15, 0))

/*
 * Port D setup.
 * Disabled.
 */
#define VAL_GPIOD_MODER             0x00000000
#define VAL_GPIOD_OTYPER            0x00000000
#define VAL_GPIOD_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOD_PUPDR             0x00000000
#define VAL_GPIOD_ODR               0xFFFFFFFF
#define VAL_GPIOD_AFRL              0x00000000
#define VAL_GPIOD_AFRH              0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(0) |  \
                                     PIN_MODE_ANALOG(1) |  \
                                     PIN_MODE_ANALOG(6) |  \
                                     PIN_MODE_ANALOG(7))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(0) |                   \
                                    PIN_OTYPE_PUSHPULL(1) | \
                                    PIN_OTYPE_PUSHPULL(6) | \
                                    PIN_OTYPE_PUSHPULL(7))
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             (PIN_PUDR_PULLUP(0) |                   \
                                     PIN_PUDR_PULLUP(1) | \
                                     PIN_PUDR_PULLUP(6) | \
                                     PIN_PUDR_PULLUP(7))
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(0, 0) |    \
                                     PIN_AFIO_AF(1, 0) | \
                                     PIN_AFIO_AF(6, 0) |\
                                     PIN_AFIO_AF(7, 0))
#define VAL_GPIOF_AFRH              0x00000000

#endif /* _BOARD_H_ */
