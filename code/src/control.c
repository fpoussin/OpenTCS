#include "threads.h"

status_t status = {0, 0, 0, 0};

void checkStrainGauge(void);
void checkSpeed(void);
void checkSwitch(void);

void startControl(void)
{
    while (true)
    {



        nilThdSleepMicroseconds(50);
    }
}
