#include "conexao.h"
#include "tradutor.h"

using namespace Rede_Cliente;

Conexao::Conexao(Tradutor* _parent):QTcpSocket(_parent)
{
    QObject::connect(this, SIGNAL(readyRead()),
                     this, SLOT(recebeDado()));
}



void
Conexao::enviaDado(QString _dado)
{
    if(this->isWritable())
    {
        QByteArray bloco;
        QDataStream saida(&bloco, QIODevice::ReadWrite);
        saida.setVersion(QDataStream::Qt_4_0);
        saida << (quint16)0;
        saida << _dado;
        saida.device()->seek(0);
        saida << (quint16)(bloco.size() - sizeof(quint16));

        this->flush();
        this->write(bloco);
    }
}



void
Conexao::recebeDado()
{
    quint16 bloco = 0;
    QDataStream entrada(this);
    entrada.setVersion(QDataStream::Qt_4_0);
    
    while(this->bytesAvailable() > sizeof(quint16))
    {
        entrada >> bloco;

        if(this->bytesAvailable() < bloco)
            return;

        QString mensagem;
        entrada >> mensagem;

        emit this->incommingMessage(mensagem);
    }

}
