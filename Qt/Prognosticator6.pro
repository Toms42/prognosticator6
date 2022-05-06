QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(ext/QMidi/src/QMidi.pri)

CONFIG += c++11
#QMAKE_CXXFLAGS += -march=armv7-a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    debugwindow.cpp \
    envelope.cpp \
    hidsender.cpp \
    i2c/knobs.cpp \
    main.cpp \
    mainwindow.cpp \
    modmatrix.cpp \
    patch.cpp \
    pseudovoice.cpp \
    synthesizer.cpp \
    voice.cpp \
    widgets/envelopewidget.cpp \
    widgets/filterwidget.cpp \
    widgets/knobwidget.cpp \
    widgets/lfowidget.cpp \
    widgets/matrixwidget.cpp \
    widgets/oscillatorwidget.cpp

HEADERS += \
    debugwindow.h \
    envelope.h \
    hidsender.h \
    i2c/knobs.h \
    knobinterface.h \
    mainwindow.h \
    modmatrix.h \
    musicutil.h \
    patch.h \
    pseudovoice.h \
    simplerandom.h \
    state.h \
    synth_util.h \
    synthesizer.h \
    voice.h \
    widgets/envelopewidget.h \
    widgets/filterwidget.h \
    widgets/knobwidget.h \
    widgets/lfowidget.h \
    widgets/matrixwidget.h \
    widgets/oscillatorwidget.h

FORMS += \
    debugwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

unix:!macx: LIBS += -lhidapi-hidraw -ludev
