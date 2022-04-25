#ifndef PSEUDOVOICE_H
#define PSEUDOVOICE_H

#include <QObject>
#include <patch.h>

class PseudoVoice : public QObject
{
    Q_OBJECT
public:
    explicit PseudoVoice(QObject *parent = nullptr);

    double getFreq();


public slots:
//    void updateEnvelope(double amp);
//    void updateFreq(double freq);
//    void updatePatch(Patch::PatchData data);

private:
    double phase;
    double freq;
    double amp;

signals:
};

#endif // PSEUDOVOICE_H
