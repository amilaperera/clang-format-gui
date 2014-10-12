#include "FormatOptions.h"

FormatOptions::FormatOptions(QObject *parent) :
    QObject(parent)
{
    style = LLVM;
}

void FormatOptions::SetStyle(Style s)
{
    style = s;
}

FormatOptions::Style FormatOptions::GetStyle() const
{
    return style;
}
