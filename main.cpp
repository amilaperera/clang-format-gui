#include <QApplication>
#include "Ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.PreCheck();

    return a.exec();
}
