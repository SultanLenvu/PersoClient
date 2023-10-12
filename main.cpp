#include <QApplication>

#include "GUI/mainwindow_kernel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindowKernel window;
    window.show();
    return app.exec();
}
