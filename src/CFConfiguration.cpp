#include "CFConfiguration.h"

CFConfiguration::CFConfiguration(QObject *parent) :
    QObject(parent)
{
}

CFConfiguration::CFConfiguration(const QString &configStr, QObject *parent) :
    QObject(parent)
{
    if (!configStr.isEmpty()) {
        QStringList configStrList = configStr.split(QRegExp("[\r\n]"),
                                                    QString::SkipEmptyParts);

        for (int i = 0; i < configStrList.size(); ++i) {
            QStringList kvPair = configStrList[i].split(':',
                                                        QString::SkipEmptyParts);
            if (kvPair.size() == 2) {
                QString key = kvPair[0].trimmed();
                QString val = kvPair[1].trimmed();
                if (!key.isEmpty() && !val.isEmpty()) {
                    config[key] = val;
                }
            }
        }
    }
}

void CFConfiguration::ToString() const
{
    if (!config.isEmpty()) {
        QMap<QString, QString>::const_iterator i = config.constBegin();
        for (; i!= config.constEnd(); ++i) {
            qDebug() << i.key() << " ==> " << i.value();
        }
    }
}

QMap<QString, QString> CFConfiguration::GetConfig() const
{
    return config;
}
