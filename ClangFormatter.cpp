#include "ClangFormatter.h"

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

bool ClangFormatter::Execute(const QString &formatCmd)
{
    output.clear();

    QProcess clangFormatProc;

    clangFormatProc.start(formatCmd);

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
