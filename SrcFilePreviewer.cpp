#include "SrcFilePreviewer.h"

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
        textEdit->setLexer(lexer);
        textEdit->setText(fileContent);
    } else {
        // TODO: consider throwing an exception
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
    if (fileExt == "c" ||
        fileExt == "cpp" ||
        fileExt == "cc" ||
        fileExt == "c++" ||
        fileExt == "h" ||
        fileExt == "hpp") {

        return new QsciLexerCPP(this);
    }

    return nullptr;
}
