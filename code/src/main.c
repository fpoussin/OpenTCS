#include "hal.h"
#include "nil.h"
#include "stm32f0xx_conf.h"
#include "threads.h"

/*
 * All clocks are enabled in hwInit();
 * All Pin Mux are set in board.h
 */

/*
 * Thread 0.
 */
THD_WORKING_AREA(waThread0, 64);
THD_FUNCTION(Thread0, arg)
{
    (void)arg;
    if (RCC->CSR & RCC_CSR_WWDGRSTF)
    {
        /* WWDGRST flag set */
        serDbg("\r\n**WWDG Reset!**\r\n\r\n");

        /* Clear reset flags */
        RCC->CSR |= RCC_CSR_RMVF;
    }

    /* WWDG clock counter = (PCLK1 (48MHz)/4096)/8 = 1464Hz (~683 us)  */
    WWDG_SetPrescaler(WWDG_Prescaler_8);

    /* Set Window value to 126; WWDG counter should be refreshed only when the counter
    is below 126 (and greater than 64) otherwise a reset will be generated */
    WWDG_SetWindowValue(126);

    /* Freeze WWDG while core is stopped */
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_WWDG_STOP;

    /* Enable WWDG and set counter value to 127, WWDG timeout = ~683 us * 64 = 43.7 ms
    In this case the refresh window is: ~683 * (127-126)= 0.683ms < refresh window < ~683 * 64 = 43.7ms
    */
    WWDG_Enable(127);
    serDbg("WWDG Started\r\n");
    while (true)
    {
        chThdSleepMilliseconds(25);
        palTogglePad(GPIOC, GPIOC_LED3); /* Watchdog heartbeat */
        WWDG_SetCounter(127);
    }
}

/*
 * Thread 1.
 */
THD_WORKING_AREA(waThread1, 128);
THD_FUNCTION(Thread1, arg)
{
    (void)arg;
    startLight();
}

/*
 * Thread 2.
 */
THD_WORKING_AREA(waThread2, 256);
THD_FUNCTION(Thread2, arg)
{
    (void)arg;
    startDisplay();
}

/*
 * Thread 3.
 */
THD_WORKING_AREA(waThread3, 128);
THD_FUNCTION(Thread3, arg)
{
    (void)arg;
     startIgnition();
}

/*
 * Thread 4.
 */
THD_WORKING_AREA(waThread4, 128);
THD_FUNCTION(Thread4, arg)
{
    (void)arg;
    startAdc(); /* ADC runs in continuous mode with DMA */
    startSensors();
}

///*
// * Thread 5.
// */
//NIL_WORKING_AREA(waThread5, 128);
//THD_FUNCTION(Thread5, arg)
//{
//    (void)arg;
//    startControl();
//}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
THD_TABLE_BEGIN
    THD_TABLE_ENTRY(waThread0, "Watchdog", Thread0, NULL)
    THD_TABLE_ENTRY(waThread1, "Light", Thread1, NULL)
    THD_TABLE_ENTRY(waThread2, "Display", Thread2, NULL)
    THD_TABLE_ENTRY(waThread3, "Ignition", Thread3, NULL)
    THD_TABLE_ENTRY(waThread4, "Sensors", Thread4, NULL)
//    THD_TABLE_ENTRY(waThread5, "Control", Thread5, NULL)
THD_TABLE_END

/*
 * Application entry point.
 */
int main(void)
{
    /*
    * System initializations:
    * - HW specific initialization.
    * - Nil RTOS initialization.
    */
//    hwInit();
    halInit();
    settingsInit();
    usartInit(DBG_USART);
    chSysInit();

    /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop.*/
    while (true) {};
}
