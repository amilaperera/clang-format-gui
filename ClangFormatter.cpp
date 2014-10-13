#include "ClangFormatter.h"

ClangFormatter::ClangFormatter(QObject *parent) : QObject(parent)
{
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
