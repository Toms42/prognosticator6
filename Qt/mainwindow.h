#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "synthesizer.h"
#include "patch.h"
#include <QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectToPatch(Patch *patch);

    void onVoiceMode(Patch::VoiceMode m);
    void onTimbreMode(Patch::TimbreMode m);
    void onChordMode(Patch::ChordMode m);
    void onArpMode(Patch::ArpMode m);

public slots:

    void onPatchUpdated(const Patch::PatchData &patch);
    void onPatchReloaded(const Patch::PatchData &patch);
    void onPatchSavingStateUpdate(int current_idx, int hover_idx, bool saveActive, bool loadActive, bool yesSelected);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
