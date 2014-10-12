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

ClangFormatCommand::ClangFormatCommand(const QString &in, QObject *parent) :
    QObject(parent)
{
    command += clangFormatCmd;
    inputFile = in;
}

void ClangFormatCommand::SetInputFile(const QString &in)
{
    inputFile = in;
}

void ClangFormatCommand::SetOutputFile(const QString &of)
{
    outputFile = of;
}

void ClangFormatCommand::SetStyle(const QString &style)
{
    command += (" -style=" + style);
}

QString ClangFormatCommand::GetClangFormatCommand() const
{
    command += (" \"" + inputFile + "\"");

    // if outputFile is set to a non-empty file
    // redirect output to the output file
    if (!outputFile.isEmpty()) {
        command += ("> \"" + outputFile + "\"");
    }

    return command;
}
