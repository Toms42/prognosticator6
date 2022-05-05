#include "mainwindow.h"
#include "debugwindow.h"

#include <QApplication>
#include <QDir>
#include <QObject>

#include "synthesizer.h"
#include "patch.h"
#include <QMidiIn.h>

#include <QTimer>
#include <QThread>

#define SHOWDEBUGWINDOW

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    DebugWindow wd;
    w.show();

#ifdef SHOWDEBUGWINDOW
    wd.show();
#endif

    ////////////////////
    // Establish MIDI //
    ////////////////////

    auto midi = QMidiIn();
    QMap<QString,QString> midi_devices = midi.devices();

    //filter out "midi through" devices:
    for (auto &s : midi_devices.keys()) {
        if (midi_devices[s] == "Midi Through")
            midi_devices.remove(s);
    }
    // remaining midi devices:
    if(midi_devices.size() > 0) {
        qInfo("Midi devices found: (connecting to first)");
        for (auto &s : midi_devices.keys()) {
            qInfo(" - %s : %s",qUtf8Printable(s), qUtf8Printable(midi_devices[s]));
        }
        midi.connect(midi_devices.keys()[0]);
        midi.start();
        if (midi.isConnected()) {
            qInfo("Connection successful.");
        } else {
            qWarning("Midi connection failed!");
        }
    } else {
        qWarning("No midi devices found");
    }

    /////////////////
    // Setup Synth //
    /////////////////

    auto synth = Synthesizer();
    QObject::connect(&midi, &QMidiIn::midiEvent, &synth, &Synthesizer::midiEvent);

    auto patch = Patch(QDir(QDir::home().absolutePath() + QString("/.prog-patches/")));
    w.connectToPatch(&patch);
    wd.connectToPatch(&patch);

    /////////////////////////
    // Start synth (200Hz) //
    /////////////////////////
    synth.setPeriod(4);
    synth.start(QThread::TimeCriticalPriority);

//    auto t = QTimer();
//    QObject::connect(&t, &QTimer::timeout, &synth, &Synthesizer::update);
//    t.setTimerType(Qt::TimerType::PreciseTimer);
//    t.setSingleShot(false);
//    t.start(5);

    return a.exec();
    synth.quit();
    synth.requestInterruption();
    synth.wait();
}
