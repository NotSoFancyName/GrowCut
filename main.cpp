#include "mainwindow.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(700,600);
    QImage myImage("c:\\!try\\test.jpg");
    //graphicsView.setBackgroundBrush(myImage);

    w.show();

    return a.exec();
}
