/*
    @file     ssd1306.c
    @author   K. Townsend (microBuilder.eu)

    @section DESCRIPTION

    Driver for 128x64 OLED display based on the SSD1306 controller.

    This driver is based on the SSD1306 Library from Limor Fried
    (Adafruit Industries) at: https://github.com/adafruit/SSD1306  
    
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <string.h>
#include "ssd1306.h"
#include "stm32f0xx.h"

inline void ssd1306SendByte(uint8_t byte);

#define CMD(c)        do { gpioSetPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                           gpioClearPad( SSD1306_DC_PORT, SSD1306_DC_PIN); \
                           gpioClearPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                           ssd1306SendByte( c ); \
                           gpioSetPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                         } while (0);
#define DATA(c)       do { gpioSetPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                           gpioSetPad( SSD1306_DC_PORT, SSD1306_DC_PIN); \
                           gpioClearPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                           ssd1306SendByte( c ); \
                           gpioSetPad( SSD1306_CS_PORT, SSD1306_CS_PIN); \
                         } while (0);

#define DELAY(mS)     nilThdSleepMilliseconds(mS);

uint8_t buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];

/**************************************************************************/
/* Private Methods                                                        */
/**************************************************************************/

/**************************************************************************/
/*! 
    @brief SPI write

    @param[in]  byte
                The byte to send
*/
/**************************************************************************/
inline void ssd1306SendByte(uint8_t byte)
{
  SPI_SendData8(SSD1306_SPI, byte);
  while (SPI_I2S_GetFlagStatus(SSD1306_SPI, SPI_I2S_FLAG_BSY) == SET) {};
}

/**************************************************************************/
/*!
    @brief  Draws a single graphic character using the supplied font
*/
/**************************************************************************/
static void ssd1306DrawChar(uint16_t x, uint16_t y, uint8_t c, struct FONT_DEF font)
{
  uint8_t col, column[font.u8Width];

  // Check if the requested character is available
  if ((c >= font.u8FirstChar) && (c <= font.u8LastChar))
  {
    // Retrieve appropriate columns from font data
    for (col = 0; col < font.u8Width; col++)
    {
      column[col] = font.au8FontTable[((c - 32) * font.u8Width) + col];    // Get first column of appropriate character
    }
  }
  else
  {    
    // Requested character is not available in this font ... send a space instead
    for (col = 0; col < font.u8Width; col++)
    {
      column[col] = 0xFF;    // Send solid space
    }
  }

  // Render each column
  uint16_t xoffset, yoffset;
  for (xoffset = 0; xoffset < font.u8Width; xoffset++)
  {
    for (yoffset = 0; yoffset < (font.u8Height + 1); yoffset++)
    {
      uint8_t bit = 0x00;
      bit = (column[xoffset] << (8 - (yoffset + 1)));     // Shift current row bit left
      bit = (bit >> 7);                     // Shift current row but right (results in 0x01 for black, and 0x00 for white)
      if (bit)
      {
        ssd1306DrawPixel(x + xoffset, y + yoffset);
      }
    }
  }
}

/**************************************************************************/
/* Public Methods                                                         */
/**************************************************************************/

/**************************************************************************/
/*! 
    @brief Initialises the SSD1306 LCD display
*/
/**************************************************************************/
void ssd1306Init(uint8_t vccstate)
{

  SPI_InitTypeDef SPI_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
  SPI_Cmd(SSD1306_SPI, DISABLE);

  //configure spi
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SSD1306_SPI, &SPI_InitStructure);
  SPI_Cmd(SSD1306_SPI, ENABLE);

  /* Setup DMA */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)SPI1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
  DMA_InitStructure.DMA_BufferSize = 1024;
  /* write */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  /* Enable the SPI Rx DMA request */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

  // Reset the LCD
  gpioClearPad(SSD1306_RST_PORT, SSD1306_RST_PIN);
  DELAY(10);
  gpioSetPad(SSD1306_RST_PORT, SSD1306_RST_PIN);
  DELAY(10);

  // Initialisation sequence
  CMD(SSD1306_DISPLAYOFF);                    // 0xAE
  CMD(SSD1306_SETLOWCOLUMN | 0x0);            // low col = 0
  CMD(SSD1306_SETHIGHCOLUMN | 0x0);           // hi col = 0
  CMD(SSD1306_SETSTARTLINE | 0x0);            // line #0
  CMD(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { CMD(0x9F) }
  else
    { CMD(0xCF) }
  CMD(0xa1);                                  // setment remap 95 to 0 (?)
  CMD(SSD1306_NORMALDISPLAY);                 // 0xA6
  CMD(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  CMD(SSD1306_SETMULTIPLEX);                  // 0xA8
  CMD(0x3F);                                  // 0x3F 1/64 duty
  CMD(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  CMD(0x0);                                   // no offset
  CMD(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  CMD(0x80);                                  // the suggested ratio 0x80
  CMD(SSD1306_SETPRECHARGE);                  // 0xd9
  if (vccstate == SSD1306_EXTERNALVCC)
    { CMD(0x22) }
  else
    { CMD(0xF1) }
  CMD(SSD1306_SETCOMPINS);                    // 0xDA
  CMD(0x12);                                  // disable COM left/right remap
  CMD(SSD1306_SETVCOMDETECT);                 // 0xDB
  CMD(0x40);                                  // 0x20 is default?
  CMD(SSD1306_MEMORYMODE);                    // 0x20
  CMD(0x00);                                  // 0x0 act like ks0108
  CMD(SSD1306_SEGREMAP | 0x1);
  CMD(SSD1306_COMSCANDEC);
  CMD(SSD1306_CHARGEPUMP);                    //0x8D
  if (vccstate == SSD1306_EXTERNALVCC)
    { CMD(0x10) }
  else
    { CMD(0x14) }

  // Enabled the OLED panel
  CMD(SSD1306_DISPLAYON);
}

/**************************************************************************/
/*! 
    @brief Draws a single pixel in image buffer

    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)
*/
/**************************************************************************/
void ssd1306DrawPixel(uint8_t x, uint8_t y) 
{
  if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
    return;

  buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= (1 << y%8);
}

/**************************************************************************/
/*! 
    @brief Clears a single pixel in image buffer

    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)
*/
/**************************************************************************/
void ssd1306ClearPixel(uint8_t x, uint8_t y) 
{
  if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
    return;

  buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << y%8); 
}

/**************************************************************************/
/*! 
    @brief Gets the value (1 or 0) of the specified pixel from the buffer

    @param[in]  x
                The x position (0..127)
    @param[in]  y
                The y position (0..63)

    @return     1 if the pixel is enabled, 0 if disabled
*/
/**************************************************************************/
uint8_t ssd1306GetPixel(uint8_t x, uint8_t y)
{
  if ((x >= SSD1306_LCDWIDTH) || (y >=SSD1306_LCDHEIGHT)) return 0;
  return buffer[x+ (y/8)*SSD1306_LCDWIDTH] & (1 << y%8) ? 1 : 0;
}

/**************************************************************************/
/*! 
    @brief Clears the screen
*/
/**************************************************************************/
void ssd1306ClearScreen() 
{
  memset(buffer, 0, 1024);
}

/**************************************************************************/
/*! 
    @brief Renders the contents of the pixel buffer on the LCD
*/
/**************************************************************************/
void ssd1306Refresh(void) 
{
  CMD(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
  CMD(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
  CMD(SSD1306_SETSTARTLINE | 0x0); // line #0

  gpioSetPad( SSD1306_DC_PORT, SSD1306_DC_PIN);
  gpioClearPad( SSD1306_CS_PORT, SSD1306_CS_PIN);

  DMA_Cmd(DMA1_Channel3, DISABLE);
  DMA_ClearFlag(DMA1_FLAG_TC3);
  DMA1_Channel3->CMAR = (uint32_t)buffer;
  DMA1_Channel3->CNDTR = 1024;

  /* start */
  DMA_Cmd(DMA1_Channel3, ENABLE);

  /* Wait for transfer to finis  */
  while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) DELAY(25);
  DMA_ClearFlag(DMA1_FLAG_TC3);

  gpioSetPad( SSD1306_CS_PORT, SSD1306_CS_PIN);
}

/**************************************************************************/
/*!
    @brief  Draws a string using the supplied font data.

    @param[in]  x
                Starting x co-ordinate
    @param[in]  y
                Starting y co-ordinate
    @param[in]  text
                The string to render
    @param[in]  font
                Pointer to the FONT_DEF to use when drawing the string

    @section Example

    @code 

    #include "drivers/lcd/bitmap/ssd1306/ssd1306.h"
    #include "drivers/lcd/smallfonts.h"
    
    // Configure the pins and initialise the LCD screen
    ssd1306Init();

    // Render some text on the screen
    ssd1306DrawString(1, 10, "5x8 System", Font_System5x8);
    ssd1306DrawString(1, 20, "7x8 System", Font_System7x8);

    // Refresh the screen to see the results
    ssd1306Refresh();

    @endcode
*/
/**************************************************************************/
void ssd1306DrawString(uint16_t x, uint16_t y, char* text, struct FONT_DEF font)
{
  uint8_t l;
  for (l = 0; l < strlen(text); l++)
  {
    ssd1306DrawChar(x + (l * (font.u8Width + 1)), y, text[l], font);
  }
}

/**************************************************************************/
/*!
    @brief  Shifts the contents of the frame buffer up the specified
            number of pixels

    @param[in]  height
                The number of pixels to shift the frame buffer up, leaving
                a blank space at the bottom of the frame buffer x pixels
                high

    @section Example

    @code 

    #include "drivers/lcd/bitmap/ssd1306/ssd1306.h"
    #include "drivers/lcd/smallfonts.h"
    
    // Configure the pins and initialise the LCD screen
    ssd1306Init();

    // Enable the backlight
    ssd1306BLEnable();

    // Continually write some text, scrolling upward one line each time
    while (1)
    {
      // Shift the buffer up 8 pixels (adjust for font-height)
      ssd1306ShiftFrameBuffer(8);
      // Render some text on the screen with different fonts
      ssd1306DrawString(1, 56, "INSERT TEXT HERE", Font_System5x8);
      // Refresh the screen to see the results
      ssd1306Refresh();    
      // Wait a bit before writing the next line
      systickDelay(1000);
    }

    @endcode
*/
/**************************************************************************/
void ssd1306ShiftFrameBuffer( uint8_t height )
{
  if (height == 0) return;
  if (height >= SSD1306_LCDHEIGHT)
  {
    // Clear the entire frame buffer
    ssd1306ClearScreen();
    return;
  }

  // This is horribly inefficient, but at least easy to understand
  // In a production environment, this should be significantly optimised

  uint8_t y, x;
  for (y = 0; y < SSD1306_LCDHEIGHT; y++)
  {
    for (x = 0; x < SSD1306_LCDWIDTH; x++)
    {
      if ((SSD1306_LCDHEIGHT - 1) - y > height)
      {
        // Shift height from further ahead in the buffer
        ssd1306GetPixel(x, y + height) ? ssd1306DrawPixel(x, y) : ssd1306ClearPixel(x, y);
      }
      else
      {
        // Clear the entire line
        ssd1306ClearPixel(x, y);
      }
    }
  }
}
