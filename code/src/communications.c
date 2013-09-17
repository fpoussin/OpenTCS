#include "threads.h"
#include "board.h"
#include <string.h> // memcpy

#define I2C_TIMEOUT 100 /* ms */
#define SPI_TIMEOUT 100 /* ms */
#define USART_TIMEOUT 100 /* ms */

#define USART_TXBUF_SIZE 48
#define USART_RXBUF_SIZE 32

#define DMA_REMAP_USART TRUE
#define DMA_CHANNEL_USART1_TX DMA1_Channel4
#define DMA_CTCIF_USART1_TX DMA_IFCR_CTCIF4
#define DMA_TCIF_USART1_TX DMA_ISR_TCIF4

#define DMA_CHANNEL_USART1_RX DMA1_Channel5
#define DMA_CTCIF_USART1_RX DMA_IFCR_CTCIF5
#define DMA_TCIF_USART1_RX DMA_ISR_TCIF5

#define DMA_CHANNEL_SPI1_TX DMA1_Channel3
#define DMA_CTCIF_SPI1_TX DMA_IFCR_CTCIF3
#define DMA_TCIF_SPI1_TX DMA_ISR_TCIF3

#define DMA_CHANNEL_SPI1_RX DMA1_Channel2
#define DMA_CTCIF_SPI1_RX DMA_IFCR_CTCIF2
#define DMA_TCIF_SPI1_RX DMA_ISR_TCIF2

semaphore_t usart1_semI, usart1_semS;
semaphore_t spi1_semI, spi1_semS;
semaphore_t i2c1_semI, i2c1_semS;

char usart_txbuf[USART_TXBUF_SIZE];
char usart_rxbuf[USART_RXBUF_SIZE];

void i2cInit(I2C_TypeDef* I2Cx)
{
    chSemObjectInit(&i2c1_semI, 1);
    chSemObjectInit(&i2c1_semS, 1);

    I2C_InitTypeDef I2C_InitStructure;

    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Disable;
    I2C_InitStructure.I2C_DigitalFilter = 0x00;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_Timing = 0x0070D8FF;
    I2C_Init(I2Cx, &I2C_InitStructure);

    I2C_Cmd(I2Cx, ENABLE);
}

uint8_t i2cSendS(I2C_TypeDef* I2Cx, const uint8_t addr, uint8_t* buffer, uint8_t len)
{
    uint8_t timeout;

    if (chSemWaitTimeout(&i2c1_semS, MS2ST(I2C_TIMEOUT)) != MSG_OK)
    {
        return 1;
    }
    I2C_TransferHandling(I2Cx, addr, len, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    while (len--)
    {
        timeout = 100;
        I2Cx->TXDR = *buffer++;
        while(I2C_GetFlagStatus(I2Cx, I2C_ISR_TC) == RESET)
        {
            chThdSleepMicroseconds(20);
            if (!timeout--)
            {
                chSemSignal(&i2c1_semS);
                return 1;
            }
        };
    }
    chSemSignal(&i2c1_semS);
    return 0;
}

uint8_t i2cReceiveS(I2C_TypeDef* I2Cx, const uint8_t addr, uint8_t *buffer, uint8_t len)
{
    uint8_t timeout;

    if (chSemWaitTimeout(&i2c1_semS, MS2ST(I2C_TIMEOUT)) != MSG_OK)
    {
        return 1;
    }
    I2C_TransferHandling(I2Cx, addr, len, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    while (len--)
    {
        timeout = 100;
        while(I2C_GetFlagStatus(I2Cx, I2C_ISR_RXNE) == RESET)
        {
            chThdSleepMicroseconds(50);
            if (!timeout--)
            {
                chSemSignal(&i2c1_semS);
                return 1;
            }
        };
        *buffer++ = I2Cx->RXDR;
    }
    chSemSignal(&i2c1_semS);
    return 0;
}

void usartInit(USART_TypeDef* USARTx)
{
#if defined(DMA_REMAP_USART) && DMA_REMAP_USART
    SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Tx, ENABLE);
    SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART1Rx, ENABLE);
#endif

    chSemObjectInit(&usart1_semI, 1);
    chSemObjectInit(&usart1_semS, 1);

    memset(usart_txbuf, 0, sizeof(usart_txbuf));
    memset(usart_rxbuf, 0, sizeof(usart_rxbuf));

    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);

    /* DMA channel Tx of USART Configuration */
    DMA_DeInit(DMA_CHANNEL_USART1_TX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->TDR;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart_txbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHANNEL_USART1_TX, &DMA_InitStructure);

    /* Enable the USART Tx DMA request */
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

    /* DMA channel Rx of USART Configuration */
    DMA_DeInit(DMA_CHANNEL_USART1_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->RDR;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(usart_rxbuf);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)usart_rxbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHANNEL_USART1_RX, &DMA_InitStructure);

    /* Enable the USART Rx DMA request */
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);

    /* Enable the DMA channel */
    DMA_Cmd(DMA_CHANNEL_USART1_RX, ENABLE);

    USART_Cmd(USARTx, ENABLE);
}

uint8_t usartSendI(USART_TypeDef* USARTx, const char* buffer, uint16_t len)
{
    uint8_t ret = 1;
    if (USARTx == USART1)
    {
        if (chSemWaitTimeout(&usart1_semI, TIME_IMMEDIATE) != MSG_OK)
        {
            return 1;
        }
        if (len > USART_TXBUF_SIZE)
        {
            len = USART_TXBUF_SIZE;
        }

        /* Copy data to usart tx buffer */
        memcpy(usart_txbuf, buffer, len);

        /* Stop and re-initialise DMA1
         * We don't care if it was running,
         * we'll restart from the beginning of the buffer
         */
        DMA_CHANNEL_USART1_TX->CCR &= ~DMA_CCR_EN; /* Stop DMA1_Channel */
        DMA1->IFCR |= DMA_CTCIF_USART1_TX; /* Clear transfer complete flag */
        DMA_CHANNEL_USART1_TX->CMAR = (uint32_t)usart_txbuf;
        DMA_CHANNEL_USART1_TX->CNDTR = len;

        /* Start DMA1_Channel4 */
        DMA_CHANNEL_USART1_TX->CCR |= DMA_CCR_EN;

        chSemSignal(&usart1_semI);
    }
    return ret;
}

uint8_t usartSendS(USART_TypeDef* USARTx, const char* buffer, uint16_t len)
{
    uint8_t ret = 1;
    if (USARTx == USART1)
    {
        if (chSemWaitTimeout(&usart1_semS, MS2ST(USART_TIMEOUT)) != MSG_OK)
        {
            return 1;
        }

/* DMA */
//        if((DMA1->ISR & DMA_TCIF_USART1_TX) == RESET)
//        {
//            DMA1->IFCR |= DMA_CTCIF_USART1_TX; /* Clear transfer complete flag */
//        }

//        ret = usartSendI(USARTx, buffer, len);

//        /* Wait for transfer to finish  */
//        while((DMA1->ISR & DMA_TCIF_USART1_TX) == RESET) {};
//        DMA1->IFCR |= DMA_CTCIF_USART1_TX; /* Clear transfer complete flag */

/* Manual */
        while (len--) {
            USARTx->TDR = (*buffer++ & (uint16_t)0x01FF);
            while ((USARTx->ISR & USART_ISR_TXE) == RESET);
        }
        chSemSignal(&usart1_semS);
    }
    return ret;
}

void usartPrintString(USART_TypeDef* USARTx, const char* str)
{
    usartSendS(USARTx, str, strlen(str));
}

void serDbg(const char* str)
{
    usartPrintString(DBG_USART, str);
}

void spiInit(SPI_TypeDef* SPIx)
{
    SPI_InitTypeDef SPI_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_Channel_TypeDef* DMA_Ch;

    if (SPIx == SPI1)
    {
        chSemObjectInit(&spi1_semI, 1);
        chSemObjectInit(&spi1_semS, 1);
        DMA_Ch = DMA_CHANNEL_SPI1_TX;
    }
    else
    {
        return;
    }

    SPI_Cmd(SPIx, DISABLE);

    //configure spi
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIx, &SPI_InitStructure);
    SPI_Cmd(SPIx, ENABLE);

    /* Setup DMA */
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPIx->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;
    DMA_InitStructure.DMA_BufferSize = 0;
    /* write */
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_Init(DMA_Ch, &DMA_InitStructure);

    /* Enable the SPI Tx DMA request */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
}

uint8_t spiSendI(SPI_TypeDef* SPIx, uint8_t* buffer, uint16_t len)
{
    if (SPIx == SPI1)
    {
        if (chSemWaitTimeout(&spi1_semI, TIME_IMMEDIATE) != MSG_OK)
        {
            return 1;
        }
        /* Stop and re-initialise DMA1
         * We don't care if it was running,
         * we'll restart from the beginning of the buffer
         */
        DMA_CHANNEL_SPI1_TX->CCR &= ~DMA_CCR_EN; /* Stop DMA1_Channel3 */
        DMA1->IFCR |= DMA_CTCIF_SPI1_TX; /* Clear transfer complete flag */
        DMA_CHANNEL_SPI1_TX->CMAR = (uint32_t)buffer;
        DMA_CHANNEL_SPI1_TX->CNDTR = len;

        /* Start DMA1_Channel3 */
        DMA_CHANNEL_SPI1_TX->CCR |= DMA_CCR_EN;
        chSemSignal(&spi1_semI);

        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t spiSendS(SPI_TypeDef* SPIx, uint8_t* buffer, uint16_t len)
{
    uint8_t ret = 1;
    if (SPIx == SPI1)
    {
        if (chSemWaitTimeout(&spi1_semS, MS2ST(SPI_TIMEOUT)) != MSG_OK)
        {
            return 1;
        }

        if((DMA1->ISR & DMA_TCIF_SPI1_TX) == RESET)
        {
            DMA1->IFCR |= DMA_CTCIF_SPI1_TX; /* Clear transfer complete flag */
        }
        ret = spiSendI(SPIx, buffer, len);

        /* Wait for transfer to finish  */
        while((DMA1->ISR & DMA_TCIF_SPI1_TX) == RESET) chThdSleepMilliseconds(10);
        DMA1->IFCR |= DMA_CTCIF_SPI1_TX; /* Clear transfer complete flag */
        chSemSignal(&spi1_semS);
    }
    return ret;
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
