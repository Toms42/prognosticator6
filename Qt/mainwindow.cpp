#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/knobwidget.h"
#include "widgets/filterwidget.h"
#include "widgets/oscillatorwidget.h"
#include "widgets/lfowidget.h"

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

MainWindow::~MainWindow()
{
    delete ui;
}

