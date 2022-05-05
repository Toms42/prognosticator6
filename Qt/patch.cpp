#include "patch.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QtNumeric>
#include <QDir>

Patch::Patch(QDir patch_dir, QObject *parent)
    : QObject{parent}
{
    memset(&_p, 0, sizeof(_p));
    folder = patch_dir;
    if (!folder.exists()) {
        folder.mkpath(folder.absolutePath());
    }
    _onChange();
    emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
}

void Patch::patchButtonClicked() {
    if (saveActive && yesSelected) {
        // save
        save(hover_idx);
        _onChange();
        saveActive = false;
        loadActive = false;
        yesSelected = false;
        emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
        return;
    }
    if (loadActive && yesSelected) {
        // load
        load(hover_idx);
        _onReload();
        _onChange();
        saveActive = false;
        loadActive = false;
        yesSelected = false;
        emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
        return;
    }
    if ((saveActive || loadActive) && !yesSelected) {
        // cancel
        saveActive = false;
        loadActive = false;
        emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
        return;
    }

    // start a load
    loadActive = true;
    yesSelected = false;
    emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
}

void Patch::patchButtonLongClicked() {
    if (saveActive || loadActive) {
        // just cancel I guess
        yesSelected = false;
        saveActive = false;
        loadActive = false;
        emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
        return;
    }

    // start a save
    saveActive = true;
    yesSelected = false;
    emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
}

void Patch::patchButtonValueChanged(int n) {
    if((saveActive || loadActive) && n != patchButtonValue) {
        yesSelected = !yesSelected;
        emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
        patchButtonValue = n;
        return;
    }
    hover_idx = n;
    emit patchSavingState(current_idx, hover_idx, saveActive, loadActive, yesSelected);
    patchButtonValue = n;
//    qInfo("hello world");
}

const QString ModSourceNames[Patch::NUM_MOD_SOURCES] = {
    "MODSRC_VEL",
    "MODSRC_LFO0",
    "MODSRC_LFO1",
    "MODSRC_ENV0",
    "MODSRC_ENV1"
};
const QString ModSinkNames[Patch::NUM_MOD_SINKS] = {
    "MOD_OSC_DETUNE",
    "MOD_OSC_PITCH",
    "MOD_FILTER_CUTOFF",
    "MOD_FILTER_RESONANCE",
    "MOD_FILTER_AMP",
    "MOD_CHORUS_DEPTH",
    "MOD_STEREOPAN",
    "MOD_STEREOPHASE",
};

QJsonObject saveTimbre(const Patch::TimbreData &data) {
    QJsonObject obj;
    obj["osc_shape0"] = data.osc_shape0;
    obj["osc_shape1"] = data.osc_shape1;
    obj["osc_detune"] = data.osc_detune;
    obj["osc_octave"] = data.osc_octave;

    obj["voice_glide"] = data.voice_glide;
    obj["voice_chorus"] = data.voice_chorus;

    obj["filter_freq"] = data.filter_freq;
    obj["filter_resonance"] = data.filter_resonance;

    obj["mixer_amount0"] = data.mixer_amount0;
    obj["mixer_amount1"] = data.mixer_amount1;

    obj["env0_attack"] = data.env0_attack;
    obj["env0_decay"] = data.env0_decay;
    obj["env0_sustain"] = data.env0_sustain;
    obj["env0_release"] = data.env0_release;

    obj["env1_attack"] = data.env1_attack;
    obj["env1_decay"] = data.env1_decay;
    obj["env1_sustain"] = data.env1_sustain;
    obj["env1_release"] = data.env1_release;

    return obj;
}

void loadTimbre(const QJsonObject &obj, Patch::TimbreData &data) {
    data.osc_shape0 = obj["osc_shape0"].toDouble();
    data.osc_shape1 = obj["osc_shape1"].toDouble();
    data.osc_detune = obj["osc_detune"].toDouble();
    data.osc_octave = obj["osc_octave"].toInt();

    data.voice_glide = obj["voice_glide"].toDouble();
    data.voice_chorus = obj["voice_chorus"].toInt();

    data.filter_freq = obj["filter_freq"].toDouble();
    data.filter_resonance = obj["filter_resonance"].toDouble();

    data.mixer_amount0 = obj["mixer_amount0"].toDouble();
    data.mixer_amount1 = obj["mixer_amount1"].toDouble();

    data.env0_attack = obj["env0_attack"].toDouble();
    data.env0_decay = obj["env0_decay"].toDouble();
    data.env0_sustain = obj["env0_sustain"].toDouble();
    data.env0_release = obj["env0_release"].toDouble();

    data.env1_attack = obj["env1_attack"].toDouble();
    data.env1_decay = obj["env1_decay"].toDouble();
    data.env1_sustain = obj["env1_sustain"].toDouble();
    data.env1_release = obj["env1_release"].toDouble();
}

QJsonObject saveMatrix(qint8 mod_matrix[Patch::ModSource::NUM_MOD_SOURCES][Patch::ModSink::NUM_MOD_SINKS]) {
    QJsonObject obj;
    for (int i = 0; i < Patch::NUM_MOD_SOURCES; i++) {
        for (int j = 0; j < Patch::NUM_MOD_SINKS; j++) {
            obj[QString("mat_%1-%2").arg(ModSourceNames[i], ModSinkNames[j])] = mod_matrix[i][j];
        }
    }
    return obj;
}

void loadMatrix(const QJsonObject &obj, qint8 mod_matrix[Patch::ModSource::NUM_MOD_SOURCES][Patch::ModSink::NUM_MOD_SINKS]) {
    for (int i = 0; i < Patch::NUM_MOD_SOURCES; i++) {
        for (int j = 0; j < Patch::NUM_MOD_SINKS; j++) {
            mod_matrix[i][j] = obj[QString("mat_%1-%2").arg(ModSourceNames[i], ModSinkNames[j])].toDouble();
        }
    }
}

void Patch::save(uint8_t idx) {
//    QFile file(QString("%1/prog_%2.patch").arg(QString::number(idx)));
    QFile file(folder.absoluteFilePath(QString("prog_%1.patch").arg(QString::number(idx))));

    QJsonObject patch;
    patch["timbre0"] = saveTimbre(_p.timbre[0]);
    patch["timbre1"] = saveTimbre(_p.timbre[1]);

    patch["midi_arp_mode"] = _p.midi_arp_mode;
    patch["midi_arp_steps"] = _p.midi_arp_steps;
    patch["midi_chord_mode"] = _p.midi_chord_mode;
    patch["midi_key"] = _p.midi_key;

    patch["lfo0_freq"] = _p.lfo0_freq;
    patch["lfo0_shape"] = _p.lfo0_shape;
    patch["lfo1_freq"] = _p.lfo1_freq;
    patch["lfo1_shape"] = _p.lfo1_shape;

    patch["matrix"] = saveMatrix(_p.mod_matrix);

    QJsonDocument doc(patch);
    if (!file.exists()) {
        file.open(QFile::OpenModeFlag::NewOnly);
    } else {
        file.open(QFile::OpenModeFlag::WriteOnly);
    }
    file.write(doc.toJson());
    file.close();
    current_idx = idx;
}

void Patch::load(uint8_t idx) {
    QFile file(folder.absoluteFilePath(QString("prog_%1.patch").arg(QString::number(idx))));
    if (!file.exists()) {
        qInfo("loading from patch that does not exist yet. Initializing.");
        file.setFileName(folder.absoluteFilePath(QString("prog_0.patch")));
        if (!file.exists()) {
            qInfo("default patch prog_0.patch does not exist. Using zero-patch.");
            memset(&_p, 0, sizeof(_p));
            return;
        }
    }

    file.open(QFile::OpenModeFlag::ReadOnly);
    const QJsonObject &patch = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    qInfo(qUtf8Printable(QString("loading patch from file %1").arg(file.fileName())));

    loadTimbre(patch["timbre0"].toObject(), _p.timbre[0]);
    loadTimbre(patch["timbre1"].toObject(), _p.timbre[1]);

    _p.midi_arp_mode = Patch::ArpMode(patch["midi_arp_mode"].toInt());
    _p.midi_arp_steps = patch["midi_arp_steps"].toInt();
    _p.midi_chord_mode = Patch::ChordMode(patch["midi_chord_mode"].toInt());
    _p.midi_key = patch["midi_key"].toInt();

    _p.lfo0_freq = patch["lfo0_freq"].toDouble();
    _p.lfo0_shape = Patch::LfoShape(patch["lfo0_shape"].toInt());
    _p.lfo1_freq = patch["lfo1_freq"].toDouble();
    _p.lfo1_shape = Patch::LfoShape(patch["lfo1_shape"].toInt());

    loadMatrix(patch["matrix"].toObject(), _p.mod_matrix);
    current_idx = idx;
}
