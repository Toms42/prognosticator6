#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/knobwidget.h"
#include "widgets/filterwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->knobCutoff, &KnobWidget::realValueUpdated, ui->envelopeViewer, &FilterWidget::setCutoff0);
    connect(ui->knobResonance, &KnobWidget::realValueUpdated, ui->envelopeViewer, &FilterWidget::setResonance0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

