#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>
#include <patch.h>
#include <voice.h>

class Synthesizer : public QObject
{
    Q_OBJECT
public:
    explicit Synthesizer(QObject *parent = nullptr);

signals:

private:
    Voice v0;
    Voice v1;

};

#endif // SYNTHESIZER_H
