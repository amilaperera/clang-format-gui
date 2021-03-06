#include "ClangFormatCmdSet.h"
#include "ui_ClangFormatCmdSet.h"

ClangFormatCmdSet::ClangFormatCmdSet(const QFileInfoList &cmdList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClangFormatCmdSet)
{
    ui->setupUi(this);
    ui->okPushButton->setDefault(true);

    if (cmdList.size() > 0) {
        // if there are clang-format executables install in the system path
        if (cmdList.size() == 1) {
            ui->gridLayout->addWidget(new QLabel(tr("The following clang-format executable "
                                                    "is found in the system path"), this),
                                      0, 0);
        } else {
            ui->gridLayout->addWidget(new QLabel(tr("The following clang-format executables "
                                                    "are found in the system path"), this),
                                      0, 0);
        }

        for (int i = 0; i < cmdList.size(); ++i) {
            QFileInfo cmd = cmdList.at(i);
            QString versionStrForRButton;
            QString versionStr = Utility::GetClangFormatVersion(cmd);

            // If we can not retrieve version information,
            // we don't append them to the radio button command
            if (!versionStr.isEmpty()) {
                // add version information for this command
                versionStrForRButton = " (" + tr("version") + ": " + versionStr + ")";
            }

            QRadioButton *rButton = nullptr;
#if defined(Q_OS_WIN)
            // For windows, add separate string when we display the
            // default clang-format command bundled with the application.
            if (cmd.absolutePath() == QCoreApplication::applicationDirPath()) {
                rButton = new QRadioButton(cmd.absoluteFilePath() +
                                           versionStrForRButton +
                                           " (default executable installed with clang-format-gui)",
                                            this);
            } else {
#endif
                rButton = new QRadioButton(cmd.absoluteFilePath() + versionStrForRButton,
                                            this);
#if defined(Q_OS_WIN)
            }
#endif
            rButton->setStyleSheet("font: italic");
            radioButtonList << rButton;
            if (i == 0) {
                // select the first one
                radioButtonList.at(i)->setChecked(true);
            }
            ui->gridLayout->addWidget(radioButtonList.at(i), 3 + i, 0);
        }
    } else {
        // if no clang-format executables are found in the system path
        ui->gridLayout->addWidget(new QLabel(tr("No clang-format executables are found in the system path"), this),
                                  0, 0);
    }

    shouldSaveSettings = false;
    setManualCmdSetGroup(false);
    ui->okPushButton->setFocus();
    setTabOrderOfDialogBox();
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
            cmd = rButton->text().split("(").at(0).trimmed();
            break;
        }
    }
    return cmd;
}

void ClangFormatCmdSet::on_checkBox_toggled(bool checked)
{
    shouldSaveSettings = checked;
}

void ClangFormatCmdSet::setTabOrderOfDialogBox()
{
    for (int i = 0; i < radioButtonList.size(); ++i) {
        if (i == (radioButtonList.size() - 1)) {
            // if the current radio button is the last
            setTabOrder(radioButtonList.at(i), ui->manualSetRButton);
        } else {
            setTabOrder(radioButtonList.at(i), radioButtonList.at(i + 1));
        }
    }
}

void ClangFormatCmdSet::setManualCmdSetGroup(bool status)
{
    ui->cmdPathLineEdit->clear();
    ui->cmdPathLineEdit->setEnabled(status);
    ui->cmdBrowseToolButton->setEnabled(status);
}

void ClangFormatCmdSet::on_manualSetRButton_toggled(bool checked)
{
    if (checked) {
        setManualCmdSetGroup(true);
        ui->cmdPathLineEdit->setFocus();
    } else {
        setManualCmdSetGroup(false);
    }
}

void ClangFormatCmdSet::on_cmdBrowseToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Set clang-format executable path"),
                                                    QDir::homePath());

    if (!fileName.isEmpty() && QFileInfo(fileName).exists()) {
        ui->cmdPathLineEdit->setText(fileName);
    }
}

void ClangFormatCmdSet::on_cmdPathLineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->cmdPathLineEdit->setStyleSheet("QWidget {color: black;} QWidget QTooltip {color: black;}");
        ui->cmdPathLineEdit->setToolTip("");
    } else {
        QFileInfo fileName(arg1);
        if (fileName.exists() && fileName.isFile() && fileName.isExecutable()) {
            // set the widget's font color and tool tip color to normal
            ui->cmdPathLineEdit->setStyleSheet("QWidget {color: black;} QWidget QTooltip {color: black;}");
            ui->cmdPathLineEdit->setToolTip("");
        } else {
            // set the widget's font color to red
            ui->cmdPathLineEdit->setStyleSheet("QWidget {color: red;} QWidget QTooltip {color: black;}");
            if (!fileName.isExecutable()) {
                ui->cmdPathLineEdit->setToolTip(tr("File is not executable"));
            } else {
                ui->cmdPathLineEdit->setToolTip(tr("File doesn't exist"));
            }
        }
    }
}
