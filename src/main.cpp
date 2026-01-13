#include "include/mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Application starting...";

    MainWindow w;
    qDebug() << "MainWindow created, showing...";
    w.show();

    qDebug() << "Application running";
    return a.exec();
}
