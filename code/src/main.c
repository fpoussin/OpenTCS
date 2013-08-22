#include "nil.h"
#include "hwinit.h"
#include "threads.h"

/*
 * Thread 1.
 */
NIL_WORKING_AREA(waThread1, 128);
NIL_THREAD(Thread1, arg) {

  (void)arg;
  while (true) {
    startlight();
  }
}

/*
 * Thread 2.
 */
NIL_WORKING_AREA(waThread2, 128);
NIL_THREAD(Thread2, arg) {

  (void)arg;

  while (true) {
    startOled();
  }
}

/*
 * Thread 3.
 */
NIL_WORKING_AREA(waThread3, 128);
NIL_THREAD(Thread3, arg) {

  (void)arg;

  while (true) {
    startIgnition();
  }
}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
NIL_THREADS_TABLE_BEGIN()
  NIL_THREADS_TABLE_ENTRY("light", Thread1, NULL, waThread1, sizeof(waThread1))
  NIL_THREADS_TABLE_ENTRY("oled", Thread2, NULL, waThread2, sizeof(waThread2))
  NIL_THREADS_TABLE_ENTRY("ignition", Thread2, NULL, waThread3, sizeof(waThread3))
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
