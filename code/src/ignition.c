#include "threads.h"

#define IGN_TIMER TIM3

ign_cut_t ign_cut = {IGN_CUT_DISABLED, 0};

/*
 * Actual functions.
 */

void startIgnition(void) {

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16_t pulses[4] = {0,0,0,0};

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 2400 - 1; // 10KHz (24Mhz/2400)
    TIM_TimeBaseStructure.TIM_Period = 65535; // Max 6553.6ms - 6.5536sec
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

    while (1) {

        nilThdSleepMilliseconds(10);

        /* IGN_TIMER disable counter */
        IGN_TIMER->CR1 &= ~TIM_CR1_CEN;

        /* Reset timer counter */
        IGN_TIMER->CNT = 0;

        /* Are we shifting a gear? */
        if (sensors.shifting && (settings.data.functions & SETTINGS_FUNCTION_SHIFTER))
        {
            if (settings.data.cut_type == SETTINGS_CUT_NORMAL)
            {
                pulses[0] = settings.data.cut_time*10;
                pulses[1] = settings.data.cut_time*10;
                pulses[2] = settings.data.cut_time*10;
                pulses[3] = settings.data.cut_time*10;
            }
            else if (settings.data.cut_type == SETTINGS_CUT_PROGRESSIVE)
            {
                pulses[0] = settings.data.cut_time*10;
                pulses[1] = settings.data.cut_time*10;
                pulses[2] = settings.data.cut_time*10;
                pulses[3] = settings.data.cut_time*10;
            }
        }

        /* Are we slipping? */
        else if ((sensors.slipping_pct > settings.data.slip_threshold) && (settings.data.functions & SETTINGS_FUNCTION_TC))
        {
            if (settings.data.cut_type == SETTINGS_CUT_NORMAL)
            {
                pulses[0] = settings.data.cut_time*10;
                pulses[1] = settings.data.cut_time*10;
                pulses[2] = settings.data.cut_time*10;
                pulses[3] = settings.data.cut_time*10;
            }
            else if (settings.data.cut_type == SETTINGS_CUT_PROGRESSIVE)
            {
                pulses[0] = settings.data.cut_time*10;
                pulses[1] = settings.data.cut_time*10;
                pulses[2] = settings.data.cut_time*10;
                pulses[3] = settings.data.cut_time*10;
            }
        }

        else continue;

        IGN_TIMER->CCR1 = pulses[0];
        IGN_TIMER->CCR2 = pulses[1];
        IGN_TIMER->CCR3 = pulses[2];
        IGN_TIMER->CCR4 = pulses[3];

        /* IGN_TIMER enable counter */
        IGN_TIMER->CR1 |= TIM_CR1_CEN;
    }
}
