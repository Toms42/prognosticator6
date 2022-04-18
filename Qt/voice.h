#ifndef VOICE_H
#define VOICE_H

#include <QObject>

class Voice : public QObject
{
    Q_OBJECT
public:
    explicit Voice(QObject *parent = nullptr);

signals:

};

#endif // VOICE_H
