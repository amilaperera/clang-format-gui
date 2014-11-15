#include <QApplication>
#include "Ui/MainWindow.h"

#include <QDesktopWidget>

enum ExitStatus {
    SUCCESS,
    PRE_CHECK_FAILED,
};

static void loadQss(QApplication &a);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadQss(a);

    MainWindow w;
    w.Center();
    w.show();

    if (!w.PreCheck()) {
        return PRE_CHECK_FAILED;
    }

    w.SetStatusReady();

    return a.exec();
}

static void loadQss(QApplication &a)
{
    QFile file(":/Resources/qss/stylesheet.qss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream styleSheet(&file);
    a.setStyleSheet(styleSheet.readAll());
}
