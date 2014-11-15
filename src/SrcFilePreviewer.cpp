#include "SrcFilePreviewer.h"

const QStringList SrcFilePreviewer::cppExtList = QStringList()
                                                << "c"
                                                << "cpp"
                                                << "cc"
                                                << "c++"
                                                << "cxx"
                                                << "h"
                                                << "hpp";

SrcFilePreviewer::SrcFilePreviewer(const QString &f, QObject *parent) :
    QObject(parent)
{
    fileName = f;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        // TODO: handle error.
        // Probably we should throw an exception
    } else {
        QTextStream in(&file);
        fileContent = in.readAll();
        file.close();

        fileExt = getFileNameExtension();
        lexer = createLexer();
        if (!lexer) {
            // TODO: handle error
            // Probably we should throw an exception
        }

        qDebug() << "File Name: " << fileName;
        qDebug() << "File Ext: " << fileExt;
    }
}

SrcFilePreviewer::SrcFilePreviewer(const QString &fExt, const QString &fContent,
                                   QObject *parent) : QObject(parent)
{
    fileExt = fExt;
    fileContent = fContent;
    lexer = createLexer();
    if (!lexer) {
        // TODO: handle error
        // Probably we should throw an exception
    }
}

SrcFilePreviewer::~SrcFilePreviewer()
{
    // QSciLexer object will be deleted by the concept of ownership
}

void SrcFilePreviewer::ShowPreview(QsciScintilla *textEdit)
{
    if (lexer) {
        textEdit->clear();
        textEdit->setLexer(lexer);
        textEdit->setText(fileContent);
    } else {
        // TODO: throw an exception
        qDebug() << "Can't show preview because, the file extension is unrecognized by the application";
    }
}

QString SrcFilePreviewer::GetFileName()
{
    return fileName;
}

QString SrcFilePreviewer::GetFileNameExtension()
{
    return getFileNameExtension();
}

QString SrcFilePreviewer::GetFileContent()
{
    return fileContent;
}

const QString SrcFilePreviewer::GetCppExtListStr()
{
    QString listStr;
    foreach (const QString &str, SrcFilePreviewer::cppExtList) {
        listStr += ("*." + str + " ");
    }
    return listStr.trimmed();
}

bool SrcFilePreviewer::IsFileExtCpp(const QString &fileExt)
{
    foreach (const QString &str, SrcFilePreviewer::cppExtList) {
        if (str == fileExt) {
            return true;
        }
    }
    return false;
}

QString SrcFilePreviewer::getFileNameExtension()
{
    QString ext = "";
    if (!fileName.isEmpty()) {
        ext = fileName.section('.', -1);
    }
    return ext;
}

QsciLexer *SrcFilePreviewer::createLexer()
{
    if (SrcFilePreviewer::IsFileExtCpp(fileExt)) {
        return new QsciLexerCPP(this);
    }

    return nullptr;
}
