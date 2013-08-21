/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERNILANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    nilmboxes.h
 * @brief   Mailboxes macros and structures.
 *
 * @addtogroup mailboxes
 * @{
 */

#ifndef _NILMBOXES_H_
#define _NILMBOXES_H_

#if NIL_USE_MAILBOXES || defined(__DOXYGEN__)

#define RDY_OK 0
#define RDY_TIMEOUT 1

/**
 * @brief   Structure representing a mailbox object.
 */
typedef struct {
  msg_t                 *mb_buffer;     /**< @brief Pointer to the mailbox
                                                    buffer.                 */
  msg_t                 *mb_top;        /**< @brief Pointer to the location
                                                    after the buffer.       */
  msg_t                 *mb_wrptr;      /**< @brief Write pointer.          */
  msg_t                 *mb_rdptr;      /**< @brief Read pointer.           */
  semaphore_t             mb_fullsem;     /**< @brief Full counter
                                                    @p Semaphore.           */
  semaphore_t             mb_emptysem;    /**< @brief Empty counter
                                                    @p Semaphore.           */
} Mailbox;

#ifdef __cplusplus
extern "C" {
#endif
  void nilMBInit(Mailbox *mbp, msg_t *buf, cnt_t n);
  void nilMBReset(Mailbox *mbp);
  msg_t nilMBPost(Mailbox *mbp, msg_t msg, systime_t timeout);
  msg_t nilMBPostS(Mailbox *mbp, msg_t msg, systime_t timeout);
  msg_t nilMBPostI(Mailbox *mbp, msg_t msg);
  msg_t nilMBPostAhead(Mailbox *mbp, msg_t msg, systime_t timeout);
  msg_t nilMBPostAheadS(Mailbox *mbp, msg_t msg, systime_t timeout);
  msg_t nilMBPostAheadI(Mailbox *mbp, msg_t msg);
  msg_t nilMBFetch(Mailbox *mbp, msg_t *msgp, systime_t timeout);
  msg_t nilMBFetchS(Mailbox *mbp, msg_t *msgp, systime_t timeout);
  msg_t nilMBFetchI(Mailbox *mbp, msg_t *msgp);
#ifdef __cplusplus
}
#endif

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the mailbox buffer size.
 *
 * @param[in] mbp       the pointer to an initialized Mailbox object
 *
 * @iclass
 */
#define nilMBSizeI(mbp)                                                      \
        ((mbp)->mb_top - (mbp)->mb_buffer)

/**
 * @brief   Returns the number of free message slots into a mailbox.
 * @note    Can be invoked in any system state but if invoked out of a locked
 *          state then the returned value may change after reading.
 * @note    The returned value can be less than zero when there are waiting
 *          threads on the internal semaphore.
 *
 * @param[in] mbp       the pointer to an initialized Mailbox object
 * @return              The number of empty message slots.
 *
 * @iclass
 */
#define nilMBGetFreeCountI(mbp) nilSemGetCounterI(&(mbp)->mb_emptysem)

/**
 * @brief   Returns the number of used message slots into a mailbox.
 * @note    Can be invoked in any system state but if invoked out of a locked
 *          state then the returned value may change after reading.
 * @note    The returned value can be less than zero when there are waiting
 *          threads on the internal semaphore.
 *
 * @param[in] mbp       the pointer to an initialized Mailbox object
 * @return              The number of queued messages.
 *
 * @iclass
 */
#define nilMBGetUsedCountI(mbp) nilSemGetCounterI(&(mbp)->mb_fullsem)

/**
 * @brief   Returns the next message in the queue without removing it.
 * @pre     A message must be waiting in the queue for this function to work
 *          or it would return garbage. The correct way to use this macro is
 *          to use @p nilMBGetFullCountI() and then use this macro, all within
 *          a lock state.
 *
 * @iclass
 */
#define nilMBPeekI(mbp) (*(mbp)->mb_rdptr)
/** @} */

/**
 * @brief   Data part of a static mailbox initializer.
 * @details This macro should be used when statically initializing a
 *          mailbox that is part of a bigger structure.
 *
 * @param[in] name      the name of the mailbox variable
 * @param[in] buffer    pointer to the mailbox buffer area
 * @param[in] size      size of the mailbox buffer area
 */
#define _MAILBOX_DATA(name, buffer, size) {                             \
  (msg_t *)(buffer),                                                    \
  (msg_t *)(buffer) + size,                                             \
  (msg_t *)(buffer),                                                    \
  (msg_t *)(buffer),                                                    \
  _SEMAPHORE_DATA(name.mb_fullsem, 0),                                  \
  _SEMAPHORE_DATA(name.mb_emptysem, size),                              \
}

/**
 * @brief   Static mailbox initializer.
 * @details Statically initialized mailboxes require no explicit
 *          initialization using @p nilMBInit().
 *
 * @param[in] name      the name of the mailbox variable
 * @param[in] buffer    pointer to the mailbox buffer area
 * @param[in] size      size of the mailbox buffer area
 */
#define MAILBOX_DECL(name, buffer, size)                                \
  Mailbox name = _MAILBOX_DATA(name, buffer, size)

#endif /* NIL_USE_MAILBOXES */

#endif /* _NILMBOXES_H_ */

/** @} */
