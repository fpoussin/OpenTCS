#include "nil.h"
#include "threads.h"

/*
 * Function prototypes.
 */

void updateLight(light_settings_t s);

/*
 * Actual functions.
 */

void startlight(void) {

    while (1) {

        updateLight(light_settings);


    }
}

void updateLight(light_settings_t s) {

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
