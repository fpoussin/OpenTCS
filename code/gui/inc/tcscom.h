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

#define SETTINGS_FUNCTION_TC 0x1
#define SETTINGS_FUNCTION_SHIFTER 0x2
#define SETTINGS_FUNCTION_LED 0x4

#define SETTINGS_CUT_DISABLED 0x0
#define SETTINGS_CUT_NORMAL 0x1
#define SETTINGS_CUT_PROGRESSIVE 0x2

#define SETTINGS_SENSOR_NORMAL 0
#define SETTINGS_SENSOR_REVERSE 1

class tcscom : public QObject
{
    Q_OBJECT
public:
    explicit tcscom(ftdi* device, QObject *parent = 0);
    ~tcscom();
    
signals:
    
public slots:
    bool setSettings(const settings_t* settings);
    bool getSettings(settings_t* settings);

    bool getInfo(status_t* status);
    bool getDiag(sensors_t* sensors);

private slots:
    quint8 doChecksum(quint8 * buf, quint8 len);

private:
    ftdi*        ftdi_device;
    quint8       pb_obuffer[256];
    quint8       pb_ibuffer[256];
    pb_ostream_t pb_ostream;
    pb_istream_t pb_istream;
};

#endif // TCSCOM_H
