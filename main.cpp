#include <QApplication>
#include "Ui/MainWindow.h"

enum ExitStatus {
    SUCCESS,
    PRE_CHECK_FAILED,
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (!w.PreCheck()) {
        return PRE_CHECK_FAILED;
    }

    return a.exec();
}
