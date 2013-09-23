#include "threads.h"
#include "ssd1306.h"
#include "menu.h"

display_t display = {DISPLAY_OFF};
const char version[] = VERSION;

void showDiag(void);
void showInfo(void);
void setFunctions(void);
void setCutMode(void);
void setShifter(void);
void setGears(void);

menuItem_t settingsMenuList[] = {{"Functions", &setFunctions, NULL},
                                 {"Cut mode", &setCutMode, NULL},
                                 {"Shifter setup", &setShifter, 0},
                                 {"Gears setup", &setGears, NULL} };

menuStruct_t settingsMenu = {"Settings Menu", 2, settingsMenuList};

menuItem_t mainMenuItemList[] = {{"Diagnostics", &showDiag, NULL},
                                 {"Settings", NULL, &settingsMenu},
                                 {"Info", &showInfo, NULL}};

menuStruct_t mainMenu = {"Main Menu", sizeof(mainMenuItemList)/sizeof(menuItem_t), mainMenuItemList};

void startDisplay(void) {


    ssd1306Init(SSD1306_SWITCHCAPVCC);
    ssd1306TurnOn();
    display.state = DISPLAY_ON;
    palSetPad(GPIOC, GPIOC_LED4);

    ssd1306DrawString(40, 20, "OpenTCS", Font_System7x8);
    chThdSleepMilliseconds(100); // Fails
    palClearPad(GPIOC, GPIOC_LED4);

    serDbg("startDisplay Complete\r\n");

    while (true) {

        serDbg("Display OFF\r\n");
        ssd1306TurnOff();
        display.state = DISPLAY_OFF;

        while (!BUTTON_SEL) /* Wait until the select button is pressed */
        {
            palTogglePad(GPIOC, GPIOC_LED4); /* Display heartbeat */
            chThdSleepMilliseconds(250);
        }
        serDbg("Display ON\r\n");
        ssd1306ClearScreen();
        ssd1306TurnOn();
        display.state = DISPLAY_ON;
        openMenu(&mainMenu);
    }
}

void showDiag(void)
{

    while (!BUTTON_SEL);
}

void showInfo(void)
{
    ssd1306DrawString(20, 0, "Version:", Font_System5x8);

    ssd1306DrawString(20, 15, version, Font_System5x8);

    while (!BUTTON_SEL);
}

void setFunctions(void)
{

    while (!BUTTON_SEL);
}

void setCutMode(void)
{

    while (!BUTTON_SEL);
}

void setShifter(void)
{

    while (!BUTTON_SEL);
}

void setGears(void)
{

    while (!BUTTON_SEL);
}
