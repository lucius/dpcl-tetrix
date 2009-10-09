#include "roteador.h"

#include <QDebug>
#include <QStringList>

Rede_Server::Roteador::Roteador( int _njogadores, QObject* _parent )
        : QObject(_parent)
{
    qsrand( QTime::currentTime().msec() );
    this->njogadores = _njogadores;

    this->ouvinte = new Ouvinte(this);

    this->ouvinte->startListen();
    qDebug() << "Roteador: iniciado!";

    this->current_id = 1;
}

Rede_Server::Roteador::~Roteador()
{

}

void
Rede_Server::Roteador::stopListen()
{
    this->ouvinte->close();
}

void
Rede_Server::Roteador::novoJogador( Jogador* _novo_jogador )
{
    qDebug() << "Roteador: Novo jogador conectado!!";

    QObject::connect(_novo_jogador,SIGNAL(novoDado(QString)),
                     this,SLOT(recebeDado(QString)));

    QObject::connect(_novo_jogador,SIGNAL(erro( Jogador* )),
                     this,SLOT(jogadorCaiu(Jogador*)));

    /* SERIA LEGAL MODULARIZAR TODAS AS FORMAÇÕES DE COMANDOS */
    QString
    comando_init("init:");

    comando_init.append( QString::number(this->current_id) );
    _novo_jogador->id_jogador = this->current_id;
    ++this->current_id;

    _novo_jogador->enviaDado(comando_init);

    this->listaJogadores.push_back( _novo_jogador );

    QObject::connect(this,SIGNAL(broadcast(QString)),
                     _novo_jogador,SLOT(enviaDado(QString)));
}

void
Rede_Server::Roteador::recebeDado( QString _dado )
{
    qDebug() << "Roteador::recebeDado:" << _dado;
    QString
    comando_str = _dado.left(4).toLower();

    if ( comando_str == "enca" )
    {
	this->recebeEnca(_dado);
    }
    else if ( comando_str == "novo" )
    {
        quint16
        id_jogador = this->getIdJogador(_dado);

        this->setNomeJogador( _dado );
        this->enviaListaJogadores( id_jogador );
    }
    else if ( comando_str == "star" )
    {
        this->recebeStart( _dado );
    }
    else if ( comando_str == "over" )
    {
        this->recebeOver( _dado );
    }
    else if ( comando_str == "azia" )
    {
        this->recebeAzia( _dado );
    }
    else
    {
        emit this->broadcast(_dado);
    }
}

void
Rede_Server::Roteador::jogadorCaiu( Jogador* _com_erro )
{
    QObject::disconnect(this,SIGNAL(broadcast(QString)),
                        _com_erro,SLOT(enviaDado(QString)));

    int
    id_vetor = this->listaJogadores.indexOf(_com_erro),
    id_jogador = _com_erro->id_jogador;

    this->listaJogadores.removeAt(id_vetor);


    if ( this->listaJogadores.size() > 0 )
    {
        QString
        comando_caiu( "down:" +  QString::number(id_jogador));

        qDebug() << "Roteador: jogador " << id_jogador << " caiu!";

        emit this->broadcast(comando_caiu);
    }
    else
    {
        qDebug() << "Roteador::down:Todos os jogadores cairam!"
                 << "          roteador desligando!";

        emit this->ninguemConectado();
    }
}

void
Rede_Server::Roteador::startJogo()
{
    this->stopListen();
    qDebug() << "roteador: to startano fia!";

    sleep(1);
    QString
    comando_start("star:");

    comando_start.append( this->geraNovaPeca() + ";" );
    comando_start.append( this->geraNovaPeca() );

    emit this->broadcast(comando_start);
}

void
Rede_Server::Roteador::setNomeJogador( QString _dado )
{
    quint16
    id_jogador = this->getIdJogador( _dado );

    QString
    novo_nome = _dado.split(";")[1];

    Jogador*
    jogador = this->getJogadorById( id_jogador );

    if ( jogador != NULL)
        jogador->nome_jogador = novo_nome;
    else
        qWarning() << "Roteador::setNomeJogador: jogador desconhecido!";
}

quint16
Rede_Server::Roteador::getIdJogador( QString _dado )
{
    QString
    id_str = _dado.split(":")[1].split(";")[0];

    if ( id_str.isEmpty() )
    {
        qWarning() << "Dado sem o campo de id!";
    }

    return id_str.toInt();
}

Rede_Server::Jogador*
Rede_Server::Roteador::getJogadorById( quint16 _id_jogador )
{
    QList<Rede_Server::Jogador*>::iterator
    it_jogadores;

    for ( it_jogadores = this->listaJogadores.begin();
          it_jogadores != this->listaJogadores.end();
          it_jogadores++ )
    {
        if ( **it_jogadores == _id_jogador )
        {
            return *it_jogadores;
        }
    }

    qWarning() << "Roteador::getJogadorById: nao achei o jogador com a id: "
               << _id_jogador;

    return NULL;
}

void
Rede_Server::Roteador::enviaListaJogadores( quint16 _id_jogador )
{
    QStringList
    listaJogadores;

    QList<Jogador*>::iterator
    it_jogadores;

    QString
    entrada_jogador;

    for ( it_jogadores = this->listaJogadores.begin();
          it_jogadores != this->listaJogadores.end();
          it_jogadores++)
    {
        entrada_jogador = QString::number((*it_jogadores)->id_jogador)+";"+
                          (*it_jogadores)->nome_jogador;

        listaJogadores << entrada_jogador;
    }

    QString
    msg_lista_jogadores = "novo:";

    msg_lista_jogadores.append( listaJogadores.join(" ") );

    emit this->broadcast(msg_lista_jogadores);
}

void
Rede_Server::Roteador::recebeStart( QString _dado )
{
    Rede_Server::Jogador*
    _emissor = this->getJogadorById( this->getIdJogador( _dado ) );

    bool
    esta_pronto = _dado.split(";")[1].toInt();

    qDebug() << "Jogador " << _emissor->nome_jogador << " "
             << "enviou sinal de pronto: " << esta_pronto;

    _emissor->estou_pronto = esta_pronto;

    if ( esta_pronto && this->checkAllStart() )
    {
        QString _mensagem;

         _mensagem = "chat:1;Todos jogadores prontos, comecando em 4 segundos";
        emit this->broadcast(_mensagem);

        sleep(3);
        qDebug() << "roteador:iniciando o jogo!";
        this->startJogo();
    }
}

void
Rede_Server::Roteador::recebeOver( QString _dado )
{
    Jogador*
    jogador = this->getJogadorById( this->getIdJogador(_dado) );

    quint16
    pontuacao = _dado.split(";")[1].toInt();

    jogador->game_over = true;
    jogador->pontuacao = pontuacao;

    this->broadcast(_dado);

    foreach( jogador , this->listaJogadores )
    {
        if ( jogador->game_over == false )
        {
            return;
        }
    }
    this->declaraFimJogo();
}

void
Rede_Server::Roteador::recebeEnca( QString _dado )
{
    if (! this->getJogadorById( this->getIdJogador( _dado ) )->game_over )
    {
        _dado.append(";" + this->geraNovaPeca() );
        this->broadcast(_dado);
    }
}

void
Rede_Server::Roteador::recebeAzia( QString _dado )
{
    QString
    id_azia = QString::number((qrand() % 7 )+ 1);

    QString
    resposta = _dado +";"+id_azia;

    this->broadcast( resposta );
}

void
Rede_Server::Roteador::declaraFimJogo()
{
    Jogador*
    jogador;

    qSort(this->listaJogadores);

    QString
    mensagem_rank = "rank:";

    foreach( jogador, this->listaJogadores )
    {
        mensagem_rank += QString::number(jogador->id_jogador) + ",";
    }

    mensagem_rank.remove(mensagem_rank.length() -1 , 1);

    qDebug() << mensagem_rank;
    this->broadcast(mensagem_rank);
}


bool
Rede_Server::Roteador::checkAllStart()
{
    Rede_Server::Jogador*
    cursor;

    foreach( cursor, this->listaJogadores )
    {
        if (! cursor->estou_pronto )
        {
            return false;
        }
    }

    if ( this->njogadores == this->listaJogadores.count() )
        return true;
    return false;
}

QString
Rede_Server::Roteador::geraNovaPeca()
{
    return QString::number((qrand() % 7 )+ 1);
}
