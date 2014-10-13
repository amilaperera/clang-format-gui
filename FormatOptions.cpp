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
}

void FormatOptions::SetInputFile(const QString &in)
{
    inputFile = in;
}

void FormatOptions::SetStyle(Style s)
{
    style = s;
}

QString FormatOptions::GetClangFormatCommandStr() const
{
    QString clangFormatCommandStr;

    constructClangFormatCommandStr(clangFormatCommandStr);

    if (inputFile.isEmpty()) {
        // TODO: if no input files are set, we should throw exception
    } else {
        clangFormatCommandStr += (" \"" + inputFile + "\"");
    }

    qDebug() << "clang-format COMMAND: " << clangFormatCommandStr;

    return clangFormatCommandStr;
}

QString FormatOptions::getClangFormatCommandExe() const
{
    QString clangFormatExe = "";

#if defined(Q_OS_WIN)
    clangFormatExe += "clang-format.exe";
#elif defined(Q_OS_LINUX)
    clangFormatExe += "clang-format";
#else
    clangFormatExe += "clang-format";
#endif

    return clangFormatExe;
}

void FormatOptions::constructClangFormatCommandStr(QString &cmd) const
{
    cmd += getClangFormatCommandExe();
    setStyleInCommandStr(cmd);
}

void FormatOptions::setStyleInCommandStr(QString &cmd) const
{
    QString styleStr = " -style=";

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
        styleStr += "Webkit";
        break;
    default:
        styleStr += "LLVM";
    }

    cmd += styleStr;
}
