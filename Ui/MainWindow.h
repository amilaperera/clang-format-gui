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
#include <Qsci/qsciscintilla.h>

#include "SrcFilePreviewer.h"
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
    enum ExitCodes {
        SUCCESS = 0,

        NO_CLANG_FORMAT_EXE,
        NO_CLANG_FORMAT_EXE_SELECTED,
    };

    explicit MainWindow(QWidget *parent = 0);
    bool PreCheck();
    ~MainWindow();

private slots:
    void on_openOriginalSrcToolButton_clicked();

    void on_srcPreviewTabWidget_currentChanged(int index);
    void on_originalSrcLoaded();

    void on_llvmStyleRButton_toggled(bool checked);
    void on_googleStyleRButton_toggled(bool checked);
    void on_chromiumStyleRButton_toggled(bool checked);
    void on_mozillaStyleRButton_toggled(bool checked);
    void on_webkitStyleRButton_toggled(bool checked);

    void on_useTabsComboBox_currentIndexChanged(const QString &arg1);

signals:
    void originalSrcLoaded();

private:
    const QString organization = "clang-format-GUI";
    const QString application = "clang-format-GUI";

    QsciScintilla *originalSrcTextEdit;
    QsciScintilla *formattedSrcTextEdit;
    SrcFilePreviewer *originalSrcPreviewer;
    SrcFilePreviewer *formattedSrcPreviewer;
    Ui::MainWindow *ui;
    QString defaultFileOpenDir;
    FormatOptions *formatOptions;

private:
    void exitApplication(ExitCodes status);
    void writeSettings();
    void readSettings();
    void initializeSrcTextEdit(QsciScintilla *textEdit);
    void setInitialSplitSizes();
    void changeToOriginalSrcTab();
    void changeToFormattedSrcTab();
    void initializeFormatOptionsWidget();
    void setStyleOptions(bool enableStatus);
    void setTabOptions(bool enableStatus);
    void updateFormattedSrc();
    void changeStyleOnRButtonToggle(FormatOptions::Style style);
};

#endif // MAINWINDOW_H
