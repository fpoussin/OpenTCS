#include "threads.h"

#define LED_TIMER TIM16
#define LED_TIMER_PSC (STM32_PCLK/1000)  // 1Khz clock

/*
 * Function prototypes.
 */

light_settings_t light_settings = {LIGHT_STATE_OFF, 250};
void updateLight(light_settings_t* s);

TIM_OCInitTypeDef  TIM_OCInitStructure;

/*
 * Actual functions.
 */

void startLight(void)
{

    light_settings_t previous_settings = light_settings;

    LED_TIMER->CR1 = 0;
    LED_TIMER->CR2 = 0;
    LED_TIMER->ARR = 0xFFFF;
    LED_TIMER->PSC = LED_TIMER_PSC - 1;

    /* TIM16 OC1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = light_settings.duration; // period in ms
    TIM_OC1Init(LED_TIMER, &TIM_OCInitStructure);

    serDbg("startLight Complete\r\n");

    while (true)
    {
        nilThdSleepMilliseconds(75);

        if (settings.data.functions & SETTINGS_FUNCTION_LED)
        {
            /* Skip if updating is not needed */
            if (light_settings.state == previous_settings.state &&
                    light_settings.duration == previous_settings.duration)
                continue;

            previous_settings = light_settings;

            if (sensors.slipping_pct >= settings.data.slip_threshold)
            {
                light_settings.state = LIGHT_STATE_STILL;
            }
            else if (sensors.shifting)
            {
                light_settings.state = LIGHT_STATE_PULSE;
            }
            else
            {
                light_settings.state = LIGHT_STATE_OFF;
            }

            updateLight(&light_settings);
        }
    }
}

void updateLight(light_settings_t *s)
{

    /* Disable Timer */
    LED_TIMER->CR1 &= ~TIM_CR1_CEN;

    /* Disable previous output mode */
    LED_TIMER->CCMR1 &= ~TIM_CCMR1_OC1M;

    /* Disable one pulse mode */
    LED_TIMER->CR1 &= ~TIM_CR1_OPM;

    switch (s->state) {
        case LIGHT_STATE_OFF:
            /* Turn light off */
            LED_TIMER->CCMR1 |= TIM_ForcedAction_InActive;
            break;

        case LIGHT_STATE_STILL:
            /* Turn light on */
            LED_TIMER->CCMR1 |= TIM_ForcedAction_Active;
            break;

        case LIGHT_STATE_BLINK:
            /* Blink light */
            LED_TIMER->CCMR1 |= TIM_OCMode_Toggle;
            LED_TIMER->CCR1 = s->duration; // period in ms
            break;

        case LIGHT_STATE_PULSE:
            /* Blink light */
            LED_TIMER->CCMR1 |= TIM_OCMode_Toggle;
            LED_TIMER->CCR1 = s->duration; // period in ms
            LED_TIMER->CR1 |= TIM_CR1_OPM;
            break;

        default:
            /* Turn light off */
            LED_TIMER->CCMR1 |= TIM_ForcedAction_InActive;
            break;
    }
    /* Enable Timer */
    LED_TIMER->CR1 |= TIM_CR1_CEN;
}
