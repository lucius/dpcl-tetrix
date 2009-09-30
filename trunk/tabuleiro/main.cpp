#include <QtGui/QApplication>
#include <QResource>

#include "tabuleiro.h"


#include <QFileInfo>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setApplicationName("cpdltetris-client");

    Tabuleiro* w = new Tabuleiro();

    w->show();
    return a.exec();
}
