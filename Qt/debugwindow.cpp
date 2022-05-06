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
    connect(ui->buttonMatrix, &QPushButton::pressed, ui->knobMatrix, &KnobWidget::setKnobPressed);
    connect(ui->buttonMatrix, &QPushButton::released, ui->knobMatrix, &KnobWidget::setKnobReleased);
}

DebugWindow::~DebugWindow()
{
    delete ui;
}

KnobWidget *DebugWindow::getMatrixKnob() {
    return ui->knobMatrix;
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
void DebugWindow::connectKnobSystem(KnobInterface *ki) {
    connect(ki, &KnobInterface::matrixOccurance, ui->knobMatrix, &KnobWidget::knobOccurance);
    connect(ki, &KnobInterface::patchOccurance, ui->knobPatchSelect, &KnobWidget::knobOccurance);

}
