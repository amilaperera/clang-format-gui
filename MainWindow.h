#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMainWindow>
#include <QDebug>
#include <QFont>
#include <QTreeWidget>
#include <Qsci/qsciscintilla.h>

#include "SrcFilePreviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openOriginalSrcToolButton_clicked();

    void on_srcPreviewTabWidget_currentChanged(int index);

    void on_formatOptionsTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    QsciScintilla *originalSrcTextEdit;
    QsciScintilla *formattedSrcTextEdit;
    SrcFilePreviewer *previewer;
    Ui::MainWindow *ui;
    QString defaultFileOpenDir;

private:
    void initializeSrcTextEdit(QsciScintilla *textEdit);
};

#endif // MAINWINDOW_H
