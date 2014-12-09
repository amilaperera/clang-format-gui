#ifndef TABSUICONTROL_H
#define TABSUICONTROL_H

#include "DetailsUiControl.h"

class TabsUiControl : public DetailsUiControl
{
    Q_OBJECT
public:
    explicit TabsUiControl(QWidget *p, FormatOptions *f, QObject *parent = 0);
    ~TabsUiControl();
    void setupUiForCurrentConfig(const CFConfiguration &c);

signals:

private slots:
    void on_useTabsComboBox_currentIndexChanged(const QString &text);
    void on_tabWidthSpinBox_valueChanged(int value);

private:
    QLabel *useTabsLbl;
    QComboBox *useTabsComboBox;
    QLabel *tabWidthLbl;
    QSpinBox *tabWidthSpinBox;
    QGridLayout *gridLayout;

    bool isSettingUiForCurrentConfig;

private:
    void setupUi();
    void setupConnections();
};

#endif // TABSUICONTROL_H
