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
                setConfigKeyValPair(kvPair[0].trimmed(), kvPair[1].trimmed());
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

void CFConfiguration::setConfigKeyValPair(const QString &k, const QString &v)
{
    if (!k.isEmpty() && !v.isEmpty()) {
        config[k] = v;
    }
}
