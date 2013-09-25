#include "stm32.h"

stm32::stm32()
{

}

bool stm32::connect()
{
    this->ftdi_device.connect();

    return false;
}

bool stm32::disconnect()
{
    this->ftdi_device.disconnect();

    return false;
}

bool stm32::writeFile(QFile *file)
{
    bool res;
    file->open(QIODevice::ReadOnly);
    QByteArray data = file->readAll();
    file->close();

    this->ftdi_device.resetBootloader();

    this->unlockFlash();

    res = this->writeMem(STM32_FLASH_ADDR, (quint8*)data.data(), data.length());

    this->ftdi_device.resetNormal();

    return res;
}

bool stm32::init()
{
    quint8 len, dummy, ack;

    this->connect();

    this->sendCommand(STM32_CMD_GET);

    ftdi_device.read(&len); len++;
    ftdi_device.read(&this->bl_version); len--;
    ftdi_device.read(&cmd.get); len--;
    ftdi_device.read(&cmd.gvr); len--;
    ftdi_device.read(&cmd.gid); len--;
    ftdi_device.read(&cmd.rm); len--;
    ftdi_device.read(&cmd.go); len--;
    ftdi_device.read(&cmd.wm); len--;
    ftdi_device.read(&cmd.er); len--;
    ftdi_device.read(&cmd.wp); len--;
    ftdi_device.read(&cmd.uw); len--;
    ftdi_device.read(&cmd.rp); len--;
    ftdi_device.read(&cmd.ur); len--;

    while(len-- > 0) ftdi_device.read(&dummy);

    ftdi_device.read(&ack);

    return (ack != STM32_ACK);
}

bool stm32::sendCommand(quint8 cmd)
{
    quint8 cmd_xor = cmd^0xFF;
    quint8 ack;
    ftdi_device.write(&cmd);
    ftdi_device.write(&cmd_xor);

    ftdi_device.read(&ack);

    return (ack != STM32_ACK);
}

bool stm32::unlockFlash()
{
    this->sendCommand(cmd.uw);

    return this->sendCommand(0x8C);
}

bool stm32::readMem(quint32 address, quint8 *data, quint32 len)
{
    quint8 checksum, ack, len2;
    quint8 addr[4];

    if (address % 4 == 0)
    {
        qWarning("stm32::readMem Address is not 32b aligned");
        return true;
    }

    qToBigEndian(address, addr);
    checksum = this->calcChecksum(address);

    this->sendCommand(cmd.rm);
    ftdi_device.write(addr, 4);
    ftdi_device.write(&checksum);

    ftdi_device.read(&ack);

    len2 = len - 1;
    ftdi_device.write(&len2);
    len2 ^= 0xFF;
    ftdi_device.write(&len2);

    ftdi_device.read(&ack);

    ftdi_device.read(data, len);

    return (ack != STM32_ACK);
}

bool stm32::writeMem(quint32 address, quint8 *data, quint32 len)
{
    quint8 checksum, ack;
    quint8 addr[4];

    if (address % 4 == 0)
    {
        qWarning("stm32::writeMem Address is not 32b aligned");
        return true;
    }

    qToBigEndian(address, addr);
    checksum = this->calcChecksum(address);

    this->sendCommand(cmd.wm);
    ftdi_device.write(addr, 4);
    ftdi_device.write(&checksum);

    ftdi_device.read(&ack);

    checksum = len - 1 + (len % 4);
    ftdi_device.write(&checksum);

    /* write the data and build the checksum */
    quint32 i;
    for(i = 0; i < len; ++i)
            checksum ^= data[i];

    ftdi_device.write(data, len);
    ftdi_device.write(&checksum);
    ftdi_device.read(&ack);

    return (ack != STM32_ACK);
}

bool stm32::eraseMem()
{
    quint8 pages[3] = {0xFF, 0xFF, 0x00};
    quint8 ack;

    this->sendCommand(cmd.er);

    ftdi_device.write(pages, 3);
    ftdi_device.read(&ack);

    return (ack != STM32_ACK);
}

quint8 stm32::calcChecksum(const quint32 val)
{
    return  ((val & 0xFF000000) >> 24) ^
            ((val & 0x00FF0000) >> 16) ^
            ((val & 0x0000FF00) >>  8) ^
            ((val & 0x000000FF) >>  0);
}
