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

    // set original file name to empty string
    originalFileName = "";

    // initialize QScintilla text edit widgets with certain common properties
    initializeSrcTextEdit(originalSrcTextEdit);
    initializeSrcTextEdit(formattedSrcTextEdit);

    // set initial splitter sizes appropriately
    setInitialSplitSizes();

    // set initial list widget & stack widget settings
    initializeFormatOptionsWidget();

    // FIXME: Should we really do this with QStandardPaths?
    // initialize to home directory of the current user
    defaultFileOpenDir = QDir::homePath();
    previewer = nullptr;

    // set focus to original source preview tab
    ui->srcPreviewTabWidget->setCurrentIndex(0);

    connect(this, SIGNAL(originalSrcLoaded()), this, SLOT(on_originalSrcLoaded()));

    qDebug() << "Default Direcotory: " << defaultFileOpenDir;
}

MainWindow::~MainWindow()
{
    // NOTE: QSciScintilla objects will be deleted by ownership concept
    delete previewer;
    delete ui;
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

void MainWindow::initializeFormatOptionsWidget()
{
    // set 1 st item in the list widget selected at startup
    ui->formatOptionsListWidget->item(0)->setSelected(true);

    // we disable style options settings at startup
    setStyleOptions(false);
}

void MainWindow::setStyleOptions(bool enableStatus)
{
    ui->llvmStyleRButton->setEnabled(enableStatus);
    ui->googleStyleRButton->setEnabled(enableStatus);
    ui->chromiumStyleRButton->setEnabled(enableStatus);
    ui->mozillaStyleRButton->setEnabled(enableStatus);
    ui->webkitStyleRButton->setEnabled(enableStatus);
}

void MainWindow::on_openOriginalSrcToolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Preview File"),
                                                    defaultFileOpenDir,
                                                    tr("Source Files (*.c *.cpp *.c++ *.cc *.h *.hpp)"));

    if (!fileName.isEmpty() && QFileInfo(fileName).exists()) {
        // open file name in the preview text area
        if (previewer) {
            delete previewer;
        }

        originalFileName = fileName;
        previewer = new SrcFilePreviewer(fileName);
        previewer->ShowPreview(originalSrcTextEdit);

        // TODO: revisit this.
        // A signal may not be necessary
        emit originalSrcLoaded();
    }
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
}

void MainWindow::on_llvmStyleRButton_toggled(bool checked)
{
    if (checked) {
        ui->srcPreviewTabWidget->setCurrentIndex(1);
    }
}
