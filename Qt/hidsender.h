#ifndef HIDSENDER_H
#define HIDSENDER_H

// https://github.com/libusb/hidapi#installing-hidapi

#include <QObject>
#include <hidapi/hidapi.h>

class HidSender : public QObject
{
    Q_OBJECT

    // teensyduino serial
    const unsigned short VID = 0x16C0;
    const unsigned short  PID = 0x0486;

    // max string length
    const int MAX_STR = 255;

public:
    explicit HidSender(QObject *parent = nullptr);

public slots:
    void sendPacket(uint8_t buf[65]);

signals:

private:
    hid_device *_handle = nullptr;
    int _res;

};

#endif // HIDSENDER_H
