#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMainWindow>
#include <QDebug>
#include <QFont>
#include <QTreeWidget>
#include <QSettings>
#include <QFileInfoList>
#include <QMessageBox>
#include <QProgressBar>
#include <QThread>
#include <QMovie>
#include <QLabel>
#include <QScrollBar>
#include <Qsci/qsciscintilla.h>

#include "SrcFilePreviewer.h"
#include "SrcUpdater.h"
#include "ClangFormatter.h"
#include "FormatOptions.h"
#include "Utility.h"
#include "ClangFormatCmdSet.h"
#include "Settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool PreCheck();
    bool ExecClangFormatCmdSetDialog(const QFileInfoList &cmdList,
                                     QSettings &settings);

private slots:
    void on_openOriginalSrcToolButton_clicked();
    void on_srcPreviewTabWidget_currentChanged(int index);
    void on_llvmStyleRButton_toggled(bool checked);
    void on_googleStyleRButton_toggled(bool checked);
    void on_chromiumStyleRButton_toggled(bool checked);
    void on_mozillaStyleRButton_toggled(bool checked);
    void on_webkitStyleRButton_toggled(bool checked);

    void on_useTabsComboBox_currentIndexChanged(const QString &arg1);

    void on_tabWidthSpinBox_valueChanged(int arg1);

    void onOriginalSrcEditLinesChanged();
    void onFormattedSrcEditLinesChanged();

    void onSrcUpdaterStarted();
    void onSrcUpdaterOutputReady(const QString &cmd);

signals:

private:
    const QString organization = "clang-format-gui";
    const QString application = "clang-format-gui";

    QMovie *progressAnimation;
    QLabel *progressLabel;
    QsciScintilla *originalSrcTextEdit;
    QsciScintilla *formattedSrcTextEdit;
    QScrollBar *origSrcTextEditVSB;
    QScrollBar *formattedSrcTextEditVSB;
    int origSrcTextEditLastVSBPos;
    int formattedSrcTextEditLastVSBPos;
    SrcFilePreviewer *originalSrcPreviewer;
    SrcFilePreviewer *formattedSrcPreviewer;
    Ui::MainWindow *ui;
    QString defaultFileOpenDir;
    FormatOptions *formatOptions;

    bool userActionTriggered;
    bool newOrigSrcLoaded;

private:
    void setupProgressAnimation();
    void setupTextEditWidgets();
    bool readSettings();
    void setTextEditProperties(QsciScintilla *textEdit);
    void setInitialSplitSizes();
    void changeToOriginalSrcTab();
    void changeTabAndResetScrollPos();
    void initializeFormatOptionsWidget();
    void updateFormattedSrc();
    void updateFormattedSrcByUserAction();
    void updateUiControls();
    void changeStyleOnRButtonToggle(FormatOptions::Style style);
    void onLinesChanged(QsciScintilla *textEdit);
    void storeVSBPos();
};

#endif // MAINWINDOW_H
