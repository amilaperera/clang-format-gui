#include "DetailsUiControl.h"

DetailsUiControl::DetailsUiControl(QObject *parent) :
    QObject(parent)
{
}

DetailsUiControl::~DetailsUiControl()
{

}

void DetailsUiControl::SetMargins(QLayout *layout)
{
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);
}
