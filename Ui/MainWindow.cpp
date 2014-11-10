#include "MainWindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set the progress indicator of the clang-format command execution.
    progressAnimation = new QMovie(":/Resources/gifs/ajax-loader.gif");
    progressLabel = new QLabel(this);
    progressLabel->setMovie(progressAnimation);
    progressLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(progressLabel);

    // set QSciScintilla widget in the orignal source tab
    originalSrcTextEdit = new QsciScintilla(ui->originalSrcTab);
    ui->verticalLayout_2->addWidget(originalSrcTextEdit);

    // set QSciScintilla widget in the formatted source tab
    formattedSrcTextEdit = new QsciScintilla(ui->formattedSrcTab);
    ui->verticalLayout_3->addWidget(formattedSrcTextEdit);

    // initialize QScintilla text edit widgets with certain common properties
    initializeSrcTextEdit(originalSrcTextEdit);
    initializeSrcTextEdit(formattedSrcTextEdit);

    // get pointers to the vertical scrollbars
    originalSrcTextEditVScrollBar = originalSrcTextEdit->verticalScrollBar();
    formattedSrcTextEditVScrollBar = formattedSrcTextEdit->verticalScrollBar();

    // get values of vertical scrollbars.
    originalSrcTextEditLastVScrollBarPos = originalSrcTextEditVScrollBar->value();
    formattedSrcTextEditLastVScrollBarPos = formattedSrcTextEditVScrollBar->value();

    // The following signals are emitted whenever the lines get changed.
    // In this way we can set the margin width to display the line numbers
    // properly.
    connect(originalSrcTextEdit, SIGNAL(linesChanged()),
            this, SLOT(onOriginalSrcEditLinesChanged()));
    connect(formattedSrcTextEdit, SIGNAL(linesChanged()),
            this, SLOT(onFormattedSrcEditLinesChanged()));

    // initialize SrcFilePreviewer objects
    originalSrcPreviewer = nullptr;
    formattedSrcPreviewer = nullptr;

    srcTabBeforeUiUpdate = ui->originalSrcTab;

    // set initial splitter sizes appropriately
    setInitialSplitSizes();

    // user action is not triggered to format the souce code
    userActionTriggered = false;
    // source file is not loaded on the original source tab
    newOrigSrcLoaded = false;

    // set initial list widget & stack widget settings
    initializeFormatOptionsWidget();

    // FIXME: Should we really do this with QStandardPaths?
    // initialize to home directory of the current user
    defaultFileOpenDir = QDir::homePath();

    // set focus to original source preview tab
    changeToOriginalSrcTab();

    // create initialized formatOptions instance
    formatOptions = new FormatOptions(this);

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
    // set current encoding to UTF-8
    textEdit->setUtf8(true);
}

void MainWindow::onOriginalSrcEditLinesChanged()
{
    onLinesChanged(originalSrcTextEdit);
}

void MainWindow::onFormattedSrcEditLinesChanged()
{
    onLinesChanged(formattedSrcTextEdit);
}

void MainWindow::onLinesChanged(QsciScintilla *textEdit)
{
    textEdit->setMarginLineNumbers(1, true);
    textEdit->setMarginWidth(1, QString().setNum(textEdit->lines() * 10));
}

void MainWindow::storeStatusBeforeUpdate()
{
    srcTabBeforeUiUpdate = ui->srcPreviewTabWidget->currentWidget();

    // we just store the values of scrollbar position
    originalSrcTextEditLastVScrollBarPos = originalSrcTextEditVScrollBar->value();
    formattedSrcTextEditLastVScrollBarPos = formattedSrcTextEditVScrollBar->value();

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

void MainWindow::changeTabAndResetScrollPos()
{
    if (srcTabBeforeUiUpdate == ui->originalSrcTab) {
        ui->srcPreviewTabWidget->setCurrentWidget(ui->formattedSrcTab);
        // reset the vertical scrollbar position
        formattedSrcTextEditVScrollBar->setValue(originalSrcTextEditLastVScrollBarPos);
    } else {
        formattedSrcTextEditVScrollBar->setValue(formattedSrcTextEditLastVScrollBarPos);
    }
}

void MainWindow::initializeFormatOptionsWidget()
{
    // set 0th item in the list widget selected at startup
    ui->detailsStackWidget->setCurrentIndex(0);

    // select the first item of the format options list widget
    ui->formatOptionsListWidget->item(0)->setSelected(true);

    // disable details group box at startup
    ui->detailsGroupBox->setEnabled(false);
}

void MainWindow::on_openOriginalSrcToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Original Source File"),
                                                    defaultFileOpenDir,
                                                    "Source Files (" + SrcFilePreviewer::GetCppExtListStr() +")");

    if (!fileName.isEmpty() && QFileInfo(fileName).exists()) {
        // open file name in the preview text area
        if (originalSrcPreviewer) {
            delete originalSrcPreviewer;
        }

        originalSrcPreviewer = new SrcFilePreviewer(fileName);
        originalSrcPreviewer->ShowPreview(originalSrcTextEdit);

        // enable the details group box after the original source file is loaded
        ui->detailsGroupBox->setEnabled(true);

        // New source file is loaded on to the orignal source tab.
        // We hold the flag true only until the first user action is triggered
        // or the formatted tab is selected for the first time
        newOrigSrcLoaded = true;
    }
}

/**
 * @brief MainWindow::updateFormattedSrc
 * Update the formatted source according to the current formatting options.
 */
void MainWindow::updateFormattedSrc()
{
    // store status before update (scrollbar pos, activated tab etc.)
    storeStatusBeforeUpdate();

    // update the ui controls according to the current settings
    updateUiControls();

    formatOptions->SetInputFile(originalSrcPreviewer->GetFileName());

    QString clangFormatCmdStr = formatOptions->GetClangFormatCommandStr();

    QThread *srcUpdaterThread = new QThread;
    SrcUpdater *srcUpdater = new SrcUpdater(clangFormatCmdStr);
    srcUpdater->moveToThread(srcUpdaterThread);

    /*
     * The same signal started() of srcUpdaterThread class
     * is connected to two slots i.e. onSrcUpdaterStarted() of MainWindow class
     * and started() of srcUpdater class.
     *
     * NOTE: Below is an extrace from the current Qt documentation
     * "If several slots are connected to one signal,
     * the slots will be executed one after the other,
     * in the order they have been connected, when the signal is emitted."
     */
    connect(srcUpdaterThread, SIGNAL(started()),
            this, SLOT(onSrcUpdaterStarted()));

    connect(srcUpdaterThread, SIGNAL(started()),
            srcUpdater, SLOT(start()));

    connect(srcUpdater, SIGNAL(outputReady(QString)),
            this, SLOT(onSrcUpdaterOutputReady(QString)));

    connect(srcUpdater, SIGNAL(outputReady(QString)),
            srcUpdaterThread, SLOT(quit()));

    connect(srcUpdater, SIGNAL(outputReady(QString)),
            srcUpdater, SLOT(deleteLater()));

    connect(srcUpdaterThread, SIGNAL(finished()),
            srcUpdaterThread, SLOT(deleteLater()));

    /*
     * Start the source updatation in a separate thread.
     * This thread creates a 'clang-format' process and executes it
     * synchronously.
     *
     * NOTE: The UI thread stays responsive.
     * But it should be noted that we deliberately disable the controls in the
     * 'Details' panel during the srcUpdaterThread
     */
    srcUpdaterThread->start();
}

void MainWindow::updateFormattedSrcByUserAction()
{
    userActionTriggered = true;
    // after a user action is triggered it is no more a new source file
    newOrigSrcLoaded = false;
    updateFormattedSrc();
}

void MainWindow::onSrcUpdaterStarted()
{
    // disable the details panel, so that the user is unable to change any
    // settings, while the command is being executed
    ui->detailsGroupBox->setEnabled(false);
    // change status bar message
    statusBar()->showMessage(tr("Executing clang-format..."));
    // show the label that contains the animation
    progressLabel->show();
    // start the progress animation
    progressAnimation->start();
}

void MainWindow::onSrcUpdaterOutputReady(const QString &cmd)
{
    if (formattedSrcPreviewer) {
        delete formattedSrcPreviewer;
    }
    formattedSrcPreviewer = new SrcFilePreviewer(originalSrcPreviewer->GetFileNameExtension(),
                                                 cmd);

    formattedSrcPreviewer->ShowPreview(formattedSrcTextEdit);

    // after the formatting is done, we change to the formatted src tab,
    // irrespective of the tab that we're currently in.
    // We reset the scrollbar positions too.
    changeTabAndResetScrollPos();

    // stop the animation
    progressAnimation->stop();
    // hide the label that contains the animation
    progressLabel->hide();
    // change the status bar message
    statusBar()->showMessage(tr("Done"));
    // finally, enable the details panel, so that the user can now try other
    // settings.
    ui->detailsGroupBox->setEnabled(true);
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
 * Set the new style in the formatOptions object and update the formatted source
 * accordingly.
 *
 * @param style format style(LLVM, Google, Chromium, Mozilla, Webkit)
 */
void MainWindow::changeStyleOnRButtonToggle(FormatOptions::Style style)
{
    formatOptions->SetStyle(style);
    updateFormattedSrcByUserAction();
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

    updateFormattedSrcByUserAction();
}

void MainWindow::on_tabWidthSpinBox_valueChanged(int arg1)
{
    if (arg1 > 1 && arg1 < 17) {
        formatOptions->SetTabWidth(arg1);
    }

    updateFormattedSrcByUserAction();
}

void MainWindow::on_srcPreviewTabWidget_currentChanged(int index)
{
    if (index == 0) {
        // when the original source tab is selected we enable 'open' button
        ui->openOriginalSrcToolButton->setEnabled(true);
    } else {
        // when the formatted source tab is selected we disable 'open' button
        ui->openOriginalSrcToolButton->setEnabled(false);
        if (newOrigSrcLoaded && userActionTriggered) {
            newOrigSrcLoaded = false;
            updateFormattedSrc();
        }
    }
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
