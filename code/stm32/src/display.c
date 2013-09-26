#include "threads.h"
#include "ssd1306.h"
#include "menu.h"
#include "string.h"

display_t display = {DISPLAY_OFF};
const char version[] = VERSION;
const char enabled[] = "Enabled";
const char disabled[] = "Disabled";

void showDiag(void);
void showInfo(void);
void setCutMode(void);
void setShifter(void);
void setGears(void);

void toggleLED(void);
void toggleTC(void);
void toggleShifter(void);

/* Functions menu  */
const menuItem_t functionsMenuList[] = {{"Shift light", &toggleLED, NULL},
                                 {"Traction Control", &toggleTC, NULL},
                                 {"Shifter", &toggleShifter, NULL}};

const menuStruct_t functionsMenu = {"Toggle Functions", sizeof(functionsMenuList)/sizeof(menuItem_t), functionsMenuList};
/**/

/* Settings menu */
const menuItem_t settingsMenuList[] = {{"Functions", NULL, &functionsMenu},
                                 {"Cut mode", &setCutMode, NULL},
                                 {"Shifter sensor setup", &setShifter, NULL},
                                 {"Gears learn", &setGears, NULL} };

const menuStruct_t settingsMenu = {"Settings Menu", sizeof(settingsMenuList)/sizeof(menuItem_t), settingsMenuList};
/**/

/* Main menu */
const menuItem_t mainMenuItemList[] = {{"Settings", NULL, &settingsMenu},
                                 {"Diagnostics", &showDiag, NULL},
                                 {"Informations", &showInfo, NULL}};

const menuStruct_t mainMenu = {"Main Menu", sizeof(mainMenuItemList)/sizeof(menuItem_t), mainMenuItemList};
/**/

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

void drawTitle(char * str)
{
    const uint8_t len = Font_System7x8.u8Width*strlen(str);
    ssd1306DrawString(64-(len/2), 0, str, Font_System7x8);
}

void showDiag(void)
{
    char str[10] = "";

    while (!BUTTON_SEL) {

        drawTitle("Diagnostics");

        ssd1306DrawString(0, 10, "RPM:", Font_System5x8);
        itoa(sensors.rpm, str);
        ssd1306DrawString(25, 10, str, Font_System5x8);

        ssd1306DrawString(0, 20, "Speed:", Font_System5x8);
        itoa(sensors.speed, str);
        ssd1306DrawString(35, 20, str, Font_System5x8);

        ssd1306DrawString(0, 30, "Shifter:", Font_System5x8);
        itoa(sensors.strain_gauge, str);
        ssd1306DrawString(45, 30, str, Font_System5x8);

        ssd1306DrawString(0, 40, "TC Switch:", Font_System5x8);
        itoa(sensors.tc_switch, str);
        ssd1306DrawString(55, 40, str, Font_System5x8);

        ssd1306DrawString(0, 50, "VBAT:", Font_System5x8);
        itoa(sensors.tc_switch, str);
        ssd1306DrawString(30, 50, str, Font_System5x8);

        chThdSleepMilliseconds(100);
    }
}

void showInfo(void)
{
    drawTitle("Version");
    ssd1306DrawString(0, 10, version, Font_System5x8);

    ssd1306DrawString(0, 20, "Cut mode:", Font_System5x8);
    if (settings.data.functions == SETTINGS_CUT_PROGRESSIVE)
    {
        ssd1306DrawString(25, 20, "Progressive", Font_System5x8);
    }
    else if (settings.data.functions == SETTINGS_CUT_NORMAL)
    {
        ssd1306DrawString(25, 20, "Normal", Font_System5x8);
    }
    else
    {
        ssd1306DrawString(25, 20, disabled, Font_System5x8);
    }

    ssd1306DrawString(0, 30, "Shifter:", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_SHIFTER)
    {
        ssd1306DrawString(35, 30, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(35, 30, enabled, Font_System5x8);
    }

    ssd1306DrawString(0, 40, "TC:", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_TC)
    {
        ssd1306DrawString(20, 40, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(20, 40, enabled, Font_System5x8);
    }

    ssd1306DrawString(0, 50, "Shift Light:", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_LED)
    {
        ssd1306DrawString(50, 50, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(50, 50, enabled, Font_System5x8);
    }

    while (!BUTTON_SEL) chThdSleepMilliseconds(100);
}

void setCutMode(void)
{
    drawTitle("Cut Mode");

    ssd1306DrawString(10, 0, "Shift Light", Font_System5x8);
    if (settings.data.functions == SETTINGS_CUT_NORMAL)
    {
        ssd1306DrawString(25, 0, "Progressive", Font_System5x8);
        settings.data.functions = SETTINGS_CUT_PROGRESSIVE;
    }
    else
    {
        ssd1306DrawString(25, 0, "Normal", Font_System5x8);
        settings.data.functions = SETTINGS_CUT_NORMAL;
    }
    writeSettings(&settings);
    chThdSleepMilliseconds(2000);
}

void setShifter(void)
{
    drawTitle("Shifter Sensor Setup");

    uint8_t i;
    uint16_t peak;

    ssd1306DrawString(0, 25, "Shift a gear", Font_System5x8);
    ssd1306DrawString(10, 10, "to detect direction", Font_System5x8);

    chThdSleepMilliseconds(2000);

    peak = 0;
    for (i=0; i<40; i++)
    {
        if (sensors.strain_gauge >= peak)
            peak = sensors.strain_gauge;

        chThdSleepMilliseconds(100);
    }

    ssd1306ClearScreen();

    /* Peak >= 1.65v */
    if (peak >= 0x8000)
    {
        ssd1306DrawString(0, 10, "Normal direction", Font_System5x8);
        settings.data.sensor_direction = SETTINGS_SENSOR_NORMAL;
    }
    else
    {
        ssd1306DrawString(0, 5, "Reverse direction", Font_System5x8);
        settings.data.sensor_direction = SETTINGS_SENSOR_REVERSE;
    }

    settings.data.sensor_threshold = peak;
    writeSettings(&settings);

    chThdSleepMilliseconds(2000);

    while (!BUTTON_SEL) chThdSleepMilliseconds(100);
}

void setGears(void)
{
    uint8_t i, j;
    uint16_t last_ratio, cur_ratio, speed;
    char str[2];

    cur_ratio = 0, last_ratio = 0;

    for (i=0; i<6; i++)
    {
        ssd1306ClearScreen();
        drawTitle("Gears Learning");
        itoa(i, str);
        ssd1306DrawString(15, 50, "Gear", Font_System7x8);
        ssd1306DrawString(25, 60, str, Font_System7x8);

        for (j=0; j<40; j++)
        {
            speed = sensors.speed*100;
            cur_ratio = speed / sensors.rpm;

            /* Wait until we shift up */
            while (last_ratio >= cur_ratio+(cur_ratio/100))
            {
                chThdSleepMilliseconds(50);
            }

            chThdSleepMilliseconds(100);
        }

        settings.data.gears_ratio[i] = cur_ratio-(cur_ratio/100);
        last_ratio = cur_ratio;
    }

    writeSettings(&settings);
}


void toggleLED(void)
{
    ssd1306DrawString(10, 0, "Shift Light", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_LED)
    {
        ssd1306DrawString(25, 0, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(25, 0, enabled, Font_System5x8);
    }
    settings.data.functions ^= SETTINGS_FUNCTION_LED;
    writeSettings(&settings);
    chThdSleepMilliseconds(2000);
}

void toggleShifter(void)
{
    ssd1306DrawString(10, 0, "Shifter", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_SHIFTER)
    {
        ssd1306DrawString(25, 0, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(25, 0, enabled, Font_System5x8);
    }
    settings.data.functions ^= SETTINGS_FUNCTION_SHIFTER;
    writeSettings(&settings);
    chThdSleepMilliseconds(2000);
}

void toggleTC(void)
{
    ssd1306DrawString(10, 0, "Traction control", Font_System5x8);
    if (settings.data.functions & SETTINGS_FUNCTION_TC)
    {
        ssd1306DrawString(25, 0, disabled, Font_System5x8);
    }
    else
    {
        ssd1306DrawString(25, 0, enabled, Font_System5x8);
    }
    settings.data.functions ^= SETTINGS_FUNCTION_TC;
    writeSettings(&settings);
    chThdSleepMilliseconds(2000);
}
