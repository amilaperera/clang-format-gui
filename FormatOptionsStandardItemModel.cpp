#include "FormatOptionsStandardItemModel.h"

FormatOptionsStandardItemModel::FormatOptionsStandardItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
    rootNode = new QStandardItem("Format Options");
    styleNode = new QStandardItem("Style");
    dummy1 = new QStandardItem("Dummy");
    dummy2 = new QStandardItem("Dummy");

    // build up the hierachy
    appendRow(rootNode);
    rootNode->appendRow(styleNode);
    styleNode->appendRow(dummy1);
    rootNode->appendRow(dummy2);
}

FormatOptionsStandardItemModel::~FormatOptionsStandardItemModel()
{
    delete dummy1;
    delete styleNode;
    delete dummy2;
    delete rootNode;
}
