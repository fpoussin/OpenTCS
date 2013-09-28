#include "tcscom.h"

tcscom::tcscom(QObject *parent) :
    QObject(parent)
{

    pb_istream = pb_istream_from_buffer(pb_ibuffer, sizeof(pb_ibuffer));
    pb_ostream = pb_ostream_from_buffer(pb_obuffer, sizeof(pb_obuffer));
}

bool tcscom::connect()
{
    return this->ftdi_device.connect();
}

bool tcscom::disconnect()
{
    return this->ftdi_device.disconnect();
}

void tcscom::setSettings()
{
    quint8 cmd[4] = {SER_MAGIC1, SER_MAGIC2, CMD_SAVE_SETTINGS, 0};
    quint8 cs;

    pb_encode(&pb_ostream, settings_t_fields, &settings);
    quint8 len = pb_ostream.bytes_written;
    cmd[3] = len+4;
    cs = doChecksum(cmd, sizeof(cmd));
    cs += doChecksum((quint8*)&pb_obuffer, len);

    this->ftdi_device.write(cmd, sizeof(cmd));
    this->ftdi_device.write((quint8*)&pb_obuffer, len);
    this->ftdi_device.write(&cs);
}

void tcscom::getSettings()
{
    settings_t settings_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 info[4];
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read(info, sizeof(info));
    quint8 len = info[3]-4;

    this->ftdi_device.read((quint8*)&pb_ibuffer, len);
    this->ftdi_device.read(&cs);

    pb_decode(&pb_istream, settings_t_fields, &settings_tmp);

    settings = settings_tmp;
}

void tcscom::getInfo()
{
    status_t status_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 info[4];
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read(info, sizeof(info));
    quint8 len = info[3]-4;

    this->ftdi_device.read((quint8*)&pb_ibuffer, len);
    this->ftdi_device.read(&cs);

    pb_decode(&pb_istream, status_t_fields, &status_tmp);

    status = status_tmp;
}

void tcscom::getDiag()
{
    sensors_t sensors_tmp;
    quint8 cmd[5] = {SER_MAGIC1, SER_MAGIC2, CMD_SEND_SETTINGS, sizeof(cmd), 0};
    quint8 info[4];
    quint8 cs;

    cmd[4] = doChecksum(cmd, sizeof(cmd));
    this->ftdi_device.write(cmd, sizeof(cmd));

    this->ftdi_device.read(info, sizeof(info));
    quint8 len = info[3]-4;

    this->ftdi_device.read((quint8*)&pb_ibuffer, len);
    this->ftdi_device.read(&cs);

    pb_decode(&pb_istream, sensors_t_fields, &sensors_tmp);

    sensors = sensors_tmp;
}

quint8 tcscom::doChecksum(quint8 *buf, quint8 len)
{
    quint8 sum = 0;

    while (len--)
        sum += *buf++;

    return sum;
}
