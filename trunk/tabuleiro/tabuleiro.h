#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <QtGui/QWidget>
#include <QKeyEvent>

#include "basicos.h"
#include "pivo.h"

namespace Ui
{
    class Tabuleiro;
}

class Tabuleiro : public QWidget
{
    Q_OBJECT

    public:
        Tabuleiro( QString nome="boga", QWidget *parent = 0 );
        ~Tabuleiro( );

        void
        setNomeJogador( QString );

        QString
        getNomeJogador( );

        //todo;
        void
        clear() {}

    public slots:
        void
        rotacionapeca();

        void
        movepeca( bool _direcao );

        void
        startjogo( quint16 descendo, quint16 proxima );

        void
        novapeca( qint8 nova );

        void
        colidiu( );

        void
        procuraLinhasCheias( );

        void
        apagaLinhaCheia( Tab::XyView );

        void
        setLevel( );

        void
        setPontuacao( );

        void
        desce();

        void
        desceTudo();

    signals:
        void
        encaixe( );

        void
        gameover( int pontuacao );

    #if !defined(Q_MOC_RUN)
    private: // can only be emitted by TetrixBoard
    #endif

        void
        linhaCheia( Tab::XyView );

        void
        procuraLinhas( );

        void
        levelMudou( );

        void
        pontuacaoMudou( );

    protected:
        QTimer*
        timer;

        unsigned int
        pontuacao;

        unsigned int
        level;

        unsigned int
        numeroPecasUtilizadas;

        Tab::Pivo*
        currentPiece;

        Tab::Pivo*
        previewPiece;

        void
        desceLinhas( Tab::XyView );

        unsigned int
        getTimeOut( );

        bool
        checkGameOver( Tab::TipoPeca );

    private:
        Ui::Tabuleiro *ui;

        bool
        posso_mexer;
};

#endif // TABULEIRO_H
