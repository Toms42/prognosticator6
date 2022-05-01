QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(ext/QMidi/src/QMidi.pri)

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    envelope.cpp \
    main.cpp \
    mainwindow.cpp \
    modmatrix.cpp \
    patch.cpp \
    pseudovoice.cpp \
    synthesizer.cpp \
    voice.cpp

HEADERS += \
    envelope.h \
    mainwindow.h \
    modmatrix.h \
    patch.h \
    pseudovoice.h \
    synth_util.h \
    synthesizer.h \
    voice.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc
