#include <QApplication>
#include "mainmenu.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainMenu window;
    window.show();
    return application.exec();
}
