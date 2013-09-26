#include "threads.h"
#include "string.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"


#define GUI_USART USART1

#define SER_MAGIC1 0xAB
#define SER_MAGIC2 0xCD

#define CMD_OFFSET_CMD 2
#define CMD_OFFSET_LEN 3

#define CMD_SEND_DIAG 0x01
#define CMD_SEND_INFO 0x02
#define CMD_SEND_SETTINGS 0x03
#define CMD_SAVE_SETTINGS 0x04

uint8_t sendToGUI(const char *data, uint16_t len);
uint8_t processCmd(uint8_t cmd, uint8_t len);
int8_t searchBuffer(void);
uint8_t doChecksum(const char * buf, uint8_t len);

int8_t cmd_pos;
uint8_t update = 0;
uint8_t pb_buffer[settings_t_size+4]; /* Set to max structure size */

void startSerialCom(void)
{
    uint8_t cmd, checksum, len;

    while (true)
    {
        chThdSleepMilliseconds(50);

        if (!update) continue;

        if ((cmd_pos = searchBuffer()) >= 0)
        {
            cmd = usart_rxbuf[cmd_pos+CMD_OFFSET_CMD]; /* Command to process */
            len = usart_rxbuf[cmd_pos+CMD_OFFSET_LEN]; /* Total length */
            checksum  = usart_rxbuf[cmd_pos+len]; /* CS is at the end */

            /* Verify checksum and restart loop if fails
             * maybe some data is still missing? */
            if (checksum != doChecksum(&usart_rxbuf[cmd_pos], len))
                continue;

            /* We got the data and it is valid */
            update = 0;

            processCmd(cmd, len);

            /* Clear buffer */
            memset(usart_rxbuf, 0, USART_RXBUF_SIZE);
        }
    }
}

uint8_t doChecksum(const char * buf, uint8_t len)
{
    uint8_t sum = 0;

    while (len--)
        sum += *buf++;

    return sum;
}

int8_t searchBuffer(void)
{
    uint8_t i;
    for (i = 0; i < USART_RXBUF_SIZE-1; i++)
    {
        if (usart_rxbuf[i] == SER_MAGIC1
                && usart_rxbuf[i+1] == SER_MAGIC2)
            return i;
    }

    return -1;
}

void sendDiag(void)
{
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));

    pb_encode(&stream, sensors_t_fields, &sensors);

    sendToGUI((const char*)&pb_buffer, stream.bytes_written);
}

void sendInfo(void)
{
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));

    pb_encode(&stream, status_t_fields, &status);

    sendToGUI((const char*)&pb_buffer, stream.bytes_written);
}

void sendSettings(void)
{
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));

    pb_encode(&stream, settings_t_fields, &settings);

    sendToGUI((const char*)&pb_buffer, stream.bytes_written);
}

void saveSettings(uint8_t len)
{
    pb_istream_t stream = pb_istream_from_buffer((uint8_t*)&usart_rxbuf[cmd_pos+CMD_OFFSET_LEN+1], len-4);

    pb_decode(&stream, settings_t_fields, &settings);

    writeSettings(&settings);
}

uint8_t sendToGUI(const char* data, uint16_t len)
{
    usartSendI(GUI_USART, data, len);

    return 0;
}

uint8_t processCmd(uint8_t cmd, uint8_t len)
{
    switch (cmd)
    {
        case CMD_SEND_DIAG:
            sendDiag();
            break;
        case CMD_SEND_INFO:
            sendInfo();
            break;
        case CMD_SEND_SETTINGS:
            sendSettings();
            break;
        case CMD_SAVE_SETTINGS:
            saveSettings(len);
            break;
        default:
            return 1;
    }

    return 0;
}

void DMA1_Ch4_5_IRQHandler(void)
{
  /* Test on DMA Stream Transfer Complete interrupt */
  if (DMA1->ISR & DMA_ISR_TCIF5)
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA1->IFCR |= DMA_IFCR_CTCIF5;

    update = 1;
  }
}
