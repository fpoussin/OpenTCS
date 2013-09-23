#include "threads.h"

#define IGN_TIMER TIM3
#define IGN_TIMER_IRQn TIM3_IRQn
#define IGN_TIMER_IRQHandler TIM3_IRQHandler
#define IGN_TIMER_PSC 48 /* 500KHz (24Mhz/48), max time 131ms */
#define IGN_TIMER_ARR 0xFFFF
/* Convert x from ms to us, divide by timer clock period in us */
#define IGN_TIMER_CONV(x) (IGN_TIMER_ARR-(((uint32_t)x*1000)/(1000000/(STM32_PCLK/IGN_TIMER_PSC))))

#define SLIP_CUT_TIME 100 /* Max: 131ms */

uint8_t cutting = false;
uint8_t cutting_count = 0;

/*
 * Actual functions.
 */

void startIgnition(void) {

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
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
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC2Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC3Init(IGN_TIMER, &TIM_OCInitStructure);
    TIM_OC4Init(IGN_TIMER, &TIM_OCInitStructure);

    /* One Pulse Mode selection */
    TIM_SelectOnePulseMode(IGN_TIMER, TIM_OPMode_Single);

    IGN_TIMER->DIER |= TIM_DIER_UIE; // Enable update interrupt (timer level)
    NVIC_EnableIRQ(IGN_TIMER_IRQn); // Enable interrupt from IGN_TIMER (NVIC level)

    serDbg("startIgnition Complete\r\n");

    while (true) {

        chThdSleepMilliseconds(20);

        /* Are we already cutting the ignition? Is it enabled? */
        if (cutting == true || settings.data.cut_type == SETTINGS_CUT_DISABLED)
        {
            continue;
        }

        /* Are we shifting a gear? */
        if (status.shifting && (settings.data.functions & SETTINGS_FUNCTION_SHIFTER))
        {
            /* Get cut time based on current gear */
            const uint16_t cut_time = getCurCutTime();

            pulses[0] = cut_time;
            pulses[1] = cut_time;
            pulses[2] = cut_time;
            pulses[3] = cut_time;

            IGN_TIMER->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);
        }

        /* Are we slipping? */
        else if ((status.slipping_pct > settings.data.slip_threshold) && (settings.data.functions & SETTINGS_FUNCTION_TC))
        {
            pulses[0] = SLIP_CUT_TIME;
            pulses[1] = SLIP_CUT_TIME;
            pulses[2] = SLIP_CUT_TIME;
            pulses[3] = SLIP_CUT_TIME;

            IGN_TIMER->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

            /* This mode cuts cylinders one by one up to 4 */
            if (settings.data.cut_type == SETTINGS_CUT_PROGRESSIVE && cutting_count < 4)
            {
                cutting_count++;

                /* Disable all OC outputs */
                IGN_TIMER->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

                /* Enable OC outputs based on cylinder cut count */
                switch(cutting_count)
                {
                    case 1:
                        IGN_TIMER->CCER |= TIM_CCER_CC1E;
                        break;
                    case 2:
                        IGN_TIMER->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
                        break;
                    case 3:
                        IGN_TIMER->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E);
                        break;
                    case 4:
                        IGN_TIMER->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);
                        break;
                    default:
                        break;
                }
            }
        }

        else
        {
            cutting_count = 0;
            continue;
        }

        cutting = true;
        palSetPad(GPIOB, GPIOB_PIN9);

        /* Convert cut time to CCR value and assign to register */
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

void IGN_TIMER_IRQHandler(void)
{
    if(IGN_TIMER->SR & TIM_SR_UIF) // if UIF flag is set
    {
        IGN_TIMER->SR &= ~TIM_SR_UIF; // clear UIF flag

        /* Disable all OC outputs */
        IGN_TIMER->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

        cutting = false;

        palClearPad(GPIOB, GPIOB_PIN9);
    }
}
