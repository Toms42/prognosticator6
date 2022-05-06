#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include "patch.h"
#include "widgets/knobwidget.h"
#include "knobinterface.h"

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = nullptr);
    void connectToPatch(Patch *patch);
    void connectKnobSystem(KnobInterface *ki);
    KnobWidget *getMatrixKnob();
    ~DebugWindow();

public slots:

    void onPatchUpdated(const Patch::PatchData &patch);
    void onPatchReloaded(const Patch::PatchData &patch);

private:
    Ui::DebugWindow *ui;
};

#endif // DEBUGWINDOW_H
