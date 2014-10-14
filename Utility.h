#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QObject>
#include <QFile>
#include <QDir>
#include "ClangFormatter.h"

namespace Utility
{
    bool FindClangFormatCommand(QFileInfoList &clangFormatCmdList);
}

#endif // UTILITY_H
