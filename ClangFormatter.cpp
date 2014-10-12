#include "ClangFormatter.h"

ClangFormatter::ClangFormatter(QObject *parent) : QObject(parent)
{
}

bool ClangFormatter::Execute(const ClangFormatCommand &clangFormatCommand)
{
    output.clear();

    QProcess clangFormatProc;

    clangFormatProc.start(clangFormatCommand.GetClangFormatCommand());

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
