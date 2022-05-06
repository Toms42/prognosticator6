#include "hidsender.h"
#include <stdlib.h>

inline bool m(int x, QString msg="hid_error") {
    if (x != 0) {
        qWarning("%s: %d", qUtf8Printable(msg), x);
        return false;
    }
    return true;
}

HidSender::HidSender(QObject *parent)
    : QObject{parent}
{
    qInfo("setting up hid devices...");

    if (hid_init()) {
        qWarning("...failed to init hid!");
        return;
    }

    qInfo("...enumerating");
    struct hid_device_info *devs, *cur_dev;
    devs = hid_enumerate(VID, PID);
    cur_dev = devs;
    while (cur_dev) {
        qInfo("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        qInfo(" ");
        cur_dev = cur_dev->next;
    }

    _handle = hid_open(VID, PID, NULL);
    if (_handle == NULL) {
        qInfo("Failed to open hid device. No hid.");
        return;
    }

    wchar_t wstr[MAX_STR];
    char str[MAX_STR];
    m(hid_get_manufacturer_string(_handle, wstr, MAX_STR));
    wcstombs(str, wstr, MAX_STR);
    qInfo("connected to hid device!\n - %s",str);

    m(hid_get_product_string(_handle, wstr, MAX_STR));
    wcstombs(str, wstr, MAX_STR);
    qInfo(" - %s",str);

    m(hid_get_serial_number_string(_handle, wstr, MAX_STR));
    wcstombs(str, wstr, MAX_STR);
    qInfo(" - S/N: %s",str);
}

void HidSender::sendPacket(uint8_t buf[65]) {

}
