#include "tradutor.h"
#include <QStringList>
#include <QStringListIterator>
#include <QList>

using namespace Rede_Cliente;

Tradutor::Tradutor(QWidget *_parent):QWidget(_parent)
{
    this->conexao = new Conexao(this);

    QObject::connect(this->conexao, SIGNAL(connected()),
                     this, SLOT(conexaoEstabelecidaInterna()));
    QObject::connect(this->conexao, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(erroConexaoInterna(QAbstractSocket::SocketError)));
    QObject::connect(this->conexao, SIGNAL(incommingMessage(QString)),
                     this, SLOT(incomingMessage(QString)));
}

Tradutor::~Tradutor()
{
}

void
Tradutor::chat(QString _mensagem)
{
    _mensagem = "chat:" + this->idJogador + ";" + _mensagem;
    this->conexao->enviaDado(_mensagem);
}
void
Tradutor::azia(quint16 _id)
{
    QString _mensagem;
    _mensagem = "Esta dando um azia frenética em "+this->listaJogadores[_id];
    this->chat(_mensagem);

    _mensagem = "azia:" + this->idJogador + ";" + QString::number(_id);

    this->conexao->enviaDado(_mensagem);
}
void
Tradutor::conectar(QString _ipServidor, quint16 _portaServidor, QString _nomeJogador)
{
    this->conexao->connectToHost(_ipServidor, _portaServidor);
    this->nomeJogador = _nomeJogador;
}

void
Tradutor::conexaoEstabelecidaInterna()
{
    emit this->conexaoEstabelecida();
}

void
Tradutor::desce()
{
    QString _mensagem;
    _mensagem = "butt:" + this->idJogador + ";4";
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::encaixe()
{
    QString _mensagem;
    _mensagem = "enca:" + this->idJogador;
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::enviaNome()
{
    QString _mensagem;
    _mensagem = "novo:" + this->idJogador + ";" + this->nomeJogador;
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::erroConexaoInterna(QAbstractSocket::SocketError _erroConexao)
{
    Q_UNUSED(_erroConexao);
    emit this->erroConexao(this->conexao->errorString());
}

void
Tradutor::gameOver(quint16 _pontuacao)
{
    QString _mensagem;
    _mensagem = "over:" + this->idJogador + ";" + QString::number(_pontuacao);
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::getListaJogadores(QString _listaJogadores)
{
    QString
    jogador = this->idJogador + ";" + this->nomeJogador;

    quint16 id;
    QString nome;

    _listaJogadores = _listaJogadores.split(':')[1];

    if(_listaJogadores.split(' ').last() == jogador)
    {
        QStringList jogadores = _listaJogadores.split(' ');
        for(int i=0; i<jogadores.size(); i++)
        {
            id = jogadores[i].split(';')[0].toInt();
            nome = jogadores[i].split(';')[1];
            this->listaJogadores[id] = nome;
            emit this->novoJogador(id, nome);
        }
    }
    else
    {
        jogador = _listaJogadores.split(' ').last();
        id = jogador.split(';')[0].toInt();
        nome = jogador.split(';')[1];
        this->listaJogadores[id] = nome;
        emit this->novoJogador(id, nome);
    }
}

int
Tradutor::getPrimeiroParametro(QString _mensagem)
{
    int parametro = _mensagem.split(':')[1].split(';')[0].toInt();

    return parametro;
}

void
Tradutor::getRank(QString _mensagem)
{
    QStringList rank_nome;
    QStringList rank_id = _mensagem.split(':').last().split(',');

    QString id_str;

    foreach(id_str,rank_id)
    {
        quint16 id = id_str.toInt();
        rank_nome.push_back( this->listaJogadores[id] );
    }

    emit this->rank(rank_nome);
}

QString
Tradutor::getSegundoParametroStr(QString _mensagem)
{
    return _mensagem.split(";")[1];
}

int
Tradutor::getSegundoParametroInt(QString _mensagem)
{
    return this->getSegundoParametroStr(_mensagem).toInt();
}

void
Tradutor::movimentaDireita()
{
    QString _mensagem;
    _mensagem = "butt:" + this->idJogador + ";2";
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::movimentaEsquerda()
{
    QString _mensagem;
    _mensagem = "butt:" + this->idJogador + ";3";
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::rotacao()
{
    QString _mensagem;
    _mensagem = "butt:" + this->idJogador + ";1";
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::start(bool _started)
{
    QString _mensagem;
    if(_started)
    {
        _mensagem = "star:" + this->idJogador + ";1";
    }
    else
    {
        _mensagem = "star:" + this->idJogador + ";0";
    }
    this->conexao->enviaDado(_mensagem);
}

void
Tradutor::incomingMessage(QString _mensagem)
{
    if(this->idJogador.isEmpty())
    {
        this->idJogador = _mensagem.mid(5);
    }

    Comando traduzido = this->traduzir(_mensagem);

    switch (traduzido.comando)
    {
        case INIT:
            this->enviaNome();
            emit this->id(idJogador.toInt());
            break;
        case NOVO:
            this->getListaJogadores(_mensagem);
            break;
        case DOWN:
            emit this->down(traduzido.idJogador);
            break;
        case CHAT:
            emit this->chat(this->listaJogadores[traduzido.idJogador], traduzido.segundoStr);
            break;
        case STAR:
            emit this->startjogo(traduzido.idJogador, traduzido.segundoInt);
            break;
        case BUTT:
            switch (traduzido.segundoInt)
            {
                case 1:
                    emit this->rotacionaPeca(traduzido.idJogador);
                    break;
                case 2:
                    emit this->movePecaDireita(traduzido.idJogador);
                    break;
                case 3:
                    emit this->movePecaEsquerda(traduzido.idJogador);
                    break;
                case 4:
                    emit this->descePeca(traduzido.idJogador);
                    break;
                default:
                    break;
                }
            break;
        case ENCA:
            emit this->novapeca(traduzido.idJogador, traduzido.segundoInt);
            break;
        case OVER:
            emit this->over(traduzido.idJogador, traduzido.segundoInt);
            break;
        case RANK:
            this->getRank(_mensagem);
            break;
        case AZIA:            
            emit this->daAzia(traduzido.segundoInt);
            break;
        default:
            break;
        }
}

Comando
Tradutor::traduzir(QString _mensagem)
{
    QString comando = _mensagem.split(':')[0];

    Comando parseado;

    if(comando == "butt")
    {
        parseado.comando = BUTT;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoInt = this->getSegundoParametroInt(_mensagem);
    }
    else if (comando == "enca")
    {
        parseado.comando = ENCA;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoInt = this->getSegundoParametroInt(_mensagem);
    }
    else if(comando == "chat")
    {
        parseado.comando = CHAT;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoStr = this->getSegundoParametroStr(_mensagem);
    }
    else if(comando == "novo")
    {
        parseado.comando = NOVO;
    }
    else if(comando == "init")
    {
        parseado.comando = INIT;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
    }
    else if(comando == "down")
    {
        parseado.comando = DOWN;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
    }
    else if(comando == "star")
    {
        parseado.comando = STAR;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoInt = this->getSegundoParametroInt(_mensagem);
    }
    else if(comando == "over")
    {
        parseado.comando = OVER;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoInt = this->getSegundoParametroInt(_mensagem);
    }
    else if(comando == "rank")
    {
        parseado.comando = RANK;
    }
    else if (comando == "azia")
    {        
        parseado.comando = AZIA;
        parseado.idJogador = this->getPrimeiroParametro(_mensagem);
        parseado.segundoInt = this->getSegundoParametroInt(_mensagem);
    }

    return parseado;
}


