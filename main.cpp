#include "mainwindow.h"
#include <QApplication>
#include <QCursor>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOverrideCursor(QCursor(Qt::BlankCursor));

    MainWindow w;

    w.show();



    return a.exec();
}
