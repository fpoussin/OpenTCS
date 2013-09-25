#include "tcscom.h"

tcscom::tcscom(QObject *parent) :
    QObject(parent)
{
}

void tcscom::connect()
{
    this->ftdi_device.connect();
}

void tcscom::disconnect()
{
    this->ftdi_device.disconnect();
}

void tcscom::setSettings()
{
    quint8 cmd[4] = {SER_MAGIC1, SER_MAGIC2, CMD_SAVE_SETTINGS, sizeof(settings_t)+sizeof(cmd)+1};
    quint8 cs;

    cs = doChecksum(cmd, sizeof(cmd)) + doChecksum((quint8*)&settings, sizeof(settings_t));

    this->ftdi_device.write(cmd, sizeof(cmd));
    this->ftdi_device.write((quint8*)&settings, sizeof(settings_t));
    this->ftdi_device.write(&cs);
}

void tcscom::getSettings()
{
    settings_t settings_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read((quint8*)&settings_tmp, sizeof(settings_t));
    this->ftdi_device.read(&cs);

    settings = settings_tmp;
}

void tcscom::getInfo()
{
    status_t status_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read((quint8*)&status_tmp, sizeof(status_t));
    this->ftdi_device.read(&cs);

    status = status_tmp;
}

void tcscom::getDiag()
{
    sensors_t sensors_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read((quint8*)&sensors_tmp, sizeof(sensors_t));
    this->ftdi_device.read(&cs);

    sensors = sensors_tmp;
}

quint8 tcscom::doChecksum(quint8 *buf, quint8 len)
{
    quint8 sum = 0;

    while (len--)
        sum += *buf++;

    return sum;
}
