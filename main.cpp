#include "mainwindow.h"

#include <QApplication>
#include <myapplication.h>
#include <QObject>

int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);
    MainWindow w;

    QObject::connect(&a,SIGNAL(signalLog(QString)),&w,SLOT(slotLog(QString)));
    QObject::connect(&a,SIGNAL(signalKeyboardEvent(uint8_t, bool)),&w,SLOT(slotKeyboardEvent(uint8_t, bool)));
    QObject::connect(&w,SIGNAL(signalChangeStateFilterKeyboard(bool)),&a,SLOT(slotChangeStateFilterKeyboard(bool)));

    w.show();
    return a.exec();
}
