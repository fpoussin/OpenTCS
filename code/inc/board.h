/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for ST STM32F0-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F0_DISCOVERY
#define BOARD_NAME                  "ST STM32F0-Discovery"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 * NOTE: HSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#define STM32_LSEDRV                (3 << 3)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                0
#endif

#define STM32_HSE_BYPASS

/*
 * MCU type as defined in the ST header file stm32f0xx.h.
 */
#define STM32F0XX_MD

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON                0
#define GPIOA_PIN1                  1
#define GPIOA_PIN2                  2
#define GPIOA_PIN3                  3
#define GPIOA_PIN4                  4
#define GPIOA_PIN5                  5
#define GPIOA_PIN6                  6
#define GPIOA_PIN7                  7
#define GPIOA_PIN8                  8
#define GPIOA_PIN9                  9
#define GPIOA_PIN10                 10
#define GPIOA_PIN11                 11
#define GPIOA_PIN12                 12
#define GPIOA_SWDAT                 13
#define GPIOA_SWCLK                 14
#define GPIOA_PIN15                 15

#define GPIOB_PIN0                  0
#define GPIOB_PIN1                  1
#define GPIOB_PIN2                  2
#define GPIOB_PIN3                  3
#define GPIOB_PIN4                  4
#define GPIOB_PIN5                  5
#define GPIOB_PIN6                  6
#define GPIOB_PIN7                  7
#define GPIOB_PIN8                  8
#define GPIOB_PIN9                  9
#define GPIOB_PIN10                 10
#define GPIOB_PIN11                 11
#define GPIOB_PIN12                 12
#define GPIOB_PIN13                 13
#define GPIOB_PIN14                 14
#define GPIOB_PIN15                 15

#define GPIOC_PIN0                  0
#define GPIOC_PIN1                  1
#define GPIOC_PIN2                  2
#define GPIOC_PIN3                  3
#define GPIOC_PIN4                  4
#define GPIOC_PIN5                  5
#define GPIOC_PIN6                  6
#define GPIOC_PIN7                  7
#define GPIOC_LED4                  8
#define GPIOC_LED3                  9
#define GPIOC_PIN10                 10
#define GPIOC_PIN11                 11
#define GPIOC_PIN12                 12
#define GPIOC_PIN13                 13
#define GPIOC_OSC32_IN              14
#define GPIOC_OSC32_OUT             15

#define GPIOD_PIN0                  0
#define GPIOD_PIN1                  1
#define GPIOD_PIN2                  2
#define GPIOD_PIN3                  3
#define GPIOD_PIN4                  4
#define GPIOD_PIN5                  5
#define GPIOD_PIN6                  6
#define GPIOD_PIN7                  7
#define GPIOD_PIN8                  8
#define GPIOD_PIN9                  9
#define GPIOD_PIN10                 10
#define GPIOD_PIN11                 11
#define GPIOD_PIN12                 12
#define GPIOD_PIN13                 13
#define GPIOD_PIN14                 14
#define GPIOD_PIN15                 15

#define GPIOF_OSC_IN                0
#define GPIOF_OSC_OUT               1
#define GPIOF_PIN2                  2
#define GPIOF_PIN3                  3
#define GPIOF_PIN4                  4
#define GPIOF_PIN5                  5
#define GPIOF_PIN6                  6
#define GPIOF_PIN7                  7
#define GPIOF_PIN8                  8
#define GPIOF_PIN9                  9
#define GPIOF_PIN10                 10
#define GPIOF_PIN11                 11
#define GPIOF_PIN12                 12
#define GPIOF_PIN13                 13
#define GPIOF_PIN14                 14
#define GPIOF_PIN15                 15

#define GPIOA_BUTTON         0

#define GPIOC_LED4           8
#define GPIOC_LED3           9

#define GPIOC_BUTTON_SEL    13
#define GPIOC_BUTTON_UP     14
#define GPIOC_BUTTON_DOWN   15

#define BUTTON_SEL          (!palReadPad(GPIOC, GPIOC_BUTTON_SEL))
#define BUTTON_UP           (!palReadPad(GPIOC, GPIOC_BUTTON_UP))
#define BUTTON_DOWN         (!palReadPad(GPIOC, GPIOC_BUTTON_DOWN))

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_10M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_40M(n)           (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
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
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(0) |      \
                                     PIN_PUPDR_FLOATING(1) |                   \
                                     PIN_PUPDR_FLOATING(2) |                   \
                                     PIN_PUPDR_PULLUP(3) |                   \
                                     PIN_PUPDR_PULLUP(4) |                   \
                                     PIN_PUPDR_PULLUP(5) |                   \
                                     PIN_PUPDR_PULLUP(6) |                   \
                                     PIN_PUPDR_PULLUP(7) |                   \
                                     PIN_PUPDR_PULLUP(8) |                   \
                                     PIN_PUPDR_PULLUP(9) |                   \
                                     PIN_PUPDR_PULLUP(10) |                  \
                                     PIN_PUPDR_PULLUP(11) |                  \
                                     PIN_PUPDR_PULLUP(12) |                  \
                                     PIN_PUPDR_FLOATING(13) |                \
                                     PIN_PUPDR_FLOATING(14) |                \
                                     PIN_PUPDR_PULLUP(15))
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
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(0) |                   \
                                     PIN_PUPDR_PULLUP(1) |                   \
                                     PIN_PUPDR_PULLUP(2) |                   \
                                     PIN_PUPDR_PULLUP(3) |                   \
                                     PIN_PUPDR_PULLUP(4) |                   \
                                     PIN_PUPDR_PULLUP(5) |                   \
                                     PIN_PUPDR_FLOATING(6) |                   \
                                     PIN_PUPDR_FLOATING(7) |                   \
                                     PIN_PUPDR_PULLUP(8) |                   \
                                     PIN_PUPDR_PULLUP(9) |                   \
                                     PIN_PUPDR_PULLUP(10) |                  \
                                     PIN_PUPDR_PULLUP(11) |                  \
                                     PIN_PUPDR_PULLUP(12) |                  \
                                     PIN_PUPDR_PULLUP(13) |                  \
                                     PIN_PUPDR_PULLUP(14) |                  \
                                     PIN_PUPDR_PULLUP(15))
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
                                     PIN_MODE_OUTPUT(8) |                   \
                                     PIN_MODE_OUTPUT(9) |                   \
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
                                    PIN_PUPDR_FLOATING(8) |                   \
                                    PIN_PUPDR_FLOATING(9) |                   \
                                    PIN_OTYPE_PUSHPULL(10) |                   \
                                    PIN_OTYPE_PUSHPULL(11) |                   \
                                    PIN_OTYPE_PUSHPULL(12) |                   \
                                    PIN_OTYPE_PUSHPULL(13) |                   \
                                    PIN_OTYPE_PUSHPULL(14) |                   \
                                    PIN_OTYPE_PUSHPULL(15)
#define VAL_GPIOC_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(0) |                   \
                                     PIN_PUPDR_PULLUP(1) |                   \
                                     PIN_PUPDR_PULLUP(2) |                   \
                                     PIN_PUPDR_PULLUP(3) |                   \
                                     PIN_PUPDR_PULLUP(4) |                   \
                                     PIN_PUPDR_PULLUP(5) |                   \
                                     PIN_PUPDR_PULLUP(6) |                   \
                                     PIN_PUPDR_PULLUP(7) |                   \
                                     PIN_PUPDR_PULLUP(8) |                 \
                                     PIN_PUPDR_PULLUP(9) |                 \
                                     PIN_PUPDR_PULLUP(10) |                  \
                                     PIN_PUPDR_PULLUP(11) |                  \
                                     PIN_PUPDR_PULLUP(12) |                  \
                                     PIN_PUPDR_PULLUP(13) |                  \
                                     PIN_PUPDR_PULLUP(14) |                \
                                     PIN_PUPDR_PULLUP(15))
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
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLUP(0) |                   \
                                     PIN_PUPDR_PULLUP(1) | \
                                     PIN_PUPDR_PULLUP(6) | \
                                     PIN_PUPDR_PULLUP(7))
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(0, 0) |    \
                                     PIN_AFIO_AF(1, 0) | \
                                     PIN_AFIO_AF(6, 0) |\
                                     PIN_AFIO_AF(7, 0))
#define VAL_GPIOF_AFRH              0x00000000


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
