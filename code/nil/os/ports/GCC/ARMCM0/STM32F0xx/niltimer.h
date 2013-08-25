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

/**
 * @file    STM32F0xx/niltimer.h
 * @brief   Nil RTOS STM32F0 system timer header file.
 *
 * @addtogroup STM32F0_TIMER
 * @{
 */

#ifndef _NILTIMER_H_
#define _NILTIMER_H_

#include "stm32f0xx.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

//#define SYSTICK_TIMER          TIM2
//#define SYSTICK_TIMER_VECTOR   Vector7C
//#define SYSTICK_TIMER_VECTOR_POS   15

#define SYSTICK_TIMER          TIM14
#define SYSTICK_TIMER_VECTOR   Vector8C
#define SYSTICK_TIMER_VECTOR_POS   19

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Timer unit initialization.
 *
 * @notapi
 */
static inline void port_timer_init(void) {

  SYSTICK_TIMER->ARR     = 0xFFFFFFFF;
  SYSTICK_TIMER->CNT     = 0;
  SYSTICK_TIMER->CCMR1   = 0;
  SYSTICK_TIMER->CCR1    = 0;
  SYSTICK_TIMER->DIER    = 0;
  SYSTICK_TIMER->CR2     = 0;
  SYSTICK_TIMER->EGR    |= TIM_EGR_UG;            /* UG, CNT initialized.             */
  SYSTICK_TIMER->CR1    |= TIM_CR1_CEN;            /* CEN */
}

/**
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {

  return SYSTICK_TIMER->CNT;
}

/**
 * @brief   Starts the alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 *
 * @param[in] time      the time to be set for the first alarm
 *
 * @notapi
 */
static inline void port_timer_start_alarm(systime_t time) {

  nilDbgAssert((SYSTICK_TIMER->DIER & 2) == 0,
               "port_timer_start_alarm(), #1",
               "already started");

  SYSTICK_TIMER->CCR1    = time;
  SYSTICK_TIMER->SR      = 0;
  SYSTICK_TIMER->DIER   |= TIM_DIER_CC1IE;           /* CC1IE */
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void port_timer_stop_alarm(void) {

  nilDbgAssert((SYSTICK_TIMER->DIER & 2) != 0,
               "port_timer_stop_alarm(), #1",
               "not started");

  SYSTICK_TIMER->DIER &= ~TIM_DIER_CC1IE;
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void port_timer_set_alarm(systime_t time) {

  nilDbgAssert((SYSTICK_TIMER->DIER & 2) != 0,
               "port_timer_set_alarm(), #1",
               "not started");

  SYSTICK_TIMER->CCR1 = time;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_alarm(void) {

  nilDbgAssert((SYSTICK_TIMER->DIER & 2) != 0,
               "port_timer_get_alarm(), #1",
               "not started");

  return SYSTICK_TIMER->CCR1;
}

#endif /* _NILTIMER_H_ */

/** @} */
