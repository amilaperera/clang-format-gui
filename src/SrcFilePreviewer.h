#ifndef SRCFILEPREVIEWER_H
#define SRCFILEPREVIEWER_H

#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QDebug>
#include <QObject>
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexer.h"
#include "Qsci/qscilexercpp.h"

class SrcFilePreviewer : public QObject
{
    Q_OBJECT
public:
    explicit SrcFilePreviewer(const QString &f, QObject *parent = 0);
    explicit SrcFilePreviewer(const QString &fExt, const QString &fContent,
                              QObject *parent = 0);
    ~SrcFilePreviewer();
    void ShowPreview(QsciScintilla *textEdit);
    QString GetFileName();
    QString GetFileNameExtension();
    QString GetFileContent();

public:
    static const QString GetCppExtListStr();
    static bool IsFileExtCpp(const QString &fileExt);

signals:

public slots:

private:
    QString fileName;
    QString fileContent;
    QString fileExt;
    QsciLexer *lexer;

private:
    static const QStringList cppExtList;

private:
    QString getFileNameExtension();
    QsciLexer *createLexer();
};

#endif // SRCFILEPREVIEWER_H
