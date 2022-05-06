#include "matrixwidget.h"
#include <QPainter>
#include <QPen>

MatrixWidget::MatrixWidget(QWidget *parent)
    : QWidget{parent},
      _mod_matrix{{0}}
{
    memset(&_mod_matrix, 0, sizeof(_mod_matrix));
    memset(&_valid_cells, true, sizeof(_valid_cells));
}
void MatrixWidget::connectMatrixKnob(KnobWidget *knob) {
    _knob = knob;
    _knob->setMinimum(0);
    _knob->setMaximum(Patch::NUM_MOD_SINKS * Patch::NUM_MOD_SOURCES-1);
    _knob->setValue(0);
    _writemode = false;
    connect(_knob, &KnobWidget::valueChanged, this, &MatrixWidget::onPatchKnobChanged);
    connect(_knob, &KnobWidget::knobClicked, this, &MatrixWidget::onPatchKnobClicked);
}
void MatrixWidget::onPatchKnobChanged(int idx) {
    if (!_writemode) {
        hover_source = Patch::ModSource(idx/Patch::NUM_MOD_SINKS);
        hover_sink = Patch::ModSink(idx%Patch::NUM_MOD_SINKS);
        update();
    } else {
        emit cellSet(hover_source, hover_sink, idx);
    }
}
void MatrixWidget::onPatchKnobClicked() {
    if (!_writemode) {
        // enter writemode
        if (!_valid_cells[hover_source][hover_sink]) {
            return;
        }
        _writemode = true;
        _knob->setMaximum(127);
        _knob->setMinimum(-127);
        _knob->setValue(_mod_matrix[hover_source][hover_sink]);
    } else {
        _writemode = false;
        _knob->setMaximum(Patch::NUM_MOD_SINKS * Patch::NUM_MOD_SOURCES-1);
        _knob->setMinimum(0);
        _knob->setValue(hover_source*Patch::NUM_MOD_SINKS + hover_sink);
    }
    update();
}

void MatrixWidget::onPatchReload(const Patch::PatchData &patch) {
    _writemode = false;
    _knob->setMaximum(Patch::NUM_MOD_SINKS * Patch::NUM_MOD_SOURCES-1);
    _knob->setMinimum(0);
    _knob->setValue(hover_source*Patch::NUM_MOD_SINKS + hover_sink);
}

void MatrixWidget::onPatchUpdated(const Patch::PatchData &patch) {
    memcpy(_mod_matrix, patch.mod_matrix, sizeof(_mod_matrix));

    memset(&_valid_cells, true, sizeof(_valid_cells));
    if (patch.voice_mode == Patch::VOICE_PARAPHONIC) {
        // env0 can't really modify much because there's too many different amp envs.
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_FILTER_CUTOFF] = false;
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_FILTER_RESONANCE] = false;
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_AMP] = false;
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_CHORUS_DEPTH] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_FILTER_CUTOFF] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_FILTER_RESONANCE] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_AMP] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_CHORUS_DEPTH] = false;
    }
    if (patch.voice_mode != Patch::VOICE_MONO && patch.voice_mode != Patch::VOICE_SINGLE) {
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_STEREOPAN] = false;
        _valid_cells[Patch::MODSRC_ENV0][Patch::MOD_STEREOPHASE] = false;
        _valid_cells[Patch::MODSRC_ENV1][Patch::MOD_STEREOPAN] = false;
        _valid_cells[Patch::MODSRC_ENV1][Patch::MOD_STEREOPHASE] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_STEREOPAN] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_STEREOPHASE] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_STEREOPAN] = false;
        _valid_cells[Patch::MODSRC_VEL][Patch::MOD_STEREOPHASE] = false;
    }
    update();
}

void MatrixWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;

    double cellHeight = (h-_topmargin-_gridwidth)/Patch::NUM_MOD_SOURCES;
    double cellWidth = (w-_leftmargin-_gridwidth)/Patch::NUM_MOD_SINKS;

    // draw outer rectangle and grid
    pen.setWidthF(_gridwidth);
    pen.setColor(QColor(100,100,100));
    painter.setPen(pen);
    painter.drawRect(QRectF(_leftmargin, _topmargin, w-_leftmargin-_gridwidth, h-_topmargin-_gridwidth));
    for (int so = 1; so < Patch::NUM_MOD_SOURCES; so++) {
        double y = cellHeight*so+_topmargin;
        painter.drawLine(_leftmargin,y,w-_gridwidth,y);
    }
    for (int si = 1; si < Patch::NUM_MOD_SINKS; si++) {
        double x = cellWidth*si+_leftmargin;
        painter.drawLine(x, _topmargin, x, h-_gridwidth);
    }

    // draw text
    pen.setColor(_color0);
    painter.setPen(pen);
    for (int so = 0; so < Patch::NUM_MOD_SOURCES; so++) {
        double x0 = 0;
        double y0 = cellHeight*so+_topmargin;
        double x1 = _leftmargin;
        double y1 = cellHeight*(so+1)+_topmargin;
        QRectF bbox(x0,y0,x1-x0,y1-y0);
        painter.drawText(bbox, Qt::AlignRight | Qt::AlignHCenter, MOD_SOURCE_NAMES.value(Patch::ModSource(so), QString("????")));
    }
    pen.setColor(_color1);
    painter.setPen(pen);
    for (int si = 0; si < Patch::NUM_MOD_SINKS; si++) {
        double x0 = cellWidth*si+_leftmargin;
        double y0 = 0;
        double x1 = cellWidth*(si+1)+_leftmargin;
        double y1 = _topmargin;
        QRectF bbox(x0,y0,x1-x0,y1-y0);
        painter.drawText(bbox, Qt::AlignHCenter | Qt::AlignBottom, MOD_SINK_NAMES.value(Patch::ModSink(si), QString("????")));
    }

    // draw hover
    double x0 = cellWidth*hover_sink+_leftmargin;
    double y0 = cellHeight*hover_source+_topmargin;
    double x1 = cellWidth*(hover_sink+1)+_leftmargin;
    double y1 = cellHeight*(hover_source+1)+_topmargin;
    QRectF box(x0,y0,x1-x0,y1-y0);
    QColor c = _writemode ? _color0 : _color1;
    pen.setColor(c);
    painter.setPen(pen);
    painter.drawRect(box);

    // draw cell contents
    QFont font = painter.font();
    font.setPointSize(9); // TODO: property
    painter.setFont(font);
    for (int so = 0; so < Patch::NUM_MOD_SOURCES; so++) {
        for (int si = 0; si < Patch::NUM_MOD_SINKS; si++) {
            double x0 = cellWidth*si+_leftmargin;
            double y0 = cellHeight*so+_topmargin;
            double x1 = cellWidth*(si+1)+_leftmargin;
            double y1 = cellHeight*(so+1)+_topmargin;
            QRectF bbox(x0,y0,x1-x0,y1-y0);

            qint8 v = _mod_matrix[so][si];
            QColor c = v < 0 ? _color1 : _color0;
            if (!_valid_cells[so][si]) {
                continue;
            }
            pen.setColor(c);
            painter.setPen(pen);
            painter.drawText(bbox, Qt::AlignCenter, QString::number(v));
        }
    }
}
