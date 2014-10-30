#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QProcess>
#include <QDebug>

namespace Utility
{
    bool FindClangFormatCommand(QFileInfoList &clangFormatCmdList);
    QString GetClangFormatVersion(const QFileInfo &clangFormatCmd);
}

#endif // UTILITY_H
