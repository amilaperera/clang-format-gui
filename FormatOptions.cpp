#include "FormatOptions.h"

FormatOptions::FormatOptions(QObject *parent) :
    QObject(parent)
{
    Clear();
}

void FormatOptions::Clear()
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
