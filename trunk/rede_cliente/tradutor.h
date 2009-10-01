#ifndef TRADUTOR_H
#define TRADUTOR_H

#include <QObject>
#include "conexao.h"

#include <QMap>
#include <QKeyEvent>
#include <QWidget>

namespace Rede_Cliente
{
    enum TipoComando{
        INIT,
        NOVO,
        DOWN,
        CHAT,
        STAR,
        BUTT,
        ENCA,
        OVER,
        RANK,
        AZIA
    };

    typedef struct {
        TipoComando comando;
        int idJogador;
        int segundoInt;
        QString primeiroStr;
        QString segundoStr;
    }Comando;



    class Tradutor : public QWidget
    {
        Q_OBJECT


    public:

        Tradutor(QWidget * = 0);

        ~Tradutor();


    public slots:

//        slots de comunicação com a rede
        void
        conectar(QString, quint16, QString);

        void
        conexaoEstabelecidaInterna();

        void
        erroConexaoInterna(QAbstractSocket::SocketError);

        void
        incomingMessage(QString);

//        slots de transferencia de mensagens

        void
        chat(QString);

        void
        azia(quint16);

        void
        desce();

        void
        encaixe();
        
        void
        gameOver(quint16);

        void
        movimentaDireita();

        void
        movimentaEsquerda();

        void
        rotacao();

        void
        start(bool);


    protected:

        void
        enviaNome();

        void
        getListaJogadores(QString);

        int
        getPrimeiroParametro(QString);

        void
        getRank(QString);

        QString
        getSegundoParametroStr(QString);

        int
        getSegundoParametroInt(QString);

        QMap<quint16, QString>
        listaJogadores;

        Comando
        traduzir(QString);

    private:

        Conexao
        *conexao;

        QString
        nomeJogador;

        QString
        idJogador;


    signals:

        void
        chat(QString, QString);

        void
        daAzia(quint16);

        void
        conexaoEstabelecida();

        void
        down(quint16);

        void
        descePeca(quint16);

        void
        erroConexao(QString);

        void
        id(quint16);

        void
        movePecaDireita(quint16);

        void
        movePecaEsquerda(quint16);

        void
        novoJogador(quint16, QString);

        void
        rank(QStringList);

        void
        rotacionaPeca(quint16);

        void
        startjogo(quint16, quint16);

        void
        novapeca(quint16, quint16);

        void
        over(quint16, quint16);


    };
}

#endif // TRADUTOR_H
