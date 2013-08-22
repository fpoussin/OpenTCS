/*
 * This file contains all the function prototypes
 * and global structures used in threads.
 */

/* Light */

#define LIGHT_STATE_OFF 0
#define LIGHT_STATE_STILL 1
#define LIGHT_STATE_BLINK 2
#define LIGHT_STATE_PULSE 3

void startlight(void) __attribute__ ((noreturn));
struct __light_settings {

    uint8_t state;
    uint16_t duration; /* Duration/interval in ms */
};
typedef struct __light_settings light_settings_t;
extern light_settings_t light_settings;

/* End of Light */


/* ADC */
void startAdc(void) __attribute__ ((noreturn));
/* End of ADC */


/* Ignition */

void startIgnition(void) __attribute__ ((noreturn));
struct __ign_cut {
    uint8_t type;
    uint16_t duration;
};
typedef struct __ign_cut ign_cut_t;
extern ign_cut_t ign_cut;

/* End of Ignition */


/* OLED */
void startOled(void) __attribute__ ((noreturn));
/* End of OLED */
