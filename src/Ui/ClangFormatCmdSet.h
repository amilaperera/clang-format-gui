#ifndef CLANGFORMATCMDSET_H
#define CLANGFORMATCMDSET_H

#include <QDialog>
#include <QList>
#include <QFileInfo>
#include <QLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QFileDialog>
#include <QToolTip>

#include "Utility.h"

namespace Ui {
class ClangFormatCmdSet;
}

class ClangFormatCmdSet : public QDialog
{
    Q_OBJECT

public:
    explicit ClangFormatCmdSet(const QFileInfoList &cmdList, QWidget *parent = 0);
    ~ClangFormatCmdSet();
    bool GetSaveSettingsStatus();
    QString GetSelectedFormatCmd();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_manualSetRButton_toggled(bool checked);
    void on_cmdBrowseToolButton_clicked();
    void on_cmdPathLineEdit_textChanged(const QString &arg1);

private:
    typedef QList<QRadioButton *> QRadioButtonList;

    Ui::ClangFormatCmdSet *ui;
    QRadioButtonList radioButtonList;
    bool shouldSaveSettings;

private:
    void setTabOrderOfDialogBox();
    void setManualCmdSetGroup(bool status);
};

#endif // CLANGFORMATCMDSET_H
