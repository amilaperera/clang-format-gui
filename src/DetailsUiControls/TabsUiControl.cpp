#include "TabsUiControl.h"

TabsUiControl::TabsUiControl(QWidget *p, FormatOptions *f, QObject *parent) :
    DetailsUiControl(p, f, parent)
{
    setupUi();
    setupConnections();
    isSettingUiForCurrentConfig = false;
}

TabsUiControl::~TabsUiControl()
{
    delete gridLayout;
}

void TabsUiControl::setupUiForCurrentConfig(const CFConfiguration &c)
{
    isSettingUiForCurrentConfig = true;
    QMap<QString, QString> config = c.GetConfig();
    if (config.contains("UseTab")) {
        useTabsComboBox->setCurrentText(config["UseTab"]);
    }
    if (config.contains("TabWidth")) {
        tabWidthSpinBox->setValue(config["TabWidth"].toInt());
    }
}

void TabsUiControl::on_useTabsComboBox_currentIndexChanged(const QString &text)
{
    if (!isSettingUiForCurrentConfig)
    {
        if (text == "Never") {
            formatOptions->SetUseTab(FormatOptions::Never);
        } else if (text == "For indentation") {
            formatOptions->SetUseTab(FormatOptions::ForIndentation);
        } else if (text == "Always") {
            formatOptions->SetUseTab(FormatOptions::Always);
        }
        emit stylesUpdated();
    }
    isSettingUiForCurrentConfig = false;
}

void TabsUiControl::on_tabWidthSpinBox_valueChanged(int value)
{
    if (!isSettingUiForCurrentConfig)
    {
        if (value >= 2 && value <= 16) {
            formatOptions->SetTabWidth(value);
        }
        emit stylesUpdated();
    }
    isSettingUiForCurrentConfig = false;
}

void TabsUiControl::setupUi()
{
    gridLayout = new QGridLayout(layoutWidget);
    SetMargins(gridLayout);

    useTabsLbl = new QLabel(tr("Use Tabs: "), layoutWidget);
    useTabsComboBox = new QComboBox(layoutWidget);
    useTabsComboBox->clear();
    useTabsComboBox->insertItems(0, QStringList() <<
                                 tr("Never") <<
                                 tr("For Indentation") <<
                                 tr("Always"));

    tabWidthLbl = new QLabel(tr("Tab Width: "), layoutWidget);
    tabWidthSpinBox = new QSpinBox(layoutWidget);
    tabWidthSpinBox->setMinimum(2);
    tabWidthSpinBox->setMaximum(16);
    tabWidthSpinBox->setSingleStep(1);

    gridLayout->addWidget(useTabsLbl, 0, 0, 1, 1);
    gridLayout->addWidget(useTabsComboBox, 0, 1, 1, 1);
    gridLayout->addWidget(tabWidthLbl, 1, 0, 1, 1);
    gridLayout->addWidget(tabWidthSpinBox, 1, 1, 1, 1);
}

void TabsUiControl::setupConnections()
{
    connect(useTabsComboBox, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(on_useTabsComboBox_currentIndexChanged(const QString &)));
    connect(tabWidthSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(on_tabWidthSpinBox_valueChanged(int)));
}
