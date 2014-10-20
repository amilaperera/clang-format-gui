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

    // this returns the number of removed duplicates and we're not interested
    // in its value
    (void) pathList.removeDuplicates();

    for (const auto &path : pathList) {
        QDir dir(path);
        QFileInfoList files = dir.entryInfoList(QStringList() << "clang-format*",
                                                QDir::Files | QDir::Executable);

        foreach (const QFileInfo &file, files) {
            // omit diff command binaries
            if (!(file.fileName().contains("diff", Qt::CaseInsensitive))) {
                clangFormatCmdList << file;
                qDebug() << "Command found : " << file.absoluteFilePath();
            }
        }
    }

    if (clangFormatCmdList.isEmpty()) {
        // clang-format is not found
        return false;
    }

    return true;
}

}
