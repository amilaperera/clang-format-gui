#ifndef SRCUPDATER_H
#define SRCUPDATER_H

#include <QObject>

#include "ClangFormatter.h"

class SrcUpdater : public QObject
{
    Q_OBJECT
public:
    explicit SrcUpdater(const QString &cmd, QObject *parent = 0);
    ~SrcUpdater();

signals:
    void outputReady(const QString &cmd);

public slots:
    void start();

private:
    // this is a copy of the command string
    QString cmdStr;
};

#endif // SRCUPDATER_H
