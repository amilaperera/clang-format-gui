#include "ClangFormatCmdSet.h"
#include "ui_ClangFormatCmdSet.h"

ClangFormatCmdSet::ClangFormatCmdSet(const QFileInfoList &cmdList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClangFormatCmdSet)
{
    ui->setupUi(this);
    ui->okPushButton->setDefault(true);

    ui->gridLayout->addWidget(new QLabel(tr("The following clang-format binary "
                                            "file(s) are found in the system"), this),
                              0, 0);
    int i = 0;
    for (auto &cmd : cmdList) {
        QString versionStrForRButton;
        QString versionStr = Utility::GetClangFormatVersion(cmd);
        // if we can not retrieve version information,
        // we don't append them to the radio button command
        if (!versionStr.isEmpty()) {
            // add version information for this command
            versionStrForRButton = " (" + tr("version") + ": " + versionStr + ")";
        }
        QRadioButton *rButton = new QRadioButton(cmd.absoluteFilePath() +
                                                versionStrForRButton,
                                                this);
        rButton->setStyleSheet("font: italic");
        radioButtonList << rButton;
        if (i == 0) {
            // select the first one
            radioButtonList.at(i)->setChecked(true);
        }
        ui->gridLayout->addWidget(radioButtonList.at(i), 3 + i, 0);
        ++i;
    }
    shouldSaveSettings = false;
}

ClangFormatCmdSet::~ClangFormatCmdSet()
{
    delete ui;
}

bool ClangFormatCmdSet::GetSaveSettingsStatus()
{
    return shouldSaveSettings;
}

QString ClangFormatCmdSet::GetSelectedFormatCmd()
{
    QString cmd = "";
    for (auto const *rButton : radioButtonList) {
        if (rButton->isChecked()) {
            cmd = rButton->text();
            break;
        }
    }
    return cmd;
}

void ClangFormatCmdSet::on_checkBox_toggled(bool checked)
{
    shouldSaveSettings = checked;
}
