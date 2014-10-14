#include "ClangFormatter.h"
#include "FormatOptions.h"

#if defined(Q_OS_WIN)
    QString ClangFormatter::clangFormatCommand = "clang-format.exe";
#elif defined(Q_OS_LINUX)
    QString ClangFormatter::clangFormatCommand = "clang-format";
#else
    QString ClangFormatter::clangFormatCommand = "clang-format";
#endif

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
