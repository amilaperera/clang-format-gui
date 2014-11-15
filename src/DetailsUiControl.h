#ifndef DETAILSUICONTROL_H
#define DETAILSUICONTROL_H

#include <QWidget>
#include <QLayout>
#include <QRadioButton>
#include <QMainWindow>
#include <QObject>

#include "FormatOptions.h"
#include <ui_Mainwindow.h>

class DetailsUiControl : public QObject
{
    Q_OBJECT
public:
    explicit DetailsUiControl(QObject *parent = 0);
    ~DetailsUiControl();
    void SetMargins(QLayout *layout);

signals:

public slots:

};

#endif // DETAILSUICONTROL_H
