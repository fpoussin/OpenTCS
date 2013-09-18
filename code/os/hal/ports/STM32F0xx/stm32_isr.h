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

/**
 * @file    STM32F0xx/stm32_isr.h
 * @brief   ISR remapper driver header.
 *
 * @addtogroup STM32F0xx_ISR
 * @{
 */

#ifndef _STM32_ISR_H_
#define _STM32_ISR_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISR names and numbers remapping
 * @{
 */

/**
 * @name  IRQ VECTOR names
 * @{
 */
#define WWDG_IRQHandler                 Vector40    /**< Window Watchdog.           */
#define PVD_IRQHandler                  Vector44    /**< PVD through EXTI Line
                                                    detect.                    */
#define RTC_IRQHandler                  Vector48    /**< RTC through EXTI Line
                                                    Interrupt.                 */
#define FLASH_IRQHandler                Vector4C    /**< Flash.                     */
#define RCC_IRQHandler                  Vector50    /**< RCC.                       */
#define EXTI0_1_IRQHandler              Vector54    /**< EXTI Line 0..1.            */
#define EXTI2_3_IRQHandler              Vector58    /**< EXTI Line 2..3.            */
#define EXTI4_15_IRQHandler             Vector5C    /**< EXTI Line 4..15.           */
#define TS_IRQHandler                   Vector60    /**< TS.                        */
#define DMA1_Ch1_IRQHandler             Vector64    /**< DMA1 Channel 1.            */
#define DMA1_Ch2_3_IRQHandler           Vector68    /**< DMA1 Channels 2 and 3.     */
#define DMA1_Ch4_5_IRQHandler           Vector6C    /**< DMA1 Channels 4 and 5.     */
#define ADC1_COMP_IRQHandler            Vector70    /**< ADC1 comparators 1 and 2.  */
#define TIM1_BRK_UP_TRG_COM_IRQHandler  Vector74    /**< TIM1 common.           */
#define TIM1_CC_IRQHandler              Vector78    /**< TIM1 Capture Compare.      */
#define TIM2_IRQHandler                 Vector7C    /**< TIM2.                      */
#define TIM3_IRQHandler                 Vector80    /**< TIM3.                      */
#define TIM6_DAC_IRQHandler             Vector84    /**< TIM6 and DAC.              */
#define TIM14_IRQHandler                Vector8C    /**< TIM14.                     */
#define TIM15_IRQHandler                Vector90    /**< TIM15.                     */
#define TIM16_IRQHandler                Vector94    /**< TIM16.                     */
#define TIM17_IRQHandler                Vector98    /**< TIM17.                     */
#define I2C1_IRQHandler                 Vector9C    /**< I2C1.                      */
#define I2C2_IRQHandler                 VectorA0    /**< I2C2.                      */
#define SPI1_IRQHandler                 VectorA4    /**< SPI1.                      */
#define SPI2_IRQHandler                 VectorA8    /**< SPI2.                      */
#define USART1_IRQHandler               VectorAC    /**< USART1.                    */
#define USART2_IRQHandler               VectorB0    /**< USART2.                    */
#define CEC_IRQHandler                  VectorB8    /**< CEC.                       */

/*
 * I2C units.
 */
#define STM32_I2C1_GLOBAL_HANDLER   Vector9C
#define STM32_I2C1_GLOBAL_NUMBER    23

#define STM32_I2C2_GLOBAL_HANDLER   VectorA0
#define STM32_I2C2_GLOBAL_NUMBER    24

/*
 * TIM units.
 */
#define STM32_TIM1_UP_HANDLER       Vector74
#define STM32_TIM1_CC_HANDLER       Vector78
#define STM32_TIM2_HANDLER          Vector7C
#define STM32_TIM3_HANDLER          Vector80
#define STM32_TIM6_DAC_HANDLER      Vector84
#define STM32_TIM14_HANDLER         Vector8C
#define STM32_TIM15_HANDLER         Vector90
#define STM32_TIM16_HANDLER         Vector94
#define STM32_TIM17_HANDLER         Vector98

#define STM32_TIM1_UP_NUMBER        13
#define STM32_TIM1_CC_NUMBER        14
#define STM32_TIM2_NUMBER           15
#define STM32_TIM3_NUMBER           16
#define STM32_TIM6_DAC_NUMBER       17
#define STM32_TIM14_NUMBER          19
#define STM32_TIM15_NUMBER          20
#define STM32_TIM16_NUMBER          21
#define STM32_TIM17_NUMBER          22

/*
 * USART units.
 */
#define STM32_USART1_HANDLER        VectorAC
#define STM32_USART2_HANDLER        VectorB0

#define STM32_USART1_NUMBER         27
#define STM32_USART2_NUMBER         28
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#endif /* _STM32_ISR_H_ */

/** @} */
