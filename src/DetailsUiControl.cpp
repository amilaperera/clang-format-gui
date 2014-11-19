#include "DetailsUiControl.h"

DetailsUiControl::DetailsUiControl(QWidget *p, FormatOptions *f,
                                   QObject *parent) :
    QObject(parent)
{
    uiPage = p;
    formatOptions = f;
    layoutWidget = new QWidget(uiPage);
}

DetailsUiControl::~DetailsUiControl()
{
    delete layoutWidget;
}

void DetailsUiControl::SetMargins(QLayout *layout)
{
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);
}
