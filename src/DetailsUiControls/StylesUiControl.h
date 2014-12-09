#ifndef STYLESUICONTROL_H
#define STYLESUICONTROL_H

#include "DetailsUiControl.h"

class StylesUiControl : public DetailsUiControl
{
    Q_OBJECT
public:
    explicit StylesUiControl(QWidget *p, FormatOptions *f, QObject *parent = 0);
    ~StylesUiControl();

signals:

private slots:
    void on_llvmStyleRButton_toggled(bool checked);
    void on_googleStyleRButton_toggled(bool checked);
    void on_chromiumStyleRButton_toggled(bool checked);
    void on_mozillaStyleRButton_toggled(bool checked);
    void on_webkitStyleRButton_toggled(bool checked);

private:
    QRadioButton *llvmStyleRButton;
    QRadioButton *googleStyleRButton;
    QRadioButton *chromiumStyleRButton;
    QRadioButton *mozillaStyleRButton;
    QRadioButton *webkitStyleRButton;
    QVBoxLayout *vboxLayout;

private:
    void setupUi();
    void setupConnections();
    void changeStyleOnRButtonToggle(bool checked, FormatOptions::Style style);
};

#endif // STYLESUICONTROL_H
