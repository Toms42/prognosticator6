#include "pseudovoice.h"

PseudoVoice::PseudoVoice(QObject *parent)
    : QObject{parent}
{

}

Envelope::GateStatus PseudoVoice::gateStatus() {
    return Envelope::GATE_OFF;
}
