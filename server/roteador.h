#ifndef ROTEADOR_H
#define ROTEADOR_H

#include <QList>
#include <QObject>
#include <QTime>

#include "./jogador.h"
#include "./ouvinte.h"

namespace Rede_Server
{

    class Roteador : public QObject
    {
        Q_OBJECT
    public:

        Roteador( int _njogadores, QObject* _parent = 0 );

        ~Roteador();

        void
        stopListen();

    public slots:

        void
        novoJogador( Jogador* _novo_jogador );

        void
        recebeDado( QString _dado );

    protected slots:

        void
        jogadorCaiu( Jogador* _com_erro );

    protected:

        void
        startJogo();

        void
        setNomeJogador( QString _dado );

        quint16
        getIdJogador( QString _dado );

        Jogador*
        getJogadorById( quint16 _id_jogador );

        void
        enviaListaJogadores( quint16 _id_jogador );

        void
        recebeStart( QString _dado );

        void
        recebeNovo( QString _dado );

        void
        recebeOver( QString _dado );

        void
        recebeEnca( QString _dado );

        void
        recebeAzia( QString _dado );

        void
        declaraFimJogo();

        bool
        checkAllStart();

        QString
        geraNovaPeca();

    private:

        QList<Jogador*>
        listaJogadores;

        Ouvinte*
        ouvinte;

        quint16
        njogadores;

        unsigned int
        current_id;

        bool
        has_started;

    signals:

        void
        broadcast( QString _dado );

        void
        ninguemConectado();

    };

}
#endif // ROTEADOR_H
