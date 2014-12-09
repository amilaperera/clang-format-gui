#ifndef INDENTUICONTROL_H
#define INDENTUICONTROL_H

#include "DetailsUiControl.h"

class IndentUiControl : public DetailsUiControl
{
    Q_OBJECT
public:
    explicit IndentUiControl(QWidget *p, FormatOptions *f, QObject *parent = 0);
    ~IndentUiControl();

signals:

public slots:

private:
    QGridLayout *gridLayout;
    QCheckBox *indentCaseLabel;

private:
    void setupUi();
};

#endif // INDENTUICONTROL_H
