#ifndef FORMATOPTIONS_H
#define FORMATOPTIONS_H

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
    void SetStyle(Style s);
    Style GetStyle() const;

signals:

public slots:

private:
    Style style;
};

#endif // FORMATOPTIONS_H
