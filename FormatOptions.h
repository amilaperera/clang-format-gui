#ifndef FORMATOPTIONS_H
#define FORMATOPTIONS_H

#include <QDebug>
#include <QObject>

class FormatOptions : public QObject
{
    Q_OBJECT
public:
    enum Style {
        LLVM,
        Google,
        Chromium,
        Mozilla,
        Webkit
    };

    explicit FormatOptions(QObject *parent = 0);
    void Reset();
    void SetInputFile(const QString &in);
    void SetStyle(Style s);
    QString GetClangFormatCommandStr() const;

signals:

public slots:

private:
    QString inputFile;
    Style style;

private:
    QString getClangFormatCommandExe() const;
    void constructClangFormatCommandStr(QString &cmd) const;
    void setStyleInCommandStr(QString &cmd) const;
};

#endif // FORMATOPTIONS_H
