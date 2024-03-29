#include "jogador.h"

#include "./roteador.h"

using namespace Rede_Server;

Jogador::Jogador( int _socket_descriptor, Roteador* _parent )
        : QObject(_parent)
{
    qDebug() << "Novo jogador iniciando";

    this->conexao = new QTcpSocket(this);

    this->estou_pronto = false;
    this->game_over = false;
    this->pontuacao = 0;

    if (! this->conexao->setSocketDescriptor( _socket_descriptor))
    {
        //envia erro
    }

    QObject::connect(this->conexao,SIGNAL(readyRead()),
                     this,SLOT(dadoChegando()));

    QObject::connect(this->conexao,SIGNAL(error(QAbstractSocket::SocketError)),
                     this,SLOT(erroConexao(QAbstractSocket::SocketError)));
}

Jogador::~Jogador()
{
    if (this->conexao->isOpen())
    {
        this->conexao->close();
    }
}

bool
Jogador::operator== ( quint16 _id_jogador )
{
    if (this->id_jogador == _id_jogador )
        return true;
    else
        return false;
}

bool
Jogador::operator!= ( quint16 _id_jogador )
{
    if ( this->id_jogador != _id_jogador )
        return true;
    else
        return false;
}

bool
Jogador::operator< ( Jogador* _outro_jogador )
{
    if( this->pontuacao < _outro_jogador->pontuacao )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void
Jogador::erroConexao( QAbstractSocket::SocketError _erro )
{
    Q_UNUSED( _erro );

    qWarning() << "Jogador" << this->id_jogador << ": ih! A conex�o caiu: "
               << this->conexao->errorString();

    this->conexao->close();

    emit this->erro(this);
}

void
Jogador::enviaDado( QString _dado )
{
    if ( this->conexao->isWritable() )
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::ReadWrite);
        out.setVersion(QDataStream::Qt_4_0);
        out << (quint16)0;
        out << _dado;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        qDebug() << "Jogador " << this->id_jogador << ": escrevendo dados"
                 << _dado;

        this->conexao->flush();
        this->conexao->write( block );
        this->conexao->flush();

    }
    else
    {
        qDebug() << "Jogador" << this->id_jogador << ": nao pude escrever";
        this->erroConexao( QAbstractSocket::NetworkError );
    }
}

void
Jogador::dadoChegando()
{
    quint16 bloco = 0;
    QDataStream in(this->conexao);
    in.setVersion(QDataStream::Qt_4_0);

    while ( this->conexao->bytesAvailable() > sizeof(quint16))
    {
        in >> bloco;

        if ( this->conexao->bytesAvailable() < bloco )
            return; //erro ao ler pacote

        QString mensagem;
        in >> mensagem;

        emit this->novoDado(mensagem);
    }
}
