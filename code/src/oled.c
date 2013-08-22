#include "nil.h"
#include "ssd1306.h"


void startOled(void) {

    ssd1306Init(SSD1306_SWITCHCAPVCC);

    while (1) {

        ssd1306Refresh();


    }


}
