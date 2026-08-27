#include "mainwindow.h"

#include <QApplication>
#include <stdlib.h> //qputenv için gerekli

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_EGLFS_ROTATION", "90");   //EGLFS
    //qputenv("QT_QPA_LINUXFB_ROTATION", "90"); //LinuxFB
    qputenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS", "rotate=90"); //dokunmatik ekran da kayarsa kullan

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
