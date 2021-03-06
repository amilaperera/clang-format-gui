#include "FormatOptions.h"

FormatOptions::FormatOptions(QObject *parent) :
    QObject(parent)
{
    Reset();
}

void FormatOptions::Reset()
{
    inputFile = "";
    style = LLVM;
    useTab = Never;
    tabWidth = 4;
}

void FormatOptions::SetInputFile(const QString &in)
{
    inputFile = in;
}

void FormatOptions::SetStyle(Style s)
{
    style = s;
}

void FormatOptions::SetUseTab(FormatOptions::UseTab ut)
{
    useTab = ut;
}

void FormatOptions::SetTabWidth(int tw)
{
    tabWidth = tw;
}

QString FormatOptions::GetClangFormatDumpCommandStr()
{
    QString clangFormatDumpCommandStr;
    constructClangFormatCommandStr(clangFormatDumpCommandStr);

    return (clangFormatDumpCommandStr + " -dump-config");
}

QString FormatOptions::GetClangFormatCommandStr() const
{
    QString clangFormatCommandStr;
    constructClangFormatCommandStr(clangFormatCommandStr);

    if (!inputFile.isEmpty()) {
        clangFormatCommandStr += (" \"" + inputFile + "\"");
    }

    qDebug() << "clang-format COMMAND: " << clangFormatCommandStr;

    return clangFormatCommandStr;
}

void FormatOptions::constructClangFormatCommandStr(QString &cmd) const
{
    cmd += ClangFormatter::GetClangFormatCommand();

    cmd += " -style=\"{"; // starting configuration options

    setStyleInCommandStr(cmd);
    setUseTabInCommandStr(cmd);
    setTabWidthCommandStr(cmd);

    cmd += "}\""; // ending configuration options
}

void FormatOptions::addFormatOptionsSeparator(QString &cmd) const
{
    cmd += ", ";
}

void FormatOptions::setStyleInCommandStr(QString &cmd) const
{
    QString styleStr = "BasedOnStyle: \"";

    switch(style) {
    case LLVM:
        styleStr += "LLVM";
        break;
    case Google:
        styleStr += "Google";
        break;
    case Chromium:
        styleStr += "Chromium";
        break;
    case Mozilla:
        styleStr += "Mozilla";
        break;
    case Webkit:
        styleStr += "WebKit";
        break;
    default:
        styleStr += "LLVM";
    }

    cmd += (styleStr + "\"");
    addFormatOptionsSeparator(cmd);
}

void FormatOptions::setUseTabInCommandStr(QString &cmd) const
{
    QString useTabStr = "UseTab: ";

    switch(useTab) {
    case Never:
        useTabStr += "Never";
        break;
    case ForIndentation:
        useTabStr += "ForIndentation";
        break;
    case Always:
        useTabStr += "Always";
        break;
    default:
        useTabStr += Never;
    }

    cmd += useTabStr;
    addFormatOptionsSeparator(cmd);
}

void FormatOptions::setTabWidthCommandStr(QString &cmd) const
{
    cmd += "TabWidth: " + QString::number(tabWidth);
    addFormatOptionsSeparator(cmd);
}
