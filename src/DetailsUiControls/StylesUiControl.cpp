#include "StylesUiControl.h"

StylesUiControl::StylesUiControl(QWidget *p, FormatOptions *f,
                                 QObject *parent) :
    DetailsUiControl(p, f, parent)
{
    setupUi();
    setupConnections();
}

StylesUiControl::~StylesUiControl()
{
    delete vboxLayout;
}

void StylesUiControl::setupUi()
{
    vboxLayout = new QVBoxLayout(layoutWidget);
    SetMargins(vboxLayout);

    llvmStyleRButton = new QRadioButton(tr("LLVM coding standard"),
                                        layoutWidget);
    googleStyleRButton = new QRadioButton(tr("Google's C++ style guide"),
                                          layoutWidget);
    chromiumStyleRButton = new QRadioButton(tr("Chromium's style guide"),
                                            layoutWidget);
    mozillaStyleRButton = new QRadioButton(tr("Mozilla's style guide"),
                                           layoutWidget);
    webkitStyleRButton = new QRadioButton(tr("Webkit's style guide"),
                                          layoutWidget);
    vboxLayout->addWidget(llvmStyleRButton);
    vboxLayout->addWidget(googleStyleRButton);
    vboxLayout->addWidget(chromiumStyleRButton);
    vboxLayout->addWidget(mozillaStyleRButton);
    vboxLayout->addWidget(webkitStyleRButton);
}

void StylesUiControl::setupConnections()
{
    connect(llvmStyleRButton, SIGNAL(toggled(bool)),
            this, SLOT(on_llvmStyleRButton_toggled(bool)));
    connect(googleStyleRButton, SIGNAL(toggled(bool)),
            this, SLOT(on_googleStyleRButton_toggled(bool)));
    connect(chromiumStyleRButton, SIGNAL(toggled(bool)),
            this, SLOT(on_chromiumStyleRButton_toggled(bool)));
    connect(mozillaStyleRButton, SIGNAL(toggled(bool)),
            this, SLOT(on_mozillaStyleRButton_toggled(bool)));
    connect(webkitStyleRButton, SIGNAL(toggled(bool)),
            this, SLOT(on_webkitStyleRButton_toggled(bool)));
}

void StylesUiControl::changeStyleOnRButtonToggle(bool checked,
                                                 FormatOptions::Style style)
{
    if (checked) {
        formatOptions->SetStyle(style);
        emit stylesUpdated();
    }
}

void StylesUiControl::on_llvmStyleRButton_toggled(bool checked)
{
    changeStyleOnRButtonToggle(checked, FormatOptions::LLVM);
}

void StylesUiControl::on_googleStyleRButton_toggled(bool checked)
{
    changeStyleOnRButtonToggle(checked, FormatOptions::Google);
}

void StylesUiControl::on_chromiumStyleRButton_toggled(bool checked)
{
    changeStyleOnRButtonToggle(checked, FormatOptions::Chromium);
}

void StylesUiControl::on_mozillaStyleRButton_toggled(bool checked)
{
    changeStyleOnRButtonToggle(checked, FormatOptions::Mozilla);
}

void StylesUiControl::on_webkitStyleRButton_toggled(bool checked)
{
    changeStyleOnRButtonToggle(checked, FormatOptions::Webkit);
}
