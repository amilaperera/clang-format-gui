#include <QApplication>
#include "Ui/MainWindow.h"

#include <QDesktopWidget>

enum ExitStatus {
    SUCCESS,
    PRE_CHECK_FAILED,
};

#if 0
static void loadQss(QApplication &a);
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    loadQss(a);
#endif

    MainWindow w;
    w.Center();
    w.show();

    if (!w.PreCheck()) {
        return PRE_CHECK_FAILED;
    }

    w.SetStatusReady();

    return a.exec();
}

#if 0
static void loadQss(QApplication &a)
{
    QFile file(":/Resources/qss/stylesheet.qss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream styleSheet(&file);
    a.setStyleSheet(styleSheet.readAll());
}
#endif
