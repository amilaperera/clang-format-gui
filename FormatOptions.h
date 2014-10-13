#ifndef FORMATOPTIONS_H
#define FORMATOPTIONS_H

#include <QObject>
#include "ClangFormatCommand.h"

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
    void Clear();
    void SetStyle(Style s);

    ClangFormatCommand *GetClangFormatCommand() const;
    QString GetClangFormatCommandStr() const;

signals:

public slots:

private:
    ClangFormatCommand *clangFormatCommand;
    Style style;
};

#endif // FORMATOPTIONS_H
