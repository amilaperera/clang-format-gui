#include "MainWindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up the progress animation movie & label
    setupProgressAnimation();

    // set up the QScintilla2 text edit widget properties
    setupTextEditWidgets();

    // initialize scroll bar positions of the text widgets
    origSrcTextEditLastVSBPos = 0;
    formattedSrcTextEditLastVSBPos = 0;

    // initialize SrcFilePreviewer objects
    originalSrcPreviewer = nullptr;
    formattedSrcPreviewer = nullptr;

    // set initial splitter sizes appropriately
    setInitialSplitSizes();

    isSrcUpdateInAction = false;
    // user action is not triggered to format the souce code
    userActionTriggered = false;
    // source file is not loaded on the original source tab
    newOrigSrcLoaded = false;

    // set initial list widget & stack widget settings
    initializeFormatOptionsWidget();

    // FIXME: Should we really do this with QStandardPaths?
    // initialize to home directory of the current user
    defaultFileOpenDir = QDir::homePath();
    qDebug() << "Default Direcotory: " << defaultFileOpenDir;

    // set focus to original source preview tab
    changeToOriginalSrcTab();

    // create initialized formatOptions instance
    formatOptions = new FormatOptions(this);

    // setup ui controls in 'Details' panel
    setupDetailsUiControls();
}

MainWindow::~MainWindow()
{
    // NOTE: QSciScintilla objects will be deleted by ownership concept
    delete originalSrcPreviewer;
    // It is clearly mentioned in the documentation setMovie() does not take
    // the ownership of the movie. Therefore the movie has to be deleted
    // manually.
    delete progressAnimation;
    delete ui;
}

void MainWindow::setupProgressAnimation()
{
    // set the progress indicator of the clang-format command execution.
    progressAnimation = new QMovie(":/Resources/gifs/ajax-loader.gif");
    progressLabel = new QLabel(this);
    progressLabel->setMovie(progressAnimation);
    progressLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(progressLabel);
    progressLabel->hide();
}

void MainWindow::setupTextEditWidgets()
{
    // set QSciScintilla widget in the orignal source tab
    originalSrcTextEdit = new QsciScintilla(ui->originalSrcTab);
    ui->verticalLayout_2->addWidget(originalSrcTextEdit);

    // set QSciScintilla widget in the formatted source tab
    formattedSrcTextEdit = new QsciScintilla(ui->formattedSrcTab);
    ui->verticalLayout_3->addWidget(formattedSrcTextEdit);

    // get pointers to the vertical scrollbars
    origSrcTextEditVSB = originalSrcTextEdit->verticalScrollBar();
    formattedSrcTextEditVSB = formattedSrcTextEdit->verticalScrollBar();

    // initialize QScintilla text edit widgets with certain common properties
    setTextEditProperties(originalSrcTextEdit);
    setTextEditProperties(formattedSrcTextEdit);

    // The following signals are emitted whenever the lines get changed.
    // In this way we can set the margin width to display the line numbers
    // properly.
    connect(originalSrcTextEdit, SIGNAL(linesChanged()),
            this, SLOT(onOriginalSrcEditLinesChanged()));
    connect(formattedSrcTextEdit, SIGNAL(linesChanged()),
            this, SLOT(onFormattedSrcEditLinesChanged()));
}

void MainWindow::setupDetailsUiControls()
{
    if (!formatOptions) {
        formatOptions = new FormatOptions(this);
    }

    styles = new StylesUiControl(ui->styleOptionsPage, formatOptions);
    connect(styles, SIGNAL(stylesUpdated()),
            this, SLOT(onDetailsUiControlsUpdate()));

    tabs = new TabsUiControl(ui->tabsOptionsPage, formatOptions);
    connect(tabs, SIGNAL(stylesUpdated()),
            this, SLOT(onDetailsUiControlsUpdate()));
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

void MainWindow::SetStatusReady()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::Center()
{
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

    int width = frameGeometry().width();
    int height = frameGeometry().height();
    setGeometry((screenWidth / 2) - (width / 2),
               (screenHeight / 2) - (height / 2),
               width, height);
}

void MainWindow::setTextEditProperties(QsciScintilla *textEdit)
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

void MainWindow::onDetailsUiControlsUpdate()
{
    updateFormattedSrcByUserAction();
}

void MainWindow::onLinesChanged(QsciScintilla *textEdit)
{
    textEdit->setMarginLineNumbers(1, true);
    textEdit->setMarginWidth(1, QString().setNum(textEdit->lines() * 10));
}

void MainWindow::storeVSBPos()
{
    // we just store the values of scrollbar position
    origSrcTextEditLastVSBPos = origSrcTextEditVSB->value();
    formattedSrcTextEditLastVSBPos = formattedSrcTextEditVSB->value();
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
    // reset the vertical scrollbar position according to the positions
    // that we stored before executing clang-format command
    if (ui->srcPreviewTabWidget->currentWidget() == ui->originalSrcTab) {
        ui->srcPreviewTabWidget->setCurrentWidget(ui->formattedSrcTab);

        formattedSrcTextEditVSB->setValue(origSrcTextEditLastVSBPos);
    } else {
        formattedSrcTextEditVSB->setValue(formattedSrcTextEditLastVSBPos);
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

        statusBar()->showMessage(tr("New file loaded"));
    }
}

/**
 * @brief MainWindow::updateFormattedSrc
 * Update the formatted source according to the current formatting options.
 */
void MainWindow::updateFormattedSrc()
{
    // update the ui controls according to the current settings
    updateDetailsUiControls();

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
    isSrcUpdateInAction = true;

    // change status bar message
    statusBar()->showMessage(tr("Executing clang-format..."));

    updateUiControlsAtStart();

    // show the label that contains the animation
    progressLabel->show();
    // start the progress animation
    progressAnimation->start();
}

void MainWindow::onSrcUpdaterOutputReady(const QString &cmd)
{
    // store verticle scroll bar positions
    storeVSBPos();

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

    updateUiControlsAtFinish();

    // change the status bar message
    statusBar()->showMessage(tr("Done"));

    isSrcUpdateInAction = false;
}

void MainWindow::updateUiControlsAtStart()
{
    // disable/enable ui components just before the update process
    ui->detailsGroupBox->setEnabled(false);
    ui->openOriginalSrcToolButton->setEnabled(false);
}

void MainWindow::updateUiControlsAtFinish()
{
    // disable/enable ui components just after the update process
    ui->detailsGroupBox->setEnabled(true);
    ui->openOriginalSrcToolButton->setEnabled(ui->srcPreviewTabWidget->currentWidget() == ui->originalSrcTab ?
                                                  true : false);
}

void MainWindow::updateDetailsUiControls()
{
    // TODO: update the UI according to the clang command which is to be
    // executed.
    ClangFormatter clangFormatter;
    if (clangFormatter.Execute(formatOptions->GetClangFormatDumpCommandStr())) {
        qDebug() << "clangFormatter configuration dumped successfully";
    } else {
        qDebug() << "clangFormatter configuration dump failed";
    }

    CFConfiguration currentConfig(clangFormatter.GetOutput());
#ifdef QT_DEBUG
    qDebug() << "Configurations:";
    currentConfig.ToString();
#endif
    setupDetailsUiControlsForCurrentConfig(currentConfig);
}

void MainWindow::setupDetailsUiControlsForCurrentConfig(const CFConfiguration &config)
{

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

void MainWindow::on_srcPreviewTabWidget_currentChanged(int index)
{
    if (!isSrcUpdateInAction) {
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
}
