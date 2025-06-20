#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon customIcon(":/icons/Kaban.png"); // Задаем путь через систему ресурсов Qt
    w.setWindowIcon(customIcon);
    w.show();
    return a.exec();
}
