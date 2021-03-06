#ifndef CLANGFORMATTER_H
#define CLANGFORMATTER_H

#include <QProcess>
#include <QDebug>
#include <QObject>

class ClangFormatter : public QObject
{
    Q_OBJECT
public:
    explicit ClangFormatter(QObject *parent = 0);
    static QString GetClangFormatCommand();
    static void SetClangFormatCommand(const QString &cmd);
    bool Execute(const QString &formatCmd);
    QString GetOutput();

signals:

public slots:

private:
    static QString clangFormatCommand;
    QString output;
};

#endif // CLANGFORMATTER_H
