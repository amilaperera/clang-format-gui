#ifndef CLANGFORMATTER_H
#define CLANGFORMATTER_H

#include <QProcess>
#include <QDebug>
#include <QObject>

#include "ClangFormatCommand.h"

class ClangFormatter : public QObject
{
    Q_OBJECT
public:
    explicit ClangFormatter(const ClangFormatCommand *c, QObject *parent = 0);
    bool Execute();
    QString GetOutput();

signals:

public slots:

private:
    QString output;
    const ClangFormatCommand *cmd;
};

#endif // CLANGFORMATTER_H
