#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/knobwidget.h"
#include "widgets/filterwidget.h"
#include "widgets/oscillatorwidget.h"
#include "widgets/lfowidget.h"

#include "synthesizer.h"
#include "patch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->knobCutoff, &KnobWidget::realValueUpdated, ui->envelopeViewer, &FilterWidget::setCutoff0);
    connect(ui->knobResonance, &KnobWidget::realValueUpdated, ui->envelopeViewer, &FilterWidget::setResonance0);
    connect(ui->knobShape0, &KnobWidget::realValueUpdated, ui->oscillatorViewer, &OscillatorWidget::setShape0);
    connect(ui->knobShape1, &KnobWidget::realValueUpdated, ui->oscillatorViewer, &OscillatorWidget::setShape1);
    connect(ui->knobLfoShape0, &KnobWidget::valueChanged, ui->lfoViewer0, &LfoWidget::updateShape);
    connect(ui->knobLfoShape1, &KnobWidget::valueChanged, ui->lfoViewer1, &LfoWidget::updateShape);
}

void MainWindow::onVoiceMode(Patch::VoiceMode m) {
    switch (m) {
    case Patch::VOICE_SINGLE:
        ui->radioVoiceSingle->setChecked(true);
        break;
    case Patch::VOICE_MONO:
        ui->radioVoiceMono->setChecked(true);
        break;
    case Patch::VOICE_PARAPHONIC:
        ui->radioVoicePara->setChecked(true);
        break;
    case Patch::VOICE_DUOPHONIC:
        ui->radioVoiceDuo->setChecked(true);
        break;
    }
}
void MainWindow::onTimbreMode(Patch::TimbreMode m) {
    switch (m) {
    case Patch::TIMBRE_SINGLE_MONO:
        ui->radioTimbreSingle->setChecked(true);
        break;
    case Patch::TIMBRE_SINGLE_STEREO:
        ui->radioTimbreStereo->setChecked(true);
        break;
    case Patch::TIMBRE_DUAL:
        ui->radioTimbreDual->setChecked(true);
        break;
    case Patch::TIMBRE_DUALMIDI:
        ui->radioTimbreSplit->setChecked(true);
        break;
    }
}
void MainWindow::onChordMode(Patch::ChordMode m) {
    switch (m) {
    case Patch::CHORD_OFF:
        ui->radioChordOff->setChecked(true);
        break;
    case Patch::CHORD_MAJOR:
        ui->radioChordMajor->setChecked(true);
        break;
    case Patch::CHORD_MINOR:
        ui->radioChordMinor->setChecked(true);
        break;
    case Patch::CHORD_KEY3:
        ui->radioChordKey3->setChecked(true);
        break;
    case Patch::CHORD_KEY4:
        ui->radioChordKey4->setChecked(true);
        break;
    }
}
void MainWindow::onArpMode(Patch::ArpMode m) {
    switch (m) {
    case Patch::ARP_OFF:
        ui->radioArpOff->setChecked(true);
        break;
    case Patch::ARP_UP:
        ui->radioArpUp->setChecked(true);
        break;
    case Patch::ARP_DOWN:
        ui->radioArpDown->setChecked(true);
        break;
    case Patch::ARP_UPDOWN:
        ui->radioArpUpdown->setChecked(true);
        break;
    case Patch::ARP_RAND:
        ui->radioArpRand->setChecked(true);
        break;
    }
}

void MainWindow::connectToPatch(Patch *patch) {
    connect(ui->knobVoice, &KnobWidget::valueChanged, patch, &Patch::updateVoiceMode);
    connect(ui->knobTimbre, &KnobWidget::valueChanged, patch, &Patch::updateTimbreMode);
    connect(ui->knobTimbreSelect, &KnobWidget::valueChanged, patch, &Patch::updateTimbre);
    connect(ui->knobArpMode, &KnobWidget::valueChanged, patch, &Patch::updateArpMode);
    connect(ui->knobChordMode, &KnobWidget::valueChanged, patch, &Patch::updateChordMode);
    connect(ui->knobLfoFreq0, &KnobWidget::realValueUpdated, patch, &Patch::updateLfoFreq0);
    connect(ui->knobLfoFreq1, &KnobWidget::realValueUpdated, patch, &Patch::updateLfoFreq1);
    connect(ui->knobLfoShape0, &KnobWidget::valueChanged, patch, &Patch::updateLfoShape0);
    connect(ui->knobLfoShape1, &KnobWidget::valueChanged, patch, &Patch::updateLfoShape1);
}


MainWindow::~MainWindow()
{
    delete ui;
}
