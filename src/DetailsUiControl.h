#ifndef DETAILSUICONTROL_H
#define DETAILSUICONTROL_H

#include <QWidget>
#include <QLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QMainWindow>
#include <QObject>

#include "FormatOptions.h"
#include "CFConfiguration.h"
#include <ui_Mainwindow.h>

class DetailsUiControl : public QObject
{
    Q_OBJECT
public:
    explicit DetailsUiControl(QWidget *p, FormatOptions *f,
                              QObject *parent = 0);
    ~DetailsUiControl();
    void SetMargins(QLayout *layout);

signals:
    void stylesUpdated();

public slots:

protected:
    QWidget *uiPage;
    FormatOptions *formatOptions;
    QWidget *layoutWidget;
};

#endif // DETAILSUICONTROL_H
