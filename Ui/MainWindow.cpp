#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    readSettings();
    return true;
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
    // set 1 st item in the list widget selected at startup
    ui->formatOptionsListWidget->item(0)->setSelected(true);

    // disable style options settings at startup
    setStyleOptions(false);

    // disable tabs options settings at startup
    setTabOptions(false);
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
    formatOptions->SetInputFile(originalSrcPreviewer->GetFileName());

    ClangFormatter clangFormatter;
    if (clangFormatter.Execute(formatOptions)) {
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

void MainWindow::exitApplication(ExitCodes status)
{
    close();
    exit(status);
}

void MainWindow::readSettings()
{

    QSettings settings(organization, application);
    qDebug() << "Reading settings from " << settings.fileName();

    QString clangFormatExe = settings.value(Settings::ClangFormatExe).toString();
    if (clangFormatExe.isEmpty()) {
        qDebug() << "clangFormatExe is empty";

        // Probably, this is the first time the application is started.
        // We search the clang-format tool in standard locations
        QFileInfoList clangFormatCmdList;
        if (!Utility::FindClangFormatCommand(clangFormatCmdList)) {
            // if no clang-format executable is found, terminate the application
            QMessageBox::critical(this,
                                    tr("Fatal Error"),
                                    tr("Could not find clang-format command line tool in PATH.\n"
                                    "Install clang-format first and restart this application.\n"));
            exitApplication(NO_CLANG_FORMAT_EXE);
        } else {
            // set the clang-format executable
            ClangFormatCmdSet clangFormatCmdSetDialog(clangFormatCmdList);

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
                                        tr("Application can not continue without"
                                           " a proper clang-format binary selected.\n"));
                exitApplication(NO_CLANG_FORMAT_EXE_SELECTED);
            }
        }
    } else {
        ClangFormatter::SetClangFormatCommand(clangFormatExe);
    }
}
