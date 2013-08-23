#include "nil.h"
#include "threads.h"
#include "ssd1306.h"

void startDisplay(void) {

    ssd1306Init(SSD1306_SWITCHCAPVCC);

    while (1) {

        ssd1306DrawString(1, 10, "5x8 System", Font_System5x8);
        ssd1306DrawString(1, 20, "7x8 System", Font_System7x8);
        ssd1306Refresh();

        nilThdSleepMilliseconds(250);

        ssd1306ClearScreen();
        ssd1306Refresh();

        nilThdSleepMilliseconds(250);
    }
}
