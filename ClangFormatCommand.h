#ifndef CLANGFORMATCOMMANDBUILDER_H
#define CLANGFORMATCOMMANDBUILDER_H

#include <QObject>

class ClangFormatCommand : public QObject
{
    Q_OBJECT
public:
    explicit ClangFormatCommand(QObject *parent = 0);
    explicit ClangFormatCommand(const QString &f, QObject *parent = 0);
    void SetFile(const QString &in);
    void SetStyle(const QString &style);
    void SetOutputFile(const QString &out);
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
