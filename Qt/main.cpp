#include "mainwindow.h"

#include <QApplication>
#include <QDir>

#include "synthesizer.h"
#include "patch.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto synth = Synthesizer();
    auto patch = Patch(QDir("./patches"));

    return a.exec();
}
