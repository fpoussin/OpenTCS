#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDebug>

#include "ftd2xx.h"

#define CBUS2MASK(a, b, c, d) (0xF0|(0x0F&((1&a)|(2&(b<<1))|(4&(c<<2))|(8&(d<<3)))))

class device : public QObject
{
    Q_OBJECT
public:
    explicit device(QObject *parent = 0);
    bool connect(void);
    bool disconnect(void);
    bool write(char *buf, unsigned int len);
    bool read(char * buf, unsigned int len);
    bool readAll(char * buf, unsigned int max_len);
    
signals:
    
public slots:

private slots:
    bool resetBootloader(void);
    bool setCBUS(int mask);

private:
    int             iNumDevs;
    char *          pcBufLD[1];
    char            cBufLD[64];
    FT_STATUS       ftStatus;
    FT_HANDLE       ftHandle;
};

#endif // DEVICE_H
