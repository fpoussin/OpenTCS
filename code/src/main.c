#include "nil.h"
#include "hwinit.h"

/*
 * Thread 1.
 */
NIL_WORKING_AREA(waThread1, 128);
NIL_THREAD(Thread1, arg) {

  (void)arg;

  while (true) {
    gpioSetPad(GPIOC, GPIOC_LED4);
    nilThdSleepMilliseconds(500);
    gpioClearPad(GPIOC, GPIOC_LED4);
    nilThdSleepMilliseconds(500);
  }
}

/*
 * Thread 2.
 */
NIL_WORKING_AREA(waThread2, 128);
NIL_THREAD(Thread2, arg) {

  (void)arg;

  while (true) {
    gpioSetPad(GPIOC, GPIOC_LED3);
    nilThdSleepMilliseconds(250);
    gpioClearPad(GPIOC, GPIOC_LED3);
    nilThdSleepMilliseconds(250);
  }
}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
NIL_THREADS_TABLE_BEGIN()
  NIL_THREADS_TABLE_ENTRY("thread1", Thread1, NULL, waThread1, sizeof(waThread1))
  NIL_THREADS_TABLE_ENTRY("thread2", Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations:
   * - HW specific initialization.
   * - Nil RTOS initialization.
   */
  hwInit();
  nilSysInit();

  /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop.*/
  while (true) {
  }
}
