#include "debugwindow.h"
#include "ui_debugwindow.h"

#include <QPushButton>
#include "widgets/knobwidget.h"

DebugWindow::DebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DebugWindow)
{
    ui->setupUi(this);
    connect(ui->buttonPatchSave, &QPushButton::pressed, ui->knobPatchSelect, &KnobWidget::setKnobPressed);
    connect(ui->buttonPatchSave, &QPushButton::released, ui->knobPatchSelect, &KnobWidget::setKnobReleased);
}

DebugWindow::~DebugWindow()
{
    delete ui;
}

void DebugWindow::connectToPatch(Patch *patch) {
    connect(ui->knobPatchSelect, &KnobWidget::knobClicked, patch, &Patch::patchButtonClicked);
    connect(ui->knobPatchSelect, &KnobWidget::knobLongClicked, patch, &Patch::patchButtonLongClicked);
    connect(ui->knobPatchSelect, &KnobWidget::valueChanged, patch, &Patch::patchButtonValueChanged);
}

void DebugWindow::onPatchUpdated(const Patch::PatchData &patch) {

}
void DebugWindow::onPatchReloaded(const Patch::PatchData &patch) {

}
