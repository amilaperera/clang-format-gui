#include <QApplication>
#include "Ui/MainWindow.h"

#include <QDesktopWidget>

enum ExitStatus {
    SUCCESS,
    PRE_CHECK_FAILED,
};

static void centerMainWindow(MainWindow *w);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    centerMainWindow(&w);

    w.show();
    if (!w.PreCheck()) {
        return PRE_CHECK_FAILED;
    }

    return a.exec();
}

void centerMainWindow(MainWindow *w)
{
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

    int width = w->frameGeometry().width();
    int height = w->frameGeometry().height();
    w->setGeometry((screenWidth / 2) - (width / 2),
                   (screenHeight / 2) - (height / 2),
                   width,
                   height);
}
