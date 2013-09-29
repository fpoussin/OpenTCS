#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QObject>
#include <QtEndian>
#include <ftdi.h>
#include <QFile>

#define STM32_ACK       0x79
#define STM32_NACK      0x1F
#define STM32_CMD_INIT  0x7F
#define STM32_CMD_GET   0x00    /* get the version and command supported */
#define STM32_CMD_EE    0x44    /* extended erase */

#define STM32_FLASH_ADDR 0x80000000

class bootloader : public QObject
{
    Q_OBJECT
struct {
        quint8 get;
        quint8 gvr;
        quint8 gid;
        quint8 rm;
        quint8 go;
        quint8 wm;
        quint8 er; /* this may be extended erase */
        quint8 wp;
        quint8 uw;
        quint8 rp;
        quint8 ur;
} cmd;

public:
    explicit bootloader(ftdi* device, QObject *parent = 0);
    ~bootloader();
    bool connect(void);
    bool disconnect(void);
    bool writeFile(QFile * file);

public slots:

signals:

private slots:
    bool init(void);
    bool sendCommand(quint8 cmd);
    bool unlockFlash(void);
    bool readMem(quint32 address, quint8 * data, quint32 len);
    bool writeMem(quint32 address, quint8 * data, quint32 len);
    bool eraseMem(void);
    quint8 calcChecksum(const quint32 val);

private:
    ftdi*           ftdi_device;
    quint8          rx_buff[1024];
    quint8          tx_buff[1024];
    quint8          bl_version;

};

#endif // BOOTLOADER_H
