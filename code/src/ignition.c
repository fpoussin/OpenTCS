#include "threads.h"

#define IGN_TIMER TIM3
#define IGN_TIMER_PSC 48 /* 500KHz (24Mhz/48) */
#define IGN_TIMER_ARR 0xFFFF
/* Convert x from ms to us, divide by timer clock period in us */
#define IGN_TIMER_CONV(x) (IGN_TIMER_ARR-(((uint32_t)x*1000)/(1000000/(STM32_PCLK/IGN_TIMER_PSC))))

ign_cut_t ign_cut = {IGN_CUT_DISABLED, 25};

/*
 * Actual functions.
 */

void startIgnition(void) {

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16_t pulses[4] = {0,0,0,0};

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = IGN_TIMER_PSC - 1;
    TIM_TimeBaseStructure.TIM_Period = IGN_TIMER_ARR;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(IGN_TIMER, &TIM_TimeBaseStructure);

    /* IGN_TIMER PWM2 Mode configuration: Channels 1-2-3-4 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC2Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC3Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC4Init(IGN_TIMER, &TIM_OCInitStructure);

    /* One Pulse Mode selection */
    TIM_SelectOnePulseMode(IGN_TIMER, TIM_OPMode_Single);

    serDbg("startIgnition Complete\r\n");

    while (true) {

        chThdSleepMilliseconds(10);

        /* IGN_TIMER disable counter */
        IGN_TIMER->CR1 &= ~TIM_CR1_CEN;

        /* Reset timer counter */
        IGN_TIMER->CNT = 0;

        /* One Pulse mode (stops after CNT reaches ARR) */
        IGN_TIMER->CR1 |= TIM_CR1_OPM;

        /* Are we shifting a gear? */
        if (status.shifting && (settings.data.functions & SETTINGS_FUNCTION_SHIFTER))
        {
            if (settings.data.cut_type == SETTINGS_CUT_NORMAL)
            {
                pulses[0] = settings.data.cut_time;
                pulses[1] = settings.data.cut_time;
                pulses[2] = settings.data.cut_time;
                pulses[3] = settings.data.cut_time;
            }
            else if (settings.data.cut_type == SETTINGS_CUT_PROGRESSIVE)
            {
                pulses[0] = settings.data.cut_time;
                pulses[1] = settings.data.cut_time;
                pulses[2] = settings.data.cut_time;
                pulses[3] = settings.data.cut_time;
            }
        }

        /* Are we slipping? */
        else if ((status.slipping_pct > settings.data.slip_threshold) && (settings.data.functions & SETTINGS_FUNCTION_TC))
        {
            if (settings.data.cut_type == SETTINGS_CUT_NORMAL)
            {
                pulses[0] = settings.data.cut_time;
                pulses[1] = settings.data.cut_time;
                pulses[2] = settings.data.cut_time;
                pulses[3] = settings.data.cut_time;
            }
            else if (settings.data.cut_type == SETTINGS_CUT_PROGRESSIVE)
            {
                pulses[0] = settings.data.cut_time;
                pulses[1] = settings.data.cut_time;
                pulses[2] = settings.data.cut_time;
                pulses[3] = settings.data.cut_time;
            }
        }

        else continue;

        IGN_TIMER->CCR1 = IGN_TIMER_CONV(pulses[0]);
        IGN_TIMER->CCR2 = IGN_TIMER_CONV(pulses[1]);
        IGN_TIMER->CCR3 = IGN_TIMER_CONV(pulses[2]);
        IGN_TIMER->CCR4 = IGN_TIMER_CONV(pulses[3]);

        /* IGN_TIMER enable counter */
        IGN_TIMER->CR1 |= TIM_CR1_CEN;
    }
}

void doCut(uint16_t cut_time)
{
    (void)cut_time;

}
