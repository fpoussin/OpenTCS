#ifndef TCSCOM_H
#define TCSCOM_H

#include <QObject>
#include "bootloader.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "messages.pb.h"

#define SER_MAGIC1 0xAB
#define SER_MAGIC2 0xCD

#define CMD_OFFSET_CMD 2
#define CMD_OFFSET_LEN 3

#define CMD_SEND_DIAG 0x01
#define CMD_SEND_INFO 0x02
#define CMD_SEND_SETTINGS 0x03
#define CMD_SAVE_SETTINGS 0x04

class tcscom : public QObject
{
    Q_OBJECT
public:
    explicit tcscom(QObject *parent = 0);
    
signals:
    
public slots:
    bool connect(void);
    bool disconnect(void);

    void setSettings(void);
    void getSettings(void);

    void getInfo(void);
    void getDiag(void);

private slots:
    quint8 doChecksum(quint8 * buf, quint8 len);

private:
    ftdi ftdi_device;
    settings_t settings;
    status_t status;
    sensors_t sensors;
    light_settings_t light_settings;
    quint8 pb_obuffer[256];
    quint8 pb_ibuffer[256];
    pb_ostream_t pb_ostream;
    pb_istream_t pb_istream;
};

#endif // TCSCOM_H
