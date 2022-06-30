#include "Qtsever.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qtsever w;
    w.show();
    return a.exec();
}
