#include "threads.h"

#define FLASH_PAGE_SIZE         (0x00000400) /* FLASH Page Size */
#define SETTINGS_PAGE           (31) /* Page where the settings are located, starting from 0  */
#define SETTINGS_ADDRESS        (FLASH_BASE+(FLASH_PAGE_SIZE*SETTINGS_PAGE)) /* 0x8003C00 */
#define FLASH_USER_START_ADDR   (SETTINGS_ADDRESS) /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (SETTINGS_ADDRESS+FLASH_PAGE_SIZE) /* End @ of user Flash area */

settings_t settings = { {0, 0, 0, 0, 0, 0, 0, 0, {0}, {0}}, 0};
const settings_t default_settings = {
    {SETTINGS_FUNCTION_SHIFTER | SETTINGS_FUNCTION_LED,
     SETTINGS_CUT_DISABLED, /* Cut type */
     50, /* Shfifter Sensor threshold  */
     10, /* TC Slip threshold  */
     30, /* Sensor gain */
     SETTINGS_SENSOR_NORMAL, /* Sensor direction */
     30, /* Min Speed */
     3000, /* Min RPM */
     {0},
     {1000,600,400,300,300,300} /* cut time by gear in ms x10 */
    },
    0}; /* CRC */

void settingsInit()
{
    settings = readSettings();
}

settings_t readSettings(void)
{
    settings_t tmp_st;
    const settings_t* const st = (settings_t*)SETTINGS_ADDRESS;

    const uint32_t CRCValue = CRC_CalcBlockCRC((uint32_t *)&st->data, sizeof(st->data));

    tmp_st = *st; /* Copy struct from flash to ram */

    /* If CRC fails to match, assign default settings */
    if (CRCValue != st->CRCValue){

        return default_settings;
    }
    return tmp_st;
}

uint8_t writeSettings(settings_t *st)
{
    uint32_t Address = 0x00;
    uint32_t* tmp_data;
    uint8_t i = 0;

    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

    /* Erase the FLASH pages */
    if (FLASH_ErasePage(FLASH_USER_START_ADDR)!= FLASH_COMPLETE)
    {
    /* Error occurred while sector erase.
       User can add here some code to deal with this error  */
      FLASH_Lock();
      return 1;
    }

    st->CRCValue = CRC_CalcBlockCRC((uint32_t *)&st->data, sizeof(st->data));

    while (Address < (FLASH_USER_START_ADDR+sizeof(st)))
    {
      tmp_data = (uint32_t*)st+i;
      if (FLASH_ProgramWord(Address, *tmp_data) == FLASH_COMPLETE)
      {
        Address = Address + 4;
        i += 4;
      }
      else
      {
        /* Error occurred while writing data in Flash memory.
           User can add here some code to deal with this error */
          FLASH_Lock();
          return 1;
      }
    }

    FLASH_Lock();
    return 0;
}
