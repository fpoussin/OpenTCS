#include "device.h"

device::device(QObject *parent) :
    QObject(parent)
{

    iNumDevs = 0;
}

bool device::connect(void)
{

    this->ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);

    if(ftStatus != FT_OK) {
        qDebug("Error: FT_ListDevices(%d)\n", (int)ftStatus);
        return true;
    }

    if((ftStatus = FT_OpenEx(cBufLD, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK){
        /*
            This can fail if the ftdi_sio driver is loaded
            use lsmod to check this and rmmod ftdi_sio to remove
            also rmmod usbserial
        */
        qDebug("Error FT_OpenEx(%d), device %d\n", (int)ftStatus, 0);
        qDebug("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
        qDebug("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
        return 1;
    }

    FT_SetTimeouts(ftHandle, 3000, 3000);	// 3 second read timeout

    if((ftStatus = FT_SetBaudRate(ftHandle, 115200)) != FT_OK) {
        qDebug("Error FT_SetBaudRate(%d), cBufLD[i] = %s\n", (int)ftStatus, cBufLD);
        return 1;
    }

    return this->resetBootloader();
}

bool device::disconnect()
{
    this->setCBUS(0);

    return FT_Close(ftHandle) == FT_OK;
}

bool device::write(char * buf, unsigned int len)
{
    unsigned int dwBytesWritten;
    ftStatus = FT_Write(ftHandle, buf, len, &dwBytesWritten);
    if (ftStatus != FT_OK) {
        qDebug("Error FT_Write(%d)\n", (int)ftStatus);
        return true;
    }
    return false;
}

bool device::read(char * buf, unsigned int len)
{
    unsigned int dwBytesRead;
    ftStatus = FT_Read(ftHandle, buf, len, &dwBytesRead);
    if (ftStatus != FT_OK) {
        qDebug("Error FT_Read(%d)\n", (int)ftStatus);
        return true;
    }
    return false;
}

bool device::readAll(char *buf, unsigned int max_len)
{
    unsigned int len = 0;

    ftStatus = FT_GetQueueStatus(ftHandle, &len);

    if(ftStatus != FT_OK) {
        qDebug("Error FT_GetQueueStatus(%d)\n", (int)ftStatus);
        return 1;
    }

    if (len > max_len)
    {
        len = max_len;
    }

    return this->read(buf, len);
}

bool device::resetBootloader()
{
    int res;

    /* Reset low, BOOT0 high */
    res = setCBUS(CBUS2MASK(0,0,0,1));
    usleep(200000);

    /* Reset high, BOOT0 high */
    res = setCBUS(CBUS2MASK(1,0,0,1));
    usleep(200000);

    /* Back to input mode */
    res = setCBUS(0);

    return res;
}

bool device::setCBUS(int mask)
{
    if(FT_SetBitMode(ftHandle, mask, 0x20) != FT_OK) {
        qDebug("Failed to set CBUS bit mask: %d\n", mask);
        return 1;
    }

    return false;
}

