#ifndef FORMATOPTIONS_H
#define FORMATOPTIONS_H

#include <QDebug>
#include <QObject>
#include "ClangFormatter.h"

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

    enum UseTab {
        Never,
        ForIndentation,
        Always
    };

    explicit FormatOptions(QObject *parent = 0);
    void Reset();
    void SetInputFile(const QString &in);
    void SetStyle(Style s);
    void SetUseTab(UseTab ut);
    QString GetClangFormatCommandStr() const;

signals:

public slots:

private:
    QString inputFile;
    Style style;
    UseTab useTab;

private:
    void addFormatOptionsSeparator(QString &cmd) const;
    void constructClangFormatCommandStr(QString &cmd) const;
    void setStyleInCommandStr(QString &cmd) const;
    void setUseTabInCommandStr(QString &cmd) const;
};

#endif // FORMATOPTIONS_H
