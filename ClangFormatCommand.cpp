#include "ClangFormatCommand.h"

#ifdef Q_WS_WIN
const QString ClangFormatCommand::clangFormatCmd = "clang-format.exe";
#else
const QString ClangFormatCommand::clangFormatCmd = "clang-format";
#endif

ClangFormatCommand::ClangFormatCommand(QObject *parent) :
    QObject(parent)
{
    command += clangFormatCmd;
}

ClangFormatCommand::ClangFormatCommand(const QString &f, QObject *parent) :
    QObject(parent)
{
    command += clangFormatCmd;
    inputFile = f;
}

void ClangFormatCommand::SetFile(const QString &in)
{
    inputFile = in;
}

void ClangFormatCommand::SetStyle(const QString &style)
{
    command += (" -style=" + style);
}

void ClangFormatCommand::SetOutputFile(const QString &of)
{
    outputFile = of;
}

QString ClangFormatCommand::GetClangFormatCommand() const
{
    command += (" \"" + inputFile + "\"");
    if (!outputFile.isEmpty()) {
        command += ("> \"" + outputFile + "\"");
    }
    return command;
}
