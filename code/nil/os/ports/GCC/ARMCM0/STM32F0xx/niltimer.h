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

#define TICK_TIMER    TIM14

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

  TICK_TIMER->ARR     = 0xFFFF;
  TICK_TIMER->CCMR1   = 0;
  TICK_TIMER->CCR1  = 0;
  TICK_TIMER->DIER    = 0;
  TICK_TIMER->CR2     = 0;
  TICK_TIMER->EGR     = 1;            /* UG, CNT initialized.             */
  TICK_TIMER->CR1     = 1;            /* CEN */
}

/**
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {

  return TICK_TIMER->CNT;
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

  nilDbgAssert((TICK_TIMER->DIER & 2) == 0,
               "port_timer_start_alarm(), #1",
               "already started");

  TICK_TIMER->CCR1  = time;
  TICK_TIMER->SR      = 0;
  TICK_TIMER->DIER    = 2;            /* CC1IE */
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void port_timer_stop_alarm(void) {

  nilDbgAssert((TICK_TIMER->DIER & 2) != 0,
               "port_timer_stop_alarm(), #1",
               "not started");

  TICK_TIMER->DIER    = 0;
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void port_timer_set_alarm(systime_t time) {

  nilDbgAssert((TICK_TIMER->DIER & 2) != 0,
               "port_timer_set_alarm(), #1",
               "not started");

  TICK_TIMER->CCR1  = time;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_alarm(void) {

  nilDbgAssert((TICK_TIMER->DIER & 2) != 0,
               "port_timer_get_alarm(), #1",
               "not started");

  return TICK_TIMER->CCR1;
}

#endif /* _NILTIMER_H_ */

/** @} */
