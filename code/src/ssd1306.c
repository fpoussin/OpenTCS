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

#define CMD(c)        do { palClearPad( SSD1306_DC_PORT, SSD1306_DC_PIN); \
                           ssd1306SendByte( c ); \
                         } while (0);
#define DATA(c)       do { palSetPad( SSD1306_DC_PORT, SSD1306_DC_PIN); \
                           ssd1306SendByte( c ); \
                         } while (0);

static uint8_t buffer[SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8];

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
  while (SSD1306_SPI->SR & SPI_SR_BSY) {};
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
  spiInit(SSD1306_SPI);

  // Reset the LCD
  palClearPad(SSD1306_RST_PORT, SSD1306_RST_PIN);
  chThdSleepMilliseconds(10);
  palSetPad(SSD1306_RST_PORT, SSD1306_RST_PIN);
  chThdSleepMilliseconds(10);

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

  // Auto refresh 50ms - 20fps
  SSD1306_TIMER->CR1 = 0;
  SSD1306_TIMER->CR2 = 0;
  SSD1306_TIMER->CNT = 0;
  SSD1306_TIMER->PSC = 24000 - 1;	// Set prescaler to 24 000 (PSC + 1) (1KHz)
  SSD1306_TIMER->ARR = 50;	        // Auto reload value
}

void ssd1306TurnOn(void)
{
    // Enable the OLED panel
    CMD(SSD1306_DISPLAYON);
    SSD1306_TIMER->DIER |= TIM_DIER_UIE; // Enable update interrupt (timer level)
    NVIC_EnableIRQ(SSD1306_TIMER_IRQn); // Enable interrupt from SSD1306_TIMER (NVIC level)
    SSD1306_TIMER->CR1 |= TIM_CR1_CEN;   // Enable timer
}

void ssd1306TurnOff(void)
{
    SSD1306_TIMER->CR1 &= ~TIM_CR1_CEN;   // Disable timer
    SSD1306_TIMER->DIER &= ~TIM_DIER_UIE; // Disable update interrupt (timer level)
    NVIC_DisableIRQ(SSD1306_TIMER_IRQn); // Disable interrupt from SSD1306_TIMER (NVIC level)
    // Disable the OLED panel
    CMD(SSD1306_DISPLAYOFF);
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
    @brief Draws a line
*/
/**************************************************************************/
void ssd1306DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    uint8_t dy, dx;
    uint8_t addx, addy;
    int16_t P, diff, i;

    if (x1 >= x0) {
        dx = x1 - x0;
        addx = 1;
    } else {
        dx = x0 - x1;
        addx = -1;
    }
    if (y1 >= y0) {
        dy = y1 - y0;
        addy = 1;
    } else {
        dy = y0 - y1;
        addy = -1;
    }

    if (dx >= dy) {
        dy *= 2;
        P = dy - dx;
        diff = P - dx;

        for(i=0; i<=dx; ++i) {
            buffer[x0+ (y0/8)*SSD1306_LCDWIDTH] |= ~(1 << y0%8);
            if (P < 0) {
                P  += dy;
                x0 += addx;
            } else {
                P  += diff;
                x0 += addx;
                y0 += addy;
            }
        }
    } else {
        dx *= 2;
        P = dx - dy;
        diff = P - dy;

        for(i=0; i<=dy; ++i) {
            buffer[x0+ (y0/8)*SSD1306_LCDWIDTH] |= ~(1 << y0%8);
            if (P < 0) {
                P  += dx;
                y0 += addy;
            } else {
                P  += diff;
                x0 += addx;
                y0 += addy;
            }
        }
    }
}

/**************************************************************************/
/*!
    @brief Draws a circle
*/
/**************************************************************************/
void ssd1306DrawCircle(uint8_t x, uint8_t y, uint8_t radius) {
    int16_t a, b, P;

    a = 0;
    b = radius;
    P = 1 - radius;

    do {
        buffer[(x+a)+ ((y+b)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y+b)%8);
        buffer[(x+b)+ ((y+a)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y+a)%8);
        buffer[(x-a)+ ((y+b)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y+b)%8);
        buffer[(x-b)+ ((y+a)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y+a)%8);
        buffer[(x+b)+ ((y-a)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y-a)%8);
        buffer[(x+a)+ ((y+b)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y+b)%8);
        buffer[(x-a)+ ((y-b)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y-b)%8);
        buffer[(x-b)+ ((y-a)/8)*SSD1306_LCDWIDTH] |= ~(1 << (y-a)%8);
        if (P < 0)
            P += 3 + 2*a++;
        else
            P += 5 + 2*(a++ - b--);
    } while(a <= b);
}

/**************************************************************************/
/*!
    @brief Draws a filled rectangle
*/
/**************************************************************************/
void ssd1306FillArea(uint8_t x, uint8_t y, uint8_t cx, uint8_t cy) {
    uint8_t x0, x1, y1;

    x0 = x;
    x1 = x + cx;
    y1 = y + cy;
    for(; y < y1; y++)
        for(x = x0; x < x1; x++)
            buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= ~(1 << y%8);
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

  palSetPad( SSD1306_DC_PORT, SSD1306_DC_PIN);

  spiSendS(SSD1306_SPI, buffer, 1024);
}

/**************************************************************************/
/*!
    @brief  Draws a single graphic character using the supplied font
*/
/**************************************************************************/
void ssd1306DrawChar(uint8_t x, uint8_t y, uint8_t c, struct FONT_DEF font)
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
void ssd1306DrawString(uint8_t x, uint8_t y, char* text, struct FONT_DEF font)
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
      systicknilThdSleepMilliseconds(1000);
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

void SSD1306_TIMER_IRQHandler(void)
{
    if(SSD1306_TIMER->SR & TIM_SR_UIF) // if UIF flag is set
    {
        SSD1306_TIMER->SR &= ~TIM_SR_UIF; // clear UIF flag

        CMD(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
        CMD(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
        CMD(SSD1306_SETSTARTLINE | 0x0); // line #0

        palSetPad(SSD1306_DC_PORT, SSD1306_DC_PIN);

        spiSendI(SSD1306_SPI, buffer, 1024);

        palTogglePad(GPIOC, GPIOC_LED4); /* Display heartbeat */
    }
}
