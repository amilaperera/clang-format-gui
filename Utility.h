#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>

class Utility : public QObject
{
    Q_OBJECT
public:
    explicit Utility(QObject *parent = 0);
    static bool doesClangFormatExist();

signals:

public slots:

};

#endif // UTILITY_H
