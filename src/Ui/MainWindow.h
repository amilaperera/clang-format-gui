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
#include <QDesktopWidget>
#include <Qsci/qsciscintilla.h>

#include "SrcFilePreviewer.h"
#include "SrcUpdater.h"
#include "ClangFormatter.h"
#include "FormatOptions.h"
#include "Utility.h"
#include "ClangFormatCmdSet.h"
#include "Settings.h"
#include "CFConfiguration.h"
#include "DetailsUiControl.h"
#include "DetailsUiControls/StylesUiControl.h"
#include "DetailsUiControls/TabsUiControl.h"

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
    void SetStatusReady();
    void Center();
    bool ExecClangFormatCmdSetDialog(const QFileInfoList &cmdList,
                                     QSettings &settings);

private slots:
    void on_openOriginalSrcToolButton_clicked();
    void on_srcPreviewTabWidget_currentChanged(int index);
    void onOriginalSrcEditLinesChanged();
    void onFormattedSrcEditLinesChanged();

    void onDetailsUiControlsUpdate();
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

    bool isSrcUpdateInAction;
    bool userActionTriggered;
    bool newOrigSrcLoaded;

    StylesUiControl *styles;
    TabsUiControl *tabs;

private:
    void setupProgressAnimation();
    void setupTextEditWidgets();
    void setupDetailsUiControls();
    bool readSettings();
    void setTextEditProperties(QsciScintilla *textEdit);
    void setInitialSplitSizes();
    void changeToOriginalSrcTab();
    void changeTabAndResetScrollPos();
    void initializeFormatOptionsWidget();
    void updateFormattedSrc();
    void updateFormattedSrcByUserAction();
    void updateDetailsUiControls();
    void onLinesChanged(QsciScintilla *textEdit);
    void storeVSBPos();
    void updateUiControlsAtStart();
    void updateUiControlsAtFinish();
    void setupDetailsUiControlsForCurrentConfig(const CFConfiguration &config);
};

#endif // MAINWINDOW_H
