#include <QApplication>
#include <QDebug>

#include <cstdlib>

#include "roteador.h"

int main( int argc, char* argv[] )
{
    QApplication*
    app = new QApplication( argc, argv );

    int
    num_jogadores = 2;

    if ( argc == 2 )
    {
        num_jogadores = atoi(argv[1]);
    }

    Rede_Server::Roteador*
    novo_roteador =  new Rede_Server::Roteador(num_jogadores);

    QObject::connect(novo_roteador,SIGNAL(ninguemConectado()),
                     app,SLOT(quit()));

    return app->exec();
}
