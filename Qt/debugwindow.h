#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include "patch.h"

namespace Ui {
class DebugWindow;
}

class DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = nullptr);
    void connectToPatch(Patch *patch);
    ~DebugWindow();

public slots:

    void onPatchUpdated(const Patch::PatchData &patch);
    void onPatchReloaded(const Patch::PatchData &patch);

private:
    Ui::DebugWindow *ui;
};

#endif // DEBUGWINDOW_H
