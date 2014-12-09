#include "IndentUiControl.h"

IndentUiControl::IndentUiControl(QWidget *p, FormatOptions *f,
                                 QObject *parent) :
    DetailsUiControl(p, f, parent)
{
    setupUi();
}

IndentUiControl::~IndentUiControl()
{
    delete gridLayout;
}

void IndentUiControl::setupUi()
{
    gridLayout = new QGridLayout(layoutWidget);
    SetMargins(gridLayout);

    indentCaseLabel = new QCheckBox(tr("Indent case labels one level "
                                       "from switch statement"), layoutWidget);

    gridLayout->addWidget(indentCaseLabel, 0, 0, 1, 1);
}
