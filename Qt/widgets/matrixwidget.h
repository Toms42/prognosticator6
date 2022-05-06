#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QMap>
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include "patch.h"
#include "knobwidget.h"
#include <QSet>
#include <QPair>

const QMap<Patch::ModSource, QString> MOD_SOURCE_NAMES = {
    {Patch::MODSRC_ENV0,  "env.amp"},
    {Patch::MODSRC_ENV1,  "env.filter"},
    {Patch::MODSRC_LFO0,  "lfo.0"},
    {Patch::MODSRC_LFO1,  "lfo.1"},
    {Patch::MODSRC_MOD,  "mod"},
    {Patch::MODSRC_PB,  "pitchbend"},
    {Patch::MODSRC_VEL,  "velocity"},
};

const QMap<Patch::ModSink, QString> MOD_SINK_NAMES = {
    {Patch::MOD_OSC_DETUNE,  "detune"},
    {Patch::MOD_OSC_PITCH,  "pitch"},
    {Patch::MOD_FILTER_CUTOFF,  "cutoff"},
    {Patch::MOD_FILTER_RESONANCE,  "reso"},
    {Patch::MOD_AMP,  "amp"},
    {Patch::MOD_CHORUS_DEPTH,  "chorus"},
    {Patch::MOD_STEREOPAN,  "pan"},
    {Patch::MOD_STEREOPHASE,  "phase"},
    {Patch::MOD_LFO0,  "lfo.0"},
    {Patch::MOD_LFO1,  "lfo.1"},
};

class MatrixWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor colorpos READ getColor0 WRITE setColor0)
    Q_PROPERTY(QColor colorneg READ getColor1 WRITE setColor1)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
    Q_PROPERTY(double gridwidth READ getGridWidth WRITE setGridWidth)
    Q_PROPERTY(double topmargin READ getTopMargin WRITE setTopMargin)
    Q_PROPERTY(double leftmargin READ getLeftMargin WRITE setLeftMargin)
public:
    explicit MatrixWidget(QWidget *parent = nullptr);
    QColor getColor0() const {return _color0;};
    QColor getColor1() const {return _color1;};
    double getLineWidth() const {return _linewidth;};
    double getGridWidth() const {return _gridwidth;};
    double getTopMargin() const {return _topmargin;};
    double getLeftMargin() const {return _leftmargin;};
    void connectMatrixKnob(KnobWidget *knob);

public slots:
    void setColor0(QColor c) {_color0 = c;};
    void setColor1(QColor c) {_color1 = c;};
    void setLineWidth(double w) {_linewidth = w;};
    void setGridWidth(double w) {_gridwidth = w;};
    void setTopMargin(double w) {_topmargin = w;};
    void setLeftMargin(double w) {_leftmargin = w;};
    void onPatchUpdated(const Patch::PatchData &patch);
    void onPatchReload(const Patch::PatchData &patch);

    void onPatchKnobChanged(int idx);
    void onPatchKnobClicked();

protected:
    virtual void paintEvent(QPaintEvent *pe) override;

private:
    QColor _color0 = Qt::white;
    QColor _color1 = Qt::white;
    double _linewidth = 1;
    double _gridwidth = 1;
    double _topmargin = 1;
    double _leftmargin = 1;
    bool _writemode = false;

    Patch::ModSource hover_source = Patch::ModSource(0);
    Patch::ModSink hover_sink = Patch::ModSink(0);

    qint8 _mod_matrix[Patch::NUM_MOD_SOURCES][Patch::NUM_MOD_SINKS];
    bool _valid_cells[Patch::NUM_MOD_SOURCES][Patch::NUM_MOD_SINKS];
    KnobWidget *_knob = nullptr;

signals:
    void cellSet(Patch::ModSource, Patch::ModSink, qint8 amount);
};

#endif // MATRIXWIDGET_H
