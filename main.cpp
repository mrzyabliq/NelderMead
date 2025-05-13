#include "mainwindow.h"

#include <QApplication>

#include "NelderMeadDLL/include/NelderMead.h"

int main(int argc, char *argv[])
{
    auto nm = new NelderMead("x1+x2");
    nm->Solver();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
