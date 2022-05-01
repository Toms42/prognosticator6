#include "voice.h"

Voice::Voice(QObject *parent)
    : QObject{parent}
{

}

void Voice::update(qint64 t_ms) {

}

void Voice::add_note(int note, int velocity) {

}

Envelope::GateStatus Voice::gate_status() {

    Envelope::GateStatus status = Envelope::GATE_OFF;

    for (auto& v : _pvoices)
        if (v.gateStatus() > status)
            status = v.gateStatus();

    return status;
}

void Voice::release_note(int note) {

}

qint64 Voice::age_ms() {

}
