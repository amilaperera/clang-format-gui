#include "ClangFormatter.h"
#include "FormatOptions.h"

QString ClangFormatter::clangFormatCommand = "";

ClangFormatter::ClangFormatter(QObject *parent) : QObject(parent)
{
}

QString ClangFormatter::GetClangFormatCommand()
{
    return clangFormatCommand;
}

void ClangFormatter::SetClangFormatCommand(const QString &cmd)
{
    clangFormatCommand = cmd;
}

bool ClangFormatter::Execute(const FormatOptions *formatOptions)
{
    output.clear();

    QProcess clangFormatProc;

    clangFormatProc.start(formatOptions->GetClangFormatCommandStr());

    if (!clangFormatProc.waitForStarted()) {
        return false;
    }

    if (!clangFormatProc.waitForFinished()) {
        return false;
    }

    output = clangFormatProc.readAllStandardOutput();

    return true;
}

QString ClangFormatter::GetOutput()
{
    return output;
}
