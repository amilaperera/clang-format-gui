#ifndef CFCONFIGURATION_H
#define CFCONFIGURATION_H

#include <QDebug>
#include <QMap>
#include <QStringList>
#include <QRegExp>
#include <QObject>

class CFConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit CFConfiguration(QObject *parent = 0);
    explicit CFConfiguration(const QString &configStr, QObject *parent = 0);
    void ToString() const;
    QMap<QString, QString> GetConfig() const;

signals:

public slots:

private:
    QMap<QString, QString> config;
};

#endif // CFCONFIGURATION_H
