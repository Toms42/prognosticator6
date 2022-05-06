#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/knobwidget.h"
#include "widgets/filterwidget.h"
#include "widgets/oscillatorwidget.h"
#include "widgets/lfowidget.h"
#include "widgets/matrixwidget.h"
#include "knobinterface.h"

#include "synthesizer.h"
#include "patch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach (QWidget *widget, QApplication::allWidgets()) {
        widget->setFont(QApplication::font());
        widget->update();
    }

    connect(ui->knobShape0, &KnobWidget::realValueUpdated, ui->oscillatorViewer, &OscillatorWidget::setShape0);
    connect(ui->knobShape1, &KnobWidget::realValueUpdated, ui->oscillatorViewer, &OscillatorWidget::setShape1);
    connect(ui->knobLfoShape0, &KnobWidget::valueChanged, ui->lfoViewer0, &LfoWidget::updateShape);
    connect(ui->knobLfoShape1, &KnobWidget::valueChanged, ui->lfoViewer1, &LfoWidget::updateShape);
}

void MainWindow::connectMatrixKnob(KnobWidget *knob) {
    ui->matrixWidget->connectMatrixKnob(knob);
}

void MainWindow::onPatchSavingStateUpdate(int current_idx, int hover_idx, bool saveActive, bool loadActive, bool yesSelected) {
    ui->labelCurPatch->setText(QString::number(current_idx));
    ui->labelNextPatch->setText(QString::number(hover_idx));
    ui->labelLoad->setVisible(loadActive);

    ui->radioLoadNo->setVisible(loadActive);
    ui->radioLoadYes->setVisible(loadActive);
    ui->labelSave->setVisible(saveActive);

    ui->labelSave->setVisible(saveActive);
    ui->radioSaveNo->setVisible(saveActive);
    ui->radioSaveYes->setVisible(saveActive);

    if (yesSelected) {
        ui->radioSaveNo->setChecked(false);
        ui->radioLoadNo->setChecked(false);
        ui->radioSaveYes->setChecked(true);
        ui->radioLoadYes->setChecked(true);
    } else {
        ui->radioSaveYes->setChecked(false);
        ui->radioLoadYes->setChecked(false);
        ui->radioSaveNo->setChecked(true);
        ui->radioLoadNo->setChecked(true);
    }

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

void MainWindow::onPatchUpdated(const Patch::PatchData &patch) {
    onVoiceMode(patch.voice_mode);
    onTimbreMode(patch.timbre_mode);
    onChordMode(patch.midi_chord_mode);
    onArpMode(patch.midi_arp_mode);
    ui->filterViewer->setCutoff(patch.timbre[patch.activeTimbre].filter_freq);
    ui->filterViewer->setResonance(patch.timbre[patch.activeTimbre].filter_resonance);
//    qInfo("patch updated");
}

void MainWindow::connectToPatch(Patch *patch) {
    // knobs to patch
    connect(ui->knobVoice, &KnobWidget::valueChanged, patch, &Patch::updateVoiceMode);
    connect(ui->knobTimbre, &KnobWidget::valueChanged, patch, &Patch::updateTimbreMode);
    connect(ui->knobTimbreSelect, &KnobWidget::valueChanged, patch, &Patch::updateTimbre);
    connect(ui->knobArpMode, &KnobWidget::valueChanged, patch, &Patch::updateArpMode);
    connect(ui->knobChordMode, &KnobWidget::valueChanged, patch, &Patch::updateChordMode);
    connect(ui->knobLfoFreq0, &KnobWidget::realValueUpdated, patch, &Patch::updateLfoFreq0);
    connect(ui->knobLfoFreq1, &KnobWidget::realValueUpdated, patch, &Patch::updateLfoFreq1);
    connect(ui->knobLfoShape0, &KnobWidget::valueChanged, patch, &Patch::updateLfoShape0);
    connect(ui->knobLfoShape1, &KnobWidget::valueChanged, patch, &Patch::updateLfoShape1);
    connect(ui->knobShape0, &KnobWidget::realValueUpdated, patch, &Patch::updateOscShape0);
    connect(ui->knobShape1, &KnobWidget::realValueUpdated, patch, &Patch::updateOscShape1);
    connect(ui->knobDetune, &KnobWidget::realValueUpdated, patch, &Patch::updateOscDetune);
    connect(ui->knobOctave, &KnobWidget::valueChanged, patch, &Patch::updateOscOctave);
    connect(ui->knobGlide, &KnobWidget::realValueUpdated, patch, &Patch::updateVoiceGlide);
    connect(ui->knobChorus, &KnobWidget::valueChanged, patch, &Patch::updateChorus);
    connect(ui->knobCutoff, &KnobWidget::realValueUpdated, patch, &Patch::updateFilterFreq);
    connect(ui->knobResonance, &KnobWidget::realValueUpdated, patch, &Patch::updateFilterResonance);
    connect(ui->knobMix0, &KnobWidget::realValueUpdated, patch, &Patch::updateMixer0);
    connect(ui->knobMix1, &KnobWidget::realValueUpdated, patch, &Patch::updateMixer1);
    connect(ui->ampKnobAttack, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv0Attack);
    connect(ui->ampKnobDecay, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv0Decay);
    connect(ui->ampKnobSustain, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv0Sustain);
    connect(ui->ampKnobRelease, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv0Release);
    connect(ui->filterKnobAttack, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv1Attack);
    connect(ui->filterKnobDecay, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv1Decay);
    connect(ui->filterKnobSustain, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv1Sustain);
    connect(ui->filterKnobRelease, &KnobWidget::realValueUpdated, patch, &Patch::updateEnv1Release);

    // mod matrix
    connect(patch, &Patch::updated, ui->matrixWidget, &MatrixWidget::onPatchUpdated);
    connect(patch, &Patch::reloaded, ui->matrixWidget, &MatrixWidget::onPatchReload);
    connect(ui->matrixWidget, &MatrixWidget::cellSet, patch, &Patch::updateModCell);

    // patch update
    connect(patch, &Patch::updated, this, &MainWindow::onPatchUpdated);
    connect(patch, &Patch::reloaded, this, &MainWindow::onPatchReloaded);
    connect(patch, &Patch::patchSavingState, this, &MainWindow::onPatchSavingStateUpdate);
}

void MainWindow::onPatchReloaded(const Patch::PatchData &patch) {
    ui->knobVoice->setValue(patch.voice_mode);
    ui->knobTimbre->setValue(patch.timbre_mode);

    ui->knobArpMode->setValue(patch.midi_arp_mode);
    ui->knobChordMode->setValue(patch.midi_chord_mode);
    ui->knobLfoFreq0->setRealValue(patch.lfo0_freq);
    ui->knobLfoFreq1->setRealValue(patch.lfo1_freq);
    ui->knobLfoShape0->setValue(patch.lfo0_shape);
    ui->knobLfoShape1->setValue(patch.lfo1_shape);
    ui->knobShape0->setRealValue(patch.timbre[patch.activeTimbre].osc_shape0);
    ui->knobShape1->setRealValue(patch.timbre[patch.activeTimbre].osc_shape1);
    ui->knobDetune->setRealValue(patch.timbre[patch.activeTimbre].osc_detune);
    ui->knobOctave->setValue(patch.timbre[patch.activeTimbre].osc_octave);
    ui->knobGlide->setRealValue(patch.timbre[patch.activeTimbre].voice_glide);
    ui->knobChorus->setValue(patch.timbre[patch.activeTimbre].voice_chorus);
    ui->knobCutoff->setRealValue(patch.timbre[patch.activeTimbre].filter_freq);
    ui->knobResonance->setRealValue(patch.timbre[patch.activeTimbre].filter_resonance);
    ui->knobMix0->setRealValue(100*patch.timbre[patch.activeTimbre].mixer_amount0);
    ui->knobMix1->setRealValue(100*patch.timbre[patch.activeTimbre].mixer_amount1);
    ui->ampKnobAttack->setRealValue(patch.timbre[patch.activeTimbre].env0_attack);
    ui->ampKnobDecay->setRealValue(patch.timbre[patch.activeTimbre].env0_decay);
    ui->ampKnobSustain->setRealValue(100*patch.timbre[patch.activeTimbre].env0_sustain);
    ui->ampKnobRelease->setRealValue(patch.timbre[patch.activeTimbre].env0_release);
    ui->filterKnobAttack->setRealValue(patch.timbre[patch.activeTimbre].env1_attack);
    ui->filterKnobDecay->setRealValue(patch.timbre[patch.activeTimbre].env1_decay);
    ui->filterKnobSustain->setRealValue(100*patch.timbre[patch.activeTimbre].env1_sustain);
    ui->filterKnobRelease->setRealValue(patch.timbre[patch.activeTimbre].env1_release);
}

void MainWindow::connectKnobSystem(KnobInterface *ki) {
    connect(ki, &KnobInterface::shape0Occurance, ui->knobShape0, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::shape1Occurance, ui->knobShape1, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::detuneOccurance, ui->knobDetune, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::octaveOccurance, ui->knobOctave, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::voiceModeOccurance, ui->knobVoice, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::glideOccurance, ui->knobGlide, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::chorusOccurance, ui->knobChorus, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::mix0Occurance, ui->knobMix0, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::mix1Occurance, ui->knobMix1, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::arpOccurance, ui->knobArpMode, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::chordOccurance, ui->knobChordMode, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::cutoffOccurance, ui->knobCutoff, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::resonanceOccurance, ui->knobResonance, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::volumeOccurance, ui->knobVolume, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::lfoFreq0Occurance, ui->knobLfoFreq0, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::lfoShape0Occurance, ui->knobLfoShape0, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::lfoFreq0Occurance, ui->knobLfoFreq1, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::lfoShape1Occurance, ui->knobLfoShape1, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e0aOccurance, ui->ampKnobAttack, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e0dOccurance, ui->ampKnobDecay, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e0sOccurance, ui->ampKnobSustain, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e0rOccurance, ui->ampKnobRelease, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e1aOccurance, ui->filterKnobAttack, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e1dOccurance, ui->filterKnobDecay, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e1sOccurance, ui->filterKnobSustain, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::e1rOccurance, ui->filterKnobRelease, &KnobWidget::knobOccurance);


}

MainWindow::~MainWindow()
{
    delete ui;
}
