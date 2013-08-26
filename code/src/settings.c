#include "threads.h"

settings_t settings = { {0, 0, 0, 0, 0, 0}, 0};

settings_t readSettings(void) {

    settings_t tmp_st;
    const settings_t* const st = (settings_t*)SETTINGS_ADDRESS;

    const uint32_t CRCValue = CRC_CalcBlockCRC((uint32_t *)&st->data, sizeof(st->data));

    tmp_st = *st; /* Copy struct from flash to ram */

    /* If CRC fails to match, assign 0 for fault detection */
    if (CRCValue != st->CRCValue) tmp_st.CRCValue = 0;

    return tmp_st;
}

uint8_t writeSettings(settings_t *st) {

    #define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
    #define FLASH_USER_START_ADDR   (SETTINGS_ADDRESS)   /* Start @ of user Flash area */
    #define FLASH_USER_END_ADDR     (SETTINGS_ADDRESS+FLASH_PAGE_SIZE)   /* End @ of user Flash area */

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
