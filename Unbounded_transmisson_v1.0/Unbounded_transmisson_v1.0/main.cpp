#include "serverwidget.h"
#include "clientwidget.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w3;
    w3.show();
    return a.exec();
}
