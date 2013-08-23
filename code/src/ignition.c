#include "nil.h"
#include "threads.h"

ign_cut_t ign_cut = {IGN_CUT_DISABLED, 0};

/*
 * Actual functions.
 */

void startIgnition(void) {

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    uint16_t PrescalerValue = 0;
    uint16_t pulses[4] = {0,0,0,0};
    settings_t* st;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((STM32_PCLK ) / 24000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM3 PWM2 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    /* One Pulse Mode selection */
    TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);

    while (1) {

        nilThdSleepMilliseconds(10);

        /* TIM3 disable counter */
        TIM_Cmd(TIM3, DISABLE);

        TIM3->CNT = 0;

        st = readSettings();

        /* Are we shifting a gear? */
        if (sensors.shifting)
        {
            pulses[0] = 0;
            pulses[1] = 0;
            pulses[2] = 0;
            pulses[3] = 0;
        }

        /* Are we slipping? */
        else if (sensors.slipping_pct > st->data.slip_threshold)
        {
            pulses[0] = 0;
            pulses[1] = 0;
            pulses[2] = 0;
            pulses[3] = 0;
        }

        else continue;

        TIM3->CCR1 = pulses[0];
        TIM3->CCR2 = pulses[1];
        TIM3->CCR3 = pulses[2];
        TIM3->CCR4 = pulses[3];

        /* TIM3 enable counter */
        TIM_Cmd(TIM3, ENABLE);
    }
}
