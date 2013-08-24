#include "nil.h"
#include "threads.h"

/*
 * Function prototypes.
 */

light_settings_t light_settings = {LIGHT_STATE_OFF, 0};
void updateLight(light_settings_t s);

/*
 * Actual functions.
 */

void startLight(void)
{
    while (1)
    {
        updateLight(light_settings);

        gpioTogglePad(GPIOC, GPIOC_LED4);
        nilThdSleepMilliseconds(500);
    }
}

void updateLight(light_settings_t s)
{
    switch (s.state) {
        case LIGHT_STATE_OFF:
            // Turn light off
            break;

        case LIGHT_STATE_STILL:
            // Turn light on
            break;

        default:
            // Turn light off
            break;
    }
}
