#include "Utility.h"

namespace Utility
{
static bool findClangFormatCommandLinux(QFileInfoList &clangFormatCmdList);

bool FindClangFormatCommand(QFileInfoList &clangFormatCmdList)
{
    bool ret = false;
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    ret = findClangFormatCommandLinux(clangFormatCmdList);
#else
#endif
    return ret;
}

static bool findClangFormatCommandLinux(QFileInfoList &clangFormatCmdList)
{
#if defined(Q_OS_WIN)
    QStringList pathList = QString(qgetenv("PATH")).split(";",
                                                          QString::SkipEmptyParts);
    // In Windows we search the application directory too, in addition to
    // the PATH variable.
    // NOTE: We hope to bundle clang-format with the setup and therefore
    // usually clang-format exists inside the application directory.
    pathList.prepend(QCoreApplication::applicationDirPath());
#elif defined(Q_OS_LINUX)
    QStringList pathList = QString(qgetenv("PATH")).split(":",
                                                          QString::SkipEmptyParts);
#endif

    if (pathList.empty()) {
        qDebug() << "PATH variable is not set";
        return false;
    }

    // removeDuplicates() returns the number of removed duplicates
    // but we're not interested in its value
    (void) pathList.removeDuplicates();

    for (const auto &path : pathList) {
        QDir dir(path);
        QFileInfoList files = dir.entryInfoList(QStringList() << "clang-format*",
                                                QDir::Files | QDir::Executable);

        foreach (const QFileInfo &file, files) {
            // omit clang-format-diff and clang-format-gui command binaries
            // WARNING: not omitting clang-format-gui will create an infinite
            // recursion, since this is used as a process to extract the version
            // information.
            if (!(file.fileName().contains("diff", Qt::CaseInsensitive)) &&
                !(file.fileName().contains("gui", Qt::CaseInsensitive))) {
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

QString GetClangFormatVersion(const QFileInfo &clangFormatCmd)
{
    QProcess cmd;
    cmd.setProcessChannelMode(QProcess::MergedChannels);
    cmd.start(clangFormatCmd.absoluteFilePath() + " -version");
    if (!cmd.waitForStarted()) {
        return "";
    }

    if (!cmd.waitForFinished()) {
        return "";
    }

    QString output = cmd.readAll();
    QString version;

    QRegularExpression re("llvm version\\D*(\\d+(\\D\\d+)?)",
                          QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(output);
    if (match.hasMatch()) {
        version = match.captured(1);
    }

    if (!version.isEmpty()) {
        qDebug() << "version of " << clangFormatCmd.absoluteFilePath() <<
                    " is " << version;
    } else {
        qDebug() << "Could not retrieve the clang-format version information";
    }

    return version;
}

}
