#include "threads.h"

/*
 * Function prototypes.
 */

light_settings_t light_settings = {LIGHT_STATE_OFF, 0};
void updateLight(light_settings_t s);

static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  TIM_OCInitStructure;

/*
 * Actual functions.
 */

void startLight(void)
{
    uint16_t PrescalerValue;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((STM32_PCLK ) / 24000000) - 1; // 1KHz

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    /* TIM17 OC1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 250; // period in ms
    TIM_OC1Init(TIM16, &TIM_OCInitStructure);

    while (1)
    {
        if (settings.data.functions & SETTINGS_FUNCTION_LED)
        {
            updateLight(light_settings);
        }
        nilThdSleepMilliseconds(50);
    }
}

void updateLight(light_settings_t s)
{
    switch (s.state) {
        case LIGHT_STATE_OFF:
            // Turn light off
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        case LIGHT_STATE_STILL:
            // Turn light on
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);;
            break;

        case LIGHT_STATE_BLINK:
            // Blink light
            TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
            TIM_OC1Init(TIM16, &TIM_OCInitStructure);
            break;

        default:
            // Turn light off
            break;
    }
}
