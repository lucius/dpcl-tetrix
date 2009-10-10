#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <QtGui/QWidget>
#include <QKeyEvent>
#include <QPushButton>

#include "basicos.h"
#include "pivo.h"



class Azia;
class AziaEscondeTabuleiro;

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

        void
        aziado();

        QPushButton * btn_azia;

        int
        getPontuacao();

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
        setAzias(int);

        int
        getAzias();

        void
        setPontuacao( int );

        void
        desce();

        void
        desceTudo();

        void
        habilitaAzia(bool);

        void
        daAziaEmAlguem();

    signals:
        void
        encaixe( );

        void
        gameover( int pontuacao );

        void
        aziasMudou(int );

        void
        aziaEmAlguem();

        void
        pontuacaoMudou( int);

    #if !defined(Q_MOC_RUN)
    private: // can only be emitted by TetrixBoard
    #endif

        void
        linhaCheia( Tab::XyView );

        void
        procuraLinhas( );





    protected:
        QTimer*
        timer;

        int
        pontuacao;

        int
        azias;

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

        QColor
        getNewColor( qint8 );

    private:
        friend class Azia;
        friend class AziaEscondeTabuleiro;

        Ui::Tabuleiro *ui;

        bool
        posso_mexer;
};


#endif // TABULEIRO_H
