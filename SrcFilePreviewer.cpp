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

        qDebug() << "File Name: " << fileName;
        qDebug() << "File Ext: " << fileExt;
    }
}

SrcFilePreviewer::~SrcFilePreviewer()
{
    // QSciLexer object will be deleted by the concept of ownership
}

void SrcFilePreviewer::ShowPreview(QsciScintilla *textEdit)
{
    textEdit->setLexer(lexer);
    textEdit->setText(fileContent);
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
