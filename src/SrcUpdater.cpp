#include "SrcUpdater.h"

SrcUpdater::SrcUpdater(const QString &cmd, QObject *parent) :
    QObject(parent)
{
    cmdStr = cmd;
}

SrcUpdater::~SrcUpdater()
{

}

void SrcUpdater::start()
{
    ClangFormatter clangFormatter;
    if (clangFormatter.Execute(cmdStr)) {
        qDebug() << "clangFormatter process executed successfully";
    } else {
        qDebug() << "clangFormatter process execution failed";
    }

    emit outputReady(clangFormatter.GetOutput());
}
