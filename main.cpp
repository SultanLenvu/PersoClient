#include <QApplication>

#include "GUI/gui_kernel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GuiKernel window;
    window.show();
    return app.exec();
}
