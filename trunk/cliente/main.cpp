#include <QtGui/QApplication>
#include <QResource>

#include "cliente.h"

#include <QFileInfo>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setApplicationName("cpdltetris-client");

    Cliente* w = new Cliente();

    return a.exec();
}
