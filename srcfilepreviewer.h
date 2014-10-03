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
    ~SrcFilePreviewer();
    void ShowPreview(QsciScintilla *textEdit);

signals:

public slots:

private:
    QString fileName;
    QString fileContent;
    QString fileExt;
    QsciLexer *lexer;

private:
    QString getFileNameExtension();
    QsciLexer *createLexer();
};

#endif // SRCFILEPREVIEWER_H
