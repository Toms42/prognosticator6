#ifndef KNOBINTERFACE_H
#define KNOBINTERFACE_H

#include <QObject>
#include <QThread>
#include <QMap>
#include "i2c/knobs.h"

class KnobInterface : public QThread
{
    Q_OBJECT
public:
    explicit KnobInterface(QObject *parent = nullptr)
        :values{QMap<int,int>()}
    {
        openBus();
        for (int i = 0x10; i < 0x2C; i++) {
            configEncoder(i);
            values[i]=0;
        }
    };
    void run() {
        while (!isInterruptionRequested()) {
            msleep(30);
            for (int i = 0x10; i < 0x2C; i++) {
                int newV = getVal(i);
                int diff = newV - values[i];
                if (diff > 0) {
                    for (int i = 0; i < diff; i++) {
                        emitFor(i,0); // increment
                    }
                } else if (diff < 0) {
                    for (int i = 0; i < -diff; i++) {
                        emitFor(i, 1); // decrement
                    }
                }
                values[i] = newV;
                if (wasPushed(i)) {
                    emitFor(i, 2);
                }
                if (wasLongPushed(i)) {
                    emitFor(i, 3);
                }
            }
        }
    };

private:
    QMap<int,int> values;
    void emitFor(int addy, int occurance) {
        switch (addy) {
        case 0x10: emit shape0Occurance(occurance); break;
        case 0x11: emit shape1Occurance(occurance); break;
        case 0x12: emit detuneOccurance(occurance); break;
        case 0x13: emit octaveOccurance(occurance); break;
        case 0x14: emit voiceModeOccurance(occurance); break;
        case 0x15: emit glideOccurance(occurance); break;
        case 0x16: emit chorusOccurance(occurance); break;
        case 0x17: emit mix0Occurance(occurance); break;
        case 0x18: emit mix1Occurance(occurance); break;
        case 0x19: emit arpOccurance(occurance); break;
        case 0x1A: emit chordOccurance(occurance); break;
        case 0x1B: emit matrixOccurance(occurance); break;
        case 0x1C: emit patchOccurance(occurance); break;
        case 0x1D: emit cutoffOccurance(occurance); break;
        case 0x1E: emit resonanceOccurance(occurance); break;
        case 0x1F: emit volumeOccurance(occurance); break;
        case 0x20: emit lfoFreq0Occurance(occurance); break;
        case 0x21: emit lfoShape0Occurance(occurance); break;
        case 0x22: emit lfoFreq1Occurance(occurance); break;
        case 0x23: emit lfoShape1Occurance(occurance); break;
        case 0x24: emit e0aOccurance(occurance); break;
        case 0x25: emit e0dOccurance(occurance); break;
        case 0x26: emit e0sOccurance(occurance); break;
        case 0x27: emit e0rOccurance(occurance); break;
        case 0x28: emit e1aOccurance(occurance); break;
        case 0x29: emit e1dOccurance(occurance); break;
        case 0x2A: emit e1sOccurance(occurance); break;
        case 0x2B: emit e1rOccurance(occurance); break;
        }

    };

signals:
    void shape0Occurance(int);
    void shape1Occurance(int);
    void detuneOccurance(int);
    void octaveOccurance(int);
    void voiceModeOccurance(int);
    void glideOccurance(int);
    void chorusOccurance(int);
    void mix0Occurance(int);
    void mix1Occurance(int);
    void arpOccurance(int);
    void chordOccurance(int);
    void matrixOccurance(int);
    void patchOccurance(int);
    void cutoffOccurance(int);
    void resonanceOccurance(int);
    void volumeOccurance(int);
    void lfoFreq0Occurance(int);
    void lfoFreq1Occurance(int);
    void lfoShape0Occurance(int);
    void lfoShape1Occurance(int);
    void e0aOccurance(int);
    void e0dOccurance(int);
    void e0sOccurance(int);
    void e0rOccurance(int);
    void e1aOccurance(int);
    void e1dOccurance(int);
    void e1sOccurance(int);
    void e1rOccurance(int);
};

#endif // KNOBINTERFACE_H
