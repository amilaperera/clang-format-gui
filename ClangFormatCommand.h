#ifndef CLANGFORMATCOMMANDBUILDER_H
#define CLANGFORMATCOMMANDBUILDER_H

#include <QObject>

class ClangFormatCommand : public QObject
{
    Q_OBJECT
public:
    explicit ClangFormatCommand(QObject *parent = 0);
    explicit ClangFormatCommand(const QString &in, QObject *parent = 0);
    void SetInputFile(const QString &in);
    void SetOutputFile(const QString &out);
    void SetStyle(const QString &style);
    QString GetClangFormatCommand() const;

signals:

public slots:

private:
    static const QString clangFormatCmd;
    QString inputFile;
    QString outputFile;
    mutable QString command;
};

#endif // CLANGFORMATCOMMANDBUILDER_H
