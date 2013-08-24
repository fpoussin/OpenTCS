#include "nil.h"
#include "threads.h"
#include "ssd1306.h"
#include "menu.h"

display_t display = {DISPLAY_OFF};

void showDiag(void);
void showSettings(void);

menuItem_t mainMenuItemList[] = {{"Diagnostics", &showDiag, NULL}, {"Settings", &showSettings, NULL}};

menuStruct_t mainMenu = {"Main Menu", sizeof(mainMenuItemList)/sizeof(menuItem_t), mainMenuItemList};

void startDisplay(void) {

    ssd1306Init(SSD1306_SWITCHCAPVCC);
    ssd1306TurnOn();
    display.state = DISPLAY_ON;

    ssd1306DrawString(40, 20, "OpenTCS", Font_System7x8);
    nilThdSleepMilliseconds(2000);

    while (1) {

        ssd1306TurnOff();
        display.state = DISPLAY_OFF;

        while (!gpioReadPad(GPIOC, GPIOC_BUTTON_SEL))
        {
            nilThdSleepMilliseconds(250);
        }
        ssd1306ClearScreen();
        ssd1306TurnOn();
        display.state = DISPLAY_ON;
        openMenu(&mainMenu);
    }
}

void showDiag(void) {

}

void showSettings(void) {

}
