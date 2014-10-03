#ifndef FORMATOPTIONSSTANDARDITEMMODEL_H
#define FORMATOPTIONSSTANDARDITEMMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class FormatOptionsStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit FormatOptionsStandardItemModel(QObject *parent = 0);
    virtual ~FormatOptionsStandardItemModel();

signals:

public slots:

private:
    QStandardItem *rootNode;

    QStandardItem *styleNode;
    QStandardItem *dummy1;
    QStandardItem *dummy2;
};

#endif // FORMATOPTIONSSTANDARDITEMMODEL_H
