#ifndef CLANGFORMATTER_H
#define CLANGFORMATTER_H

#include <QProcess>
#include <QDebug>
#include <QObject>

#include "FormatOptions.h"

class ClangFormatter : public QObject
{
    Q_OBJECT
public:
    explicit ClangFormatter(QObject *parent = 0);
    bool Execute(const FormatOptions *formatOptions);
    QString GetOutput();

signals:

public slots:

private:
    QString output;
};

#endif // CLANGFORMATTER_H
