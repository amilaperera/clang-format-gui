#include "ClangFormatter.h"

ClangFormatter::ClangFormatter(const ClangFormatCommand *c, QObject *parent) :
    QObject(parent), cmd(c)
{
    qDebug() << "clang-format command: " << cmd->GetClangFormatCommand();
}

bool ClangFormatter::Execute()
{
    output.clear();

    QProcess clangFormatProc;
    clangFormatProc.setProcessChannelMode(QProcess::SeparateChannels);

    clangFormatProc.start(cmd->GetClangFormatCommand());
    if (!clangFormatProc.waitForStarted()) {
        return false;
    }

    if (!clangFormatProc.waitForFinished()) {
        return false;
    }

    output = clangFormatProc.readAllStandardOutput();

    // FIXME: For some f*cking reason, output is doubled when the stdout
    // is read.
    // As a temporary fix, we read only the first half of the output.
    // But there's no guarantee that this would work for other platforms as well
    // Currently only Linux(Fedora 20) is tested.
    output = output.left((output.length() / 2) - 1);

    return true;
}

QString ClangFormatter::GetOutput()
{
    return output;
}
