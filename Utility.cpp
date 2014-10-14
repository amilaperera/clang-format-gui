#include "Utility.h"

namespace Utility
{
static bool findClangFormatCommandLinux(QFileInfoList &clangFormatCmdList);

bool FindClangFormatCommand(QFileInfoList &clangFormatCmdList)
{
    bool ret = false;
#if defined(Q_OS_WIN)
#elif defined(Q_OS_LINUX)
    ret = findClangFormatCommandLinux(clangFormatCmdList);
#else
#endif
    return ret;
}

static bool findClangFormatCommandLinux(QFileInfoList &clangFormatCmdList)
{
    QStringList pathList = QString(qgetenv("PATH")).split(":", QString::SkipEmptyParts);

    for (const auto &path : pathList) {
        QDir dir(path);
        QFileInfoList files = dir.entryInfoList(QStringList() << "clang-format*",
                                                  QDir::Files | QDir::Executable);
        clangFormatCmdList << files;
    }

    for (const auto &cmd : clangFormatCmdList) {
        qDebug() << "Command Found: " << cmd.absoluteFilePath();
    }

    if (clangFormatCmdList.isEmpty()) {
        // clang-format is not found
        return false;
    }

    return true;
}

}
