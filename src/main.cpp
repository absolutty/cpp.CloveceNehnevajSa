#include "gui/main/mainwindow.h"

#include <QApplication>
#include <QByteArray>

int main(int argc, char *argv[])
{
    qputenv("QT_SCALE_FACTOR", QByteArray("1"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
