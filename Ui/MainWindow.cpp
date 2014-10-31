#include "MainWindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setProgressVisibility(false);

    // set QSciScintilla widget in the orignal source tab
    originalSrcTextEdit = new QsciScintilla(ui->originalSrcTab);
    ui->verticalLayout_2->addWidget(originalSrcTextEdit);

    // set QSciScintilla widget in the formatted source tab
    formattedSrcTextEdit = new QsciScintilla(ui->formattedSrcTab);
    ui->verticalLayout_3->addWidget(formattedSrcTextEdit);

    // initialize QScintilla text edit widgets with certain common properties
    initializeSrcTextEdit(originalSrcTextEdit);
    initializeSrcTextEdit(formattedSrcTextEdit);

    // initialize SrcFilePreviewer objects
    originalSrcPreviewer = nullptr;
    formattedSrcPreviewer = nullptr;

    // set initial splitter sizes appropriately
    setInitialSplitSizes();

    // set initial list widget & stack widget settings
    initializeFormatOptionsWidget();

    // FIXME: Should we really do this with QStandardPaths?
    // initialize to home directory of the current user
    defaultFileOpenDir = QDir::homePath();

    // set focus to original source preview tab
    changeToOriginalSrcTab();

    // create initialized formatOptions instance
    formatOptions = new FormatOptions(this);

    connect(this, SIGNAL(originalSrcLoaded()), this, SLOT(on_originalSrcLoaded()));

    qDebug() << "Default Direcotory: " << defaultFileOpenDir;
}

MainWindow::~MainWindow()
{
    // NOTE: QSciScintilla objects will be deleted by ownership concept
    delete originalSrcPreviewer;
    delete ui;
}

/**
 * @brief MainWindow::preCheck
 * Checks for several pre-requisites needed for the application to run
 * @return true on success, flase on error
 */
bool MainWindow::PreCheck()
{
    bool ret = true;
    ret = readSettings();
    return ret;
}

void MainWindow::initializeSrcTextEdit(QsciScintilla *textEdit)
{
    // make the editor read only
    textEdit->setReadOnly(true);
    // make the whitespaces visible with a centered dot
    textEdit->setWhitespaceVisibility(QsciScintilla::WsVisible);
}

void MainWindow::setInitialSplitSizes()
{
    QList<int> splitter_2_sizes;
    splitter_2_sizes << 150 << 600;
    ui->splitter_2->setSizes(splitter_2_sizes);

    QList<int> splitter_sizes;
    splitter_sizes << 300 << 300;
    ui->splitter->setSizes(splitter_sizes);
}

void MainWindow::changeToOriginalSrcTab()
{
    ui->srcPreviewTabWidget->setCurrentWidget(ui->originalSrcTab);
}

void MainWindow::changeToFormattedSrcTab()
{
    ui->srcPreviewTabWidget->setCurrentWidget(ui->formattedSrcTab);
}

void MainWindow::initializeFormatOptionsWidget()
{
    // set 0th item in the list widget selected at startup
    ui->detailsStackWidget->setCurrentIndex(0);

    ui->formatOptionsListWidget->item(0)->setSelected(true);

    // disable style options settings at startup
    setStyleOptions(false);

    // disable tabs options settings at startup
    setTabOptions(false);
}

void MainWindow::setProgressVisibility(bool status)
{
    ui->previewingLabel->setEnabled(status);
    ui->progressBar->setEnabled(status);

    if (!status) {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);
    } else {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        ui->progressBar->setValue(0);
    }
}

void MainWindow::setStyleOptions(bool enableStatus)
{
    ui->llvmStyleRButton->setEnabled(enableStatus);
    ui->googleStyleRButton->setEnabled(enableStatus);
    ui->chromiumStyleRButton->setEnabled(enableStatus);
    ui->mozillaStyleRButton->setEnabled(enableStatus);
    ui->webkitStyleRButton->setEnabled(enableStatus);
}

void MainWindow::setTabOptions(bool enableStatus)
{
    ui->useTabsLbl->setEnabled(enableStatus);
    ui->useTabsComboBox->setEnabled(enableStatus);
    ui->tabWidthLbl->setEnabled(enableStatus);
    ui->tabWidthSpinBox->setEnabled(enableStatus);
}

void MainWindow::on_openOriginalSrcToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Preview File"),
                                                    defaultFileOpenDir,
                                                    tr("Source Files (*.c *.cpp *.c++ *.cc *.h *.hpp)"));

    if (!fileName.isEmpty() && QFileInfo(fileName).exists()) {
        // open file name in the preview text area
        if (originalSrcPreviewer) {
            delete originalSrcPreviewer;
        }

        originalSrcPreviewer = new SrcFilePreviewer(fileName);
        originalSrcPreviewer->ShowPreview(originalSrcTextEdit);

        // TODO: revisit this.
        // A signal may not be necessary
        emit originalSrcLoaded();
    }
}

/**
 * @brief MainWindow::updateFormattedSrc
 * Update the formatted source according to the current formatting options.
 */
void MainWindow::updateFormattedSrc()
{
    // update the ui controls according to the current settings
    updateUiControls();

    formatOptions->SetInputFile(originalSrcPreviewer->GetFileName());

    QString clangFormatCmdStr = formatOptions->GetClangFormatCommandStr();
#if 0
    ClangFormatter clangFormatter;
#endif

    QThread *srcUpdaterThread = new QThread;
    SrcUpdater *srcUpdater = new SrcUpdater(clangFormatCmdStr);
    srcUpdater->moveToThread(srcUpdaterThread);

    connect(srcUpdaterThread, SIGNAL(started()),
            srcUpdater, SLOT(start()));
    connect(srcUpdater, SIGNAL(outputReady(QString)),
            this, SLOT(onSrcUpdaterOutputReady(QString)));
    connect(srcUpdater, SIGNAL(outputReady(QString)),
            srcUpdaterThread, SLOT(quit()));
    connect(srcUpdater, SIGNAL(outputReady(QString)),
            srcUpdater, SLOT(deleteLater()));
    connect(srcUpdater, SIGNAL(outputReady(QString)),
            srcUpdaterThread, SLOT(deleteLater()));

    setProgressVisibility(true);
    srcUpdaterThread->start();
#if 0
    if (clangFormatter.Execute(clangFormatCmdStr)) {
        qDebug() << "clangFormatter process executed successfully";
    } else {
        qDebug() << "clangFormatter process execution failed";
    }

    /*
     * formattedSrcPreviewer is created each time, the formatted source
     * is updated.
     */
    if (formattedSrcPreviewer) {
        delete formattedSrcPreviewer;
    }
    formattedSrcPreviewer = new SrcFilePreviewer(originalSrcPreviewer->GetFileNameExtension(),
                                                 clangFormatter.GetOutput());

    changeToFormattedSrcTab();
    formattedSrcPreviewer->ShowPreview(formattedSrcTextEdit);
#endif
}

void MainWindow::onSrcUpdaterOutputReady(const QString &cmd)
{
    if (formattedSrcPreviewer) {
        delete formattedSrcPreviewer;
    }
    formattedSrcPreviewer = new SrcFilePreviewer(originalSrcPreviewer->GetFileNameExtension(),
                                                 cmd);

    changeToFormattedSrcTab();
    formattedSrcPreviewer->ShowPreview(formattedSrcTextEdit);
    setProgressVisibility(false);
}

void MainWindow::updateUiControls()
{
    ClangFormatter clangFormatter;
    if (clangFormatter.Execute(formatOptions->GetClangFormatDumpCommandStr())) {
        qDebug() << "clangFormatter configuration dumped successfully";
    } else {
        qDebug() << "clangFormatter configuration dump failed";
    }
    qDebug() << "Configuration Dump";
    qDebug() << clangFormatter.GetOutput();
}

bool MainWindow::readSettings()
{
    bool ret = true;
    QSettings settings(organization, application);
    qDebug() << "Reading settings from " << settings.fileName();

    QString clangFormatExe = settings.value(Settings::ClangFormatExe).toString();
    if (clangFormatExe.isEmpty()) {
        qDebug() << "clangFormatExe is empty";

        // Probably, this is the first time the application is started.
        // We search the clang-format tool in standard locations.
        QFileInfoList clangFormatCmdList;

        // We get the installed clang-format command list in the system path.
        // Since we let the user to select the clang-format executable manually,
        // we can safely ignore the return of this function.
        (void) Utility::FindClangFormatCommand(clangFormatCmdList);

        // Displays the "Set clang-format-executable" dialog box
        // and have the required file input from the user.
       ret = ExecClangFormatCmdSetDialog(clangFormatCmdList, settings);

    } else {
        if (!QFileInfo(clangFormatExe).exists()) {
            // If the path of the clang-format saved in the configuration
            // is invalid (does not exit) display an error.
            QMessageBox::warning(this,
                                 tr("Error"),
                                 QString(tr("The default clang-format executable"
                                            " path(%1) does not exist.\n")).arg(clangFormatExe),
                                 QMessageBox::Ok);

            // We get the installed clang-format command list in the system path.
            // Since we let the user to select the clang-format executable manually,
            // we can safely ignore the return of this function.
            QFileInfoList clangFormatCmdList;
            (void) Utility::FindClangFormatCommand(clangFormatCmdList);

           ret = ExecClangFormatCmdSetDialog(clangFormatCmdList, settings);
        } else {
            ClangFormatter::SetClangFormatCommand(clangFormatExe);
        }
    }
    return ret;
}

bool MainWindow::ExecClangFormatCmdSetDialog(const QFileInfoList &cmdList,
                                             QSettings &settings)
{
    bool ret = true;
    ClangFormatCmdSet clangFormatCmdSetDialog(cmdList);

    // set as a modal dialog
    if (clangFormatCmdSetDialog.exec()) {
        // accepted
        QString cmd = clangFormatCmdSetDialog.GetSelectedFormatCmd();

        ClangFormatter::SetClangFormatCommand(cmd);
        if (clangFormatCmdSetDialog.GetSaveSettingsStatus()) {
            settings.setValue(Settings::ClangFormatExe, cmd);
        }
    } else {
        // rejected
        QMessageBox::critical(this,
                                tr("Fatal Error"),
                                tr("Application can not continue without "
                                   "a proper clang-format executable being selected.\n"));
        ret = false;
    }
    return ret;
}

/**
 * @brief MainWindow::changeStyleOnRButtonToggle
 * Set the new style in the formatOptions object.
 *
 * @param style format style(LLVM, Google, Chromium, Mozilla, Webkit)
 */
void MainWindow::changeStyleOnRButtonToggle(FormatOptions::Style style)
{
    formatOptions->SetStyle(style);
    updateFormattedSrc();
}

void MainWindow::on_srcPreviewTabWidget_currentChanged(int index)
{
    // if the user clicks on formatted src preview tab we disable open button
    if (index == 1) {
        ui->openOriginalSrcToolButton->setEnabled(false);
    } else {
        ui->openOriginalSrcToolButton->setEnabled(true);
    }
}

void MainWindow::on_originalSrcLoaded()
{
    setStyleOptions(true);
    setTabOptions(true);
}

void MainWindow::on_llvmStyleRButton_toggled(bool checked)
{
    if (checked) {
        changeStyleOnRButtonToggle(FormatOptions::LLVM);
    }
}

void MainWindow::on_googleStyleRButton_toggled(bool checked)
{
    if (checked) {
        changeStyleOnRButtonToggle(FormatOptions::Google);
    }
}

void MainWindow::on_chromiumStyleRButton_toggled(bool checked)
{
    if (checked) {
        changeStyleOnRButtonToggle(FormatOptions::Chromium);
    }
}

void MainWindow::on_mozillaStyleRButton_toggled(bool checked)
{
    if (checked) {
        changeStyleOnRButtonToggle(FormatOptions::Mozilla);
    }
}

void MainWindow::on_webkitStyleRButton_toggled(bool checked)
{
    if (checked) {
        changeStyleOnRButtonToggle(FormatOptions::Webkit);
    }
}

void MainWindow::on_useTabsComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Never") {
        formatOptions->SetUseTab(FormatOptions::Never);
    } else if (arg1 == "For indentation") {
        formatOptions->SetUseTab(FormatOptions::ForIndentation);
    } else if (arg1 == "Always") {
        formatOptions->SetUseTab(FormatOptions::Always);
    }

    updateFormattedSrc();
}


void MainWindow::on_tabWidthSpinBox_valueChanged(int arg1)
{
    if (arg1 > 1 && arg1 < 17) {
        formatOptions->SetTabWidth(arg1);
    }

    updateFormattedSrc();
}
