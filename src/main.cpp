#include "To2048.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    To2048 w;
    w.show();
    return a.exec();
}
