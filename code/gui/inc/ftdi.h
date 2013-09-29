#ifndef FTDI_H
#define FTDI_H

#include <QObject>
#include <QDebug>

#include "compat.h"
#include "ftd2xx.h"

#define CBUS2MASK(a, b, c, d) (0xF0|(0x0F&((1&a)|(2&(b<<1))|(4&(c<<2))|(8&(d<<3)))))

class ftdi : public QObject
{
    Q_OBJECT
public:
    explicit ftdi(QObject *parent = 0);
    ~ftdi();
    bool connect(void);
    bool disconnect(void);
    bool write(quint8 * buf, quint32 len = 1);
    bool read(quint8 * buf, quint32 len = 1);
    bool readAll(quint8 * buf, quint32 max_len);

    bool setCBUSMux(bool en);
    bool setCBUS(int mask);
    bool resetBootloader(void);
    bool resetNormal(void);

    bool isConnected(void) { return connected; }
    
signals:
    
public slots:

private slots:
    bool purge(void);

private:
    bool                connected;
    qint32              iNumDevs;
    qint8 *             pcBufLD[1];
    qint8               cBufLD[64];
    FT_STATUS           ftStatus;
    FT_HANDLE           ftHandle;
    FT_EEPROM_HEADER    ftEepromHeader;
    FT_EEPROM_X_SERIES  eepromDATA;
};

#endif // FTDI_H
