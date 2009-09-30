#include <QApplication>
#include <iostream>

#include "conexao.h"
#include "tradutor.h"
#include "teste.h"

#include <QDebug>

#include <QLineEdit>

using namespace Rede_Cliente;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Teste teste;
    teste.show();

    return app.exec();
}
