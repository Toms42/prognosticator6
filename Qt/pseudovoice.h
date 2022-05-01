#ifndef PSEUDOVOICE_H
#define PSEUDOVOICE_H

#include <QObject>
#include <QTime>
#include <patch.h>
#include <envelope.h>

class PseudoVoice : public QObject
{
    Q_OBJECT
public:
    explicit PseudoVoice(QObject *parent = nullptr);
    void update(qint64 t_ms);
    double getFreq();
    void setNote();
    void releaseNote();
    Envelope::GateStatus gateStatus();

public slots:
//    void updateEnvelope(double amp);
//    void updateFreq(double freq);
//    void updatePatch(Patch::PatchData data);

private:
    Envelope _amp_env;
    double _phase;
    double _freq;
    double _amp;

signals:
};

#endif // PSEUDOVOICE_H
